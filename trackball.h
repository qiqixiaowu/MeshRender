#pragma once

#include <iostream>

#include <glfw3.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/common.hpp>

#include <math.h>
#include <algorithm>

#include "camera.h"

class Trackball
{
private:
	int windowWidth;
	int windowHeight;
	int mouseEvent;
	GLfloat rollSpeed;
	GLfloat angle;
	glm::vec3 prevPos;
	glm::vec3 currPos;
	glm::vec3 camAxis;
	bool flag;

public:
	bool track;

	Trackball(int window_width, int window_height, GLfloat roll_speed = 1.0f);
	glm::vec3 ToScreenCoord(double x, double y);

	void MouseButtonCallback(bool _track, double x, double y);
	void CursorCallback(GLFWwindow* window, double x, double y);

	glm::mat4 CreateViewRotationMat(float deltaTime);
	glm::mat4 CreateWorldRotationMat(glm::mat4 view_matrix, float deltaTime, bool& f);
	glm::mat4 CreateModelRotationMat(glm::mat4 view_matrix, glm::mat4 model_matrix, float deltaTime);
};

