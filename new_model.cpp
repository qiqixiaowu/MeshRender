#include "new_model.h"

NewModel::NewModel(CubeMesh cubeMesh)
{
	m_meshes.push_back(cubeMesh);
}

void NewModel::Draw(Shader* shader)
{
	auto mesh = GetMesh();

	mesh.DrawElements(shader);
}

Mesh NewModel::GetMesh()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < m_meshes[0].Vertices.size(); i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = m_meshes[0].Vertices[i].x;
		vector.y = m_meshes[0].Vertices[i].y;
		vector.z = m_meshes[0].Vertices[i].z;
		vertex.Position = vector;

		vector.x = m_meshes[0].Vertices[i].x;
		vector.y = m_meshes[0].Vertices[i].y;
		vector.z = m_meshes[0].Vertices[i].z;
		vertex.Normal = vector;

		vector.x = 0.5;
		vector.y = 0.6;
		vector.z = 0.2;
		vertex.Color = vector;

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < m_meshes[0].Faces.size(); i++)
	{
		indices.push_back(3 * i + 0);
		indices.push_back(3 * i + 1);
		indices.push_back(3 * i + 2);
	}
	return Mesh(vertices, indices);
}
