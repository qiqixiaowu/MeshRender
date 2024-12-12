#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include <cmath>

#include "bitmap3d.h"
#include "cubemesh.h"
#include "cube.h"
#include "meshbuilder.h"
#include "mc_table.h"

class MCProcessor {
    BitMap3d* bmp;  
public:
    MCProcessor(BitMap3d* bitmap) : bmp(bitmap) {}

    CubeMesh GeneratorSurface() const {
        MeshBuilder builder;
        for (int k = 0; k < bmp->depth - 1; k++) {
            for (int j = 0; j < bmp->height - 1; j++) {
                for (int i = 0; i < bmp->width - 1; i++) {
                    Cube cell(i, j, k);
                    byte config = GetConfig(cell);
                    ExtractTriangles(cell, config, builder);
                }
            }
        }

		auto mesh = builder.GetMesh();
		auto vertices = mesh.Vertices;
		for (const auto& tri : mesh.Faces)
		{
			Point3d p0 = vertices[tri.P0Index];
			Point3d p1 = vertices[tri.P1Index];
			Point3d p2 = vertices[tri.P2Index];

            Point3d normal = (p1 - p0).Cross(p2 - p0).Normalized();
            mesh.AddNormal(normal);
            mesh.AddNormal(normal);
            mesh.AddNormal(normal);
		}

        return mesh;
    }

private:

    bool IsInside(int x, int y, int z) const {
        if (x <= 0 || y <= 0 || z <= 0 || x > bmp->width || y > bmp->height || z > bmp->depth)
            return false;
        else {
            return bmp->GetPixel(x, y, z);  
        }
    }

    byte GetConfig(const Cube& cube) const {
        byte value = 0;
        for (int i = 0; i < 8; i++) {
            if (IsInside(cube.cubeImageIndices[i].x, cube.cubeImageIndices[i].y, cube.cubeImageIndices[i].z)) {
                value |= Cube::PointIndexToFlag[i];
            }
        }
        return value;
    }

    void ExtractTriangles(const Cube& cell, byte value, MeshBuilder& builder) const {
        if (TriTable[value][0] != -1) {
            int index = 0;
            while (TriTable[value][index] != -1) {
                int e0index = TriTable[value][index];
                int e1index = TriTable[value][index + 1];
                int e2index = TriTable[value][index + 2];

                //体数据中实际点位置
                Int16Triple e0p0 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e0index][0]];
                Int16Triple e0p1 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e0index][1]];

                Int16Triple e1p0 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e1index][0]];
                Int16Triple e1p1 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e1index][1]];

                Int16Triple e2p0 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e2index][0]];
                Int16Triple e2p1 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e2index][1]];

                //两个点的边取中间点
                Point3d e0pm = GetIntersetedPoint(e0p0, e0p1);
                Point3d e1pm = GetIntersetedPoint(e1p0, e1p1);
				Point3d e2pm = GetIntersetedPoint(e2p0, e2p1);

                //处理法线
                //Point3d normal1 = ComputeGradient(e0pm);
                //Point3d normal2 = ComputeGradient(e1pm);
                //Point3d normal3 = ComputeGradient(e2pm);
                //std::array<Point3d, 3> arrNormals = { normal1, normal2, normal3 };

                Point3d normal1= calTriNormal(e0pm, e0pm, e0pm);
                std::array<Point3d, 3> arrNormals = { normal1, normal1, normal1 };
				builder.AddTriangle(e0pm, e1pm, e2pm, arrNormals);

				index += 3;
			}
		}
	}


    Point3d calTriNormal(Point3d ver1, Point3d ver2, Point3d ver3) const
    {
        double temp1[3], temp2[3], normal[3];
        double length = 0.0;
        temp1[0] = ver2.x - ver1.x;
        temp1[1] = ver2.y - ver1.y;
        temp1[2] = ver2.z - ver1.z;
        temp2[0] = ver3.x - ver2.x;
        temp2[1] = ver3.y - ver2.y;
        temp2[2] = ver3.z - ver2.z;
        //计算法线
        normal[0] = temp1[1] * temp2[2] - temp1[2] * temp2[1];
        normal[1] = -(temp1[0] * temp2[2] - temp1[2] * temp2[0]);
        normal[2] = temp1[0] * temp2[1] - temp1[1] * temp2[0];
        //法线单位化
        length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
        if (length == 0.0f) { length = 1.0f; }
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
        Point3d e_normal(normal[0], normal[1], normal[2]);
        return e_normal;
    }

    Point3d ComputeGradient(Point3d& pointIndex) const {
        double value = bmp->GetPixel(pointIndex.x, pointIndex.y, pointIndex.z);

        double dx = 0, dy = 0, dz = 0;
        if (pointIndex.x + 1 < bmp->width) {
            dx = bmp->GetPixel(pointIndex.x + 1, pointIndex.y, pointIndex.z) - value;
        }

        if (pointIndex.y + 1 < bmp->height) {
            dy = bmp->GetPixel(pointIndex.x, pointIndex.y + 1, pointIndex.z) - value;
        }

        if (pointIndex.z + 1 < bmp->depth) {
            dz = bmp->GetPixel(pointIndex.x, pointIndex.y, pointIndex.z + 1) - value;
        }

        return Point3d(dx, dy, dz).Normalized();
    }

    Point3d GetIntersetedPoint(const Int16Triple& p0, const Int16Triple& p1) const {
        return Point3d((p0.x + p1.x) / 2.0f, (p0.y + p1.y) / 2.0f, (p0.z + p1.z) / 2.0f);
    }
};