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
        return builder.GetMesh();
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

                Int16Triple e0p0 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e0index][0]];
                Int16Triple e0p1 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e0index][1]];

                Int16Triple e1p0 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e1index][0]];
                Int16Triple e1p1 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e1index][1]];

                Int16Triple e2p0 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e2index][0]];
                Int16Triple e2p1 = cell.cubeImageIndices[Cube::EdgeIndexToEdgeVertexIndex[e2index][1]];

                Point3d e0pm = GetIntersetedPoint(e0p0, e0p1);
                Point3d e1pm = GetIntersetedPoint(e1p0, e1p1);
                Point3d e2pm = GetIntersetedPoint(e2p0, e2p1);

                builder.AddTriangle(e0pm, e1pm, e2pm);

                index += 3;
            }
        }
    }

    Point3d GetIntersetedPoint(const Int16Triple& p0, const Int16Triple& p1) const {
        return Point3d((p0.x + p1.x) / 2.0f, (p0.y + p1.y) / 2.0f, (p0.z + p1.z) / 2.0f);
    }
};