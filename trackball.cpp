#include "trackball.h"

Trackball::Trackball(int window_width, int window_height, GLfloat roll_speed)
{
	this->windowWidth = window_width;
	this->windowHeight = window_height;

	this->mouseEvent = 0;
	this->rollSpeed = roll_speed;
	this->flag = false;
}


glm::vec3 Trackball::ToScreenCoord(double x, double y)
{
	glm::vec3 coord(0.0f);

	double wX = windowWidth - 1;
	double wY = windowHeight - 1;

	//if (xAxis)
	coord.x = (2 * x - wX) / wX;

	//if (yAxis)
	coord.y = -(2 * y - wY) / wY;

	float length2 = coord.x * coord.x + coord.y * coord.y;
	coord.z = sqrt(1.0 - length2);

	return glm::normalize(coord);
}


void Trackball::MouseButtonCallback(bool _track, double x, double y)
{
	if (_track)
	{
		/* Start of trackball, remember the first position */
		prevPos = ToScreenCoord(x, y);
		currPos = prevPos;
		this->track = true;
		return;
	}
	else
	{
		this->track = false;
	}
}

void Trackball::CursorCallback(GLFWwindow* window, double x, double y)
{
	if (!track)
		return;

	currPos = ToScreenCoord(x, y);

	float t = glm::dot(prevPos, currPos);

	float dist = glm::distance(prevPos, currPos);

	if (abs(dist) < 0.000001f)
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	if (t > 1.0)
	{
		t = 1.0;
	}
	if (t < -1.0) t = -1.0;

	angle = acos(t);

	camAxis = glm::cross(prevPos, currPos);

	camAxis = glm::normalize(camAxis);

	prevPos = currPos;
}

glm::mat4 Trackball::CreateViewRotationMat(float deltaTime)
{
	if (flag)
	{
		return glm::mat4(1.0f);
	}

	glm::vec3 axis = glm::normalize(camAxis);
	float angleDegrees = glm::degrees(angle) * rollSpeed * deltaTime;
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);

	return rotationMatrix;
}

glm::mat4 Trackball::CreateWorldRotationMat(glm::mat4 view_matrix, float deltaTime, bool& f)
{
	f = flag;
	glm::vec3 axis = glm::mat3(glm::inverse(view_matrix)) * camAxis;
	if (glm::length(axis) > 0.0001) // 确保轴向量不是零向量
	{
		axis = glm::normalize(axis);
	}
	else
	{
		return glm::mat4(1.0); 
	}
	// 回传的矩阵应当表示以axis为轴，旋转的角度
	float angleDegrees = glm::degrees(angle) * rollSpeed * deltaTime;  // 转换为度
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);
	return rotationMatrix;
}

glm::mat4 Trackball::CreateModelRotationMat(glm::mat4 view_matrix, glm::mat4 model_matrix, float deltaTime)
{
	if (flag)
	{
		return glm::mat4(1.0f);  
	}

	glm::vec3 axis = glm::mat3(glm::inverse(view_matrix * model_matrix)) * camAxis;
	if (glm::length(axis) > 0.0001) {
		axis = glm::normalize(axis);
	}
	else
	{
		return glm::mat4(1.0);  
	}

	float angleDegrees = glm::degrees(angle) * rollSpeed * deltaTime;
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);

	return rotationMatrix;
}