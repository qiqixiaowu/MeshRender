#pragma once
#include <vector>

 
class Point3d {
public:
    Point3d(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}
 
    bool operator<(const Point3d& other) const {
        if (x != other.x) return x< other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

    Point3d operator-(const Point3d& other) const {
        return Point3d(x - other.x, y - other.y, z - other.z);
    }

    Point3d operator+(const Point3d& other) const {
        return Point3d(x + other.x, y + other.y, z + other.z);
    }

    Point3d Cross(const Point3d& other) const {
        return Point3d(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    Point3d Normalized() const {
        double length = Length();
        if (length == 0) {
            return Point3d(0, 0, 0);
        }
        return Point3d(x / length, y / length, z / length);
    }

    double Length() const {
        return sqrt(x * x + y * y + z * z);
    }

    float x, y, z;
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
    std::vector<Point3d> Normals;
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

	int AddNormal(const std::vector<Point3d>& vecPoint) {
		Normals.insert(Normals.end(), vecPoint.begin(), vecPoint.end());
		return Normals.size() - 1;
	}

	void Clear() {
		Vertices.clear();
        Faces.clear();
        Normals.clear();
    }
};
 