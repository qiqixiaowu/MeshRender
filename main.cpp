#include "Shader.h"

#include <iostream>
#include <filesystem>

#define GLEW_STATIC

#include <glfw3.h>

#include "Camera.h"
#include "trackball.h"
#include "Mesh.h"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dcmtk/dcmdata/dcdatset.h" 
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcelem.h"
#include <dcmtk/dcmdata/dcfilefo.h>


#include "mc_processor.h"
#include "dicom_image3d.h"
#include "export_ply.h"
#include "new_model.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

float quadVertices[] = 
{
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

#pragma region Camera
bool mouseBtnPressed = false;
static Trackball trackball(640, 480, 2.0f);
glm::mat4 modelMat = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(20.0f, 0.0f, 220.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 5.0f, 0.0f);
Camera camera(cameraPos, cameraTarget, cameraUp);
#pragma endregion

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

#pragma region Device
float deltaTime = 0.0f;  
float lastFrame = 0.0f;  

void ProcessInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	trackball.CursorCallback(window, xpos, ypos);

	if (mouseBtnPressed)
	{
		bool flag = false;
		glm::mat4 newRotation = trackball.CreateWorldRotationMat(camera.GetViewMatrix(), deltaTime * 50, flag);

		if (!flag)
		{
			modelMat = newRotation * modelMat;
		}
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			trackball.MouseButtonCallback(true, xpos, ypos);
			mouseBtnPressed = true;
		}
		if (action == GLFW_RELEASE)
		{
			trackball.MouseButtonCallback(false, 0, 0);
			mouseBtnPressed = false;
		}
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
#pragma endregion
 
std::shared_ptr<UDicomImage3D> GenerateDicomImage3D(const std::vector<std::shared_ptr<UDicomImage>>& vecDicomImage)
{
	std::shared_ptr<UDicomImage3D> spDicomImage3D(new UDicomImage3D());
	spDicomImage3D->m_iDimension[0] = vecDicomImage[1]->GetColunm();
	spDicomImage3D->m_iDimension[1] = vecDicomImage[0]->GetRow();
	spDicomImage3D->m_iDimension[2] = vecDicomImage.size();
	spDicomImage3D->m_dSlope = vecDicomImage[0]->GetSlop();
	spDicomImage3D->m_dIntercept = vecDicomImage[0]->GetIntercept();

	unsigned int iSize = spDicomImage3D->m_iDimension[0] * spDicomImage3D->m_iDimension[1] * spDicomImage3D->m_iDimension[2];
	int iSliceSize = spDicomImage3D->m_iDimension[0] * spDicomImage3D->m_iDimension[1];

	if (!vecDicomImage[0] || !vecDicomImage[0]->GetPixelPointer())
	{
		throw std::runtime_error("Invalid image or pixel data pointer");
	}

	spDicomImage3D->m_spPixelPointer = new unsigned short[iSize];

	if (vecDicomImage[0]->GetBitAllocated() == 16)
	{
		for (int i = 0; i < spDicomImage3D->m_iDimension[2]; ++i)
		{
			std::memcpy((unsigned short*)spDicomImage3D->m_spPixelPointer + i * iSliceSize,
				vecDicomImage[i]->GetPixelPointer(), iSliceSize * 2);
		}
	}

	return spDicomImage3D;
}


int main() 
{
	std::filesystem::path current_path = std::filesystem::current_path();
	std::string directoryPath = "E:\\Data\\BPTQ_PET_AD_2018_024_110859\\2.850 x 300_301";

	OFList<OFString> fileList;
	OFStandard::searchDirectoryRecursively(directoryPath.c_str(), fileList, "", "", true);
	if (fileList.empty())
	{
		return 0;
	}

	std::vector<std::shared_ptr<UDicomImage>> vecDicomImage;
	for (const auto& file : fileList)
	{
		std::shared_ptr<DcmFileFormat> fileformat(new DcmFileFormat);
		if (!fileformat->loadFile(file.c_str()).good())
		{
			break;
		}

		auto pDataset = fileformat->getDataset();

		std::shared_ptr<UDicomImage> spDicomImage = std::make_shared<UDicomImage>();
		spDicomImage->InitImage(*pDataset);
		vecDicomImage.push_back(spDicomImage);
	}

	//重建
	auto spDicomImage3D = GenerateDicomImage3D(vecDicomImage);
 	//归一化
	std::vector<unsigned char> vecPixelValue(spDicomImage3D->m_iDimension[1] * spDicomImage3D->m_iDimension[0] * spDicomImage3D->m_iDimension[2]);
	const unsigned short* begin = spDicomImage3D->GetPixelPointer();
	const unsigned short* end = spDicomImage3D->GetPixelPointer() + spDicomImage3D->m_iDimension[0] * spDicomImage3D->m_iDimension[1] * spDicomImage3D->m_iDimension[2];
	auto [minVal, maxVal] = std::minmax_element(begin, end);

	for (int z = 0; z < spDicomImage3D->m_iDimension[2]; z++)
	{
		for (int y = 0; y < spDicomImage3D->m_iDimension[1]; y++)
		{
			for (int x = 0; x < spDicomImage3D->m_iDimension[0]; x++)
			{
				unsigned short pixelValue = spDicomImage3D->GetPixelPointer()[z * spDicomImage3D->m_iDimension[1] * spDicomImage3D->m_iDimension[0] + y * spDicomImage3D->m_iDimension[0] + x];
				unsigned char normPixelValue = static_cast<unsigned char>(255 * (pixelValue - *minVal) / (*maxVal - *minVal));
				vecPixelValue[z * spDicomImage3D->m_iDimension[1] * spDicomImage3D->m_iDimension[0] + y * spDicomImage3D->m_iDimension[0] + x] = normPixelValue;
			}
		}
	}

	BitMap3d bmp(spDicomImage3D->m_iDimension[0], spDicomImage3D->m_iDimension[1], spDicomImage3D->m_iDimension[2], vecPixelValue);
	auto spProcessor = std::make_shared<MCProcessor>(&bmp);

	std::string filePath = "output.ply";
	Output(spProcessor->GeneratorSurface(), filePath);

	return 0;
	auto cubeMesh = spProcessor->GeneratorSurface();

#pragma region  
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Viewer", NULL, NULL);
	if (window == NULL)
	{
		printf("Open window failed.");
		glfwTerminate();
		return -1;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int screenHeight = mode->height;
	glfwSetWindowPos(window, 0, 150);

	glfwMakeContextCurrent(window);
 
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

#pragma endregion

	Shader* screenShader = new Shader("screen.vert", "screen.frag");
	Shader* first = new Shader("first.vert", "first.frag");

#pragma region FBO
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	unsigned int renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 300, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion

#pragma region Model
	//std::string exePath = "E:\\Render\MeshRender\\output2.ply";
	//std::string exePath = "E:\\Render\\MeshRender\\model\\111115_lower.obj";
	//Model model(exePath);
	NewModel model(cubeMesh);

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//坐标翻转
	stbi_set_flip_vertically_on_load(true);
#pragma endregion

#pragma region  MVP 
	glm::mat4 viewMat;
	glm::mat4 projMat;

	glUniformBlockBinding(first->ID, glGetUniformBlockIndex(first->ID, "Matrices"), 0);
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//设置uniform缓冲中的观察矩阵
		viewMat = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMat));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//设置透视矩阵
		projMat = glm::perspective(glm::radians(camera.Zoom), 640.0f / 480.0f, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projMat));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glEnable(GL_CULL_FACE);	
		first->use();	 
		glUniformMatrix4fv(glGetUniformLocation(first->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniform3f(glGetUniformLocation(first->ID, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(first->ID, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(first->ID, "viewPos"), 1, &camera.Position[0]);

		model.Draw(first);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader->use();
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
 
		glfwSwapBuffers(window);
		glfwPollEvents();

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
 
	glfwTerminate();
	return 0;
}