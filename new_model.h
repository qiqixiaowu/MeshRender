#pragma once
#include <vector>
#include <string>
#include "cubemesh.h"
#include "shader.h"
#include "mesh.h"

class NewModel
{
public:
	NewModel(CubeMesh cubeMesh);
	void Draw(Shader* shader);
	Mesh GetMesh();

private:
	std::vector<CubeMesh> m_meshes;
};
