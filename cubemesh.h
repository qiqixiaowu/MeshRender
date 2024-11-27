#pragma once
#include <vector>

 
struct Point3d {
    Point3d(float x = 0, float y = 0, float z = 0){
        this->x = x;
        this->y = y;
        this->z = z;
    }
 
    bool operator<(const Point3d& other) const {
        if (x != other.x) return x< other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

    float x;
    float y;
    float z;
};

struct Triangle {
    int P0Index;
    int P1Index;
    int P2Index;

    Triangle(int p0index, int p1index, int p2index)
        : P0Index(p0index), P1Index(p1index), P2Index(p2index) {}
};

class CubeMesh {
public:
    std::vector<Point3d> Vertices;
    std::vector<Triangle> Faces;
 
    CubeMesh() {}
    int AddVertex(const Point3d& point) {
        Vertices.push_back(point);
        return Vertices.size() - 1; 
    }

    int AddFace(const Triangle& tri) {
        Faces.push_back(tri);
        return Faces.size() - 1;  
    }

    void Clear() {
        Vertices.clear();
        Faces.clear();
    }
};
 