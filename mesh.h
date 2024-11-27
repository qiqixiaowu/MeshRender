#pragma once

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include <iostream>

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
};
 
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
 
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void DrawElements(Shader* shader);
	unsigned int VAO, VBO, EBO; 
private:
	void setupElementsMesh();
};