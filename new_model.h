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
	std::shared_ptr<Mesh> GetMesh();
private:
	std::shared_ptr<Mesh> m_cachedMesh;

private:
	std::vector<CubeMesh> m_meshes;
};
