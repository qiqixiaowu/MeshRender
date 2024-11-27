#pragma once

#include <map>
#include <vector>
#include "cubemesh.h"
 
class MeshBuilder
{
public:
    MeshBuilder() : mesh(), m_hashMap() {}

    void AddTriangle(const Point3d& p0, const Point3d& p1, const Point3d& p2) {
        int p0i, p1i, p2i;

        auto it = m_hashMap.find(p0);
        if (it == m_hashMap.end()) {
            p0i = mesh.AddVertex(p0);
            m_hashMap[p0] = p0i;
        }
        else {
            p0i = it->second;
        }

        it = m_hashMap.find(p1);
        if (it == m_hashMap.end()) {
            p1i = mesh.AddVertex(p1);
            m_hashMap[p1] = p1i;
        }
        else {
            p1i = it->second;
        }

        it = m_hashMap.find(p2);
        if (it == m_hashMap.end()) {
            p2i = mesh.AddVertex(p2);
            m_hashMap[p2] = p2i;
        }
        else {
            p2i = it->second;
        }

        Triangle t = { p0i, p1i, p2i };
        mesh.AddFace(t);
    }

    CubeMesh GetMesh() const {
        return mesh;
    }

    void Clear() {
        m_hashMap.clear();
        mesh.Vertices.clear();
        mesh.Faces.clear();  
    }

private:
    CubeMesh mesh;
    std::map<Point3d, int> m_hashMap;
};
 