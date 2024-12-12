#pragma once

#include <map>
#include <vector>
#include "cubemesh.h"
 

class MeshBuilder
{
public:
    MeshBuilder() : mesh(), m_hashMap() {}

	void AddTriangle(const Point3d& p0, const Point3d& p1, const Point3d& p2, const std::array<Point3d, 3>& p3) {
		int p0i, p1i, p2i;

		auto it = m_hashMap.find(p0);
		if (it == m_hashMap.end()) {
			p0i = mesh.AddVertex(p0);
            //mesh.AddNormal(p3[0]);
            m_hashMap[p0] = p0i;
        }
        else {
            p0i = it->second;
        }

        it = m_hashMap.find(p1);
        if (it == m_hashMap.end()) {
            p1i = mesh.AddVertex(p1);
            //mesh.AddNormal(p3[1]);
            m_hashMap[p1] = p1i;
        }
        else {
            p1i = it->second;
        }

        it = m_hashMap.find(p2);
        if (it == m_hashMap.end()) {
            p2i = mesh.AddVertex(p2);
            //mesh.AddNormal(p3[2]);
            m_hashMap[p2] = p2i;
        }
        else {
            p2i = it->second;
        }

        Triangle t = { p0i, p1i, p2i };

        mesh.AddFace(t);
    }

    std::vector<float> crossProduct(const std::vector<float>& v1, const std::vector<float>& v2) {
        return {
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
        };
    }
 
    std::vector<float> normalize(const std::vector<float>& v) {
        float length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (length == 0.0f) {
            return { 0.0f, 0.0f, 1.0f };
        }
        return {
            v[0] / length,
            v[1] / length,
            v[2] / length
        };
	}

	std::vector<float> vectorSubtract(const std::vector<float>& v1, const std::vector<float>& v2) {

		if (v1.size() != v2.size()) {
			throw std::invalid_argument("Vectors must be of the same size to subtract them.");
		}

		std::vector<float> result(v1.size());
		for (size_t i = 0; i < v1.size(); ++i) {
			result[i] = v1[i] - v2[i];
		}
		return result;
	}

    std::vector<Point3d> ComputeVertexNormals(std::vector<Point3d>& vertices) {
        std::vector<std::vector<float>> vertexNormals(vertices.size(), std::vector<float>(3, 0.0f));

            std::vector<float> v0 = { vertices[0].x, vertices[0].y, vertices[0].z };
            std::vector<float> v1 = { vertices[1].x, vertices[1].y, vertices[1].z };
            std::vector<float> v2 = { vertices[2].x, vertices[2].y, vertices[2].z };

            std::vector<float> faceNormal = normalize(crossProduct(vectorSubtract(v1,v0), vectorSubtract(v2, v0)));

            vertexNormals[0] = { vertexNormals[0][0] + faceNormal[0],
                                vertexNormals[0][1] + faceNormal[1],
                                vertexNormals[0][2] + faceNormal[2] };
            vertexNormals[1] = { vertexNormals[1][0] + faceNormal[0],
                                vertexNormals[1][1] + faceNormal[1],
                                vertexNormals[1][2] + faceNormal[2] };
			vertexNormals[2] = { vertexNormals[2][0] + faceNormal[0],
								vertexNormals[2][1] + faceNormal[1],
								vertexNormals[2][2] + faceNormal[2] };

			std::vector<Point3d> vecPoint;
			for (auto& normal : vertexNormals) {
				normal = normalize(normal);
				Point3d point3d(normal[0], normal[1], normal[2]);
				vecPoint.push_back(point3d);
			}
            return vecPoint;
	}


    CubeMesh GetMesh() const {
        return mesh;
    }

    void Clear() {
        m_hashMap.clear();
        mesh.Vertices.clear();
        mesh.Faces.clear();  
        mesh.Normals.clear();
    }

private:
    CubeMesh mesh;
    std::map<Point3d, int> m_hashMap;
};
 