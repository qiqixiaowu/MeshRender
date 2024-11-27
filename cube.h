#pragma once

#include <array>

typedef unsigned char byte;

struct Int16Triple {
    int x, y, z;
    Int16Triple() : x(0), y(0), z(0) {}
    Int16Triple(int x, int y, int z) : x(x), y(y), z(z) {}
};

class Cube {
public:
    // 定义体素为实点的位标记
    static const byte VULF = 1;    // 00000001
    static const byte VULB = 2;    // 00000010
    static const byte VLLB = 4;    // 00000100
    static const byte VLLF = 8;    // 00001000
    static const byte VURF = 16;   // 00010000
    static const byte VURB = 32;   // 00100000
    static const byte VLRB = 64;   // 01000000
    static const byte VLRF = 128;  // 10000000

    // 体元内每个体素相对基准体素坐标的偏移
    static const std::array<Int16Triple, 8> PointIndexToPointDelta;
    // 每个体素对应的位标记
    static const std::array<byte, 8> PointIndexToFlag;
    static const std::array<std::array<int, 2>, 12> EdgeIndexToEdgeVertexIndex;

    int CellIndexX,CellIndexY, CellIndexZ;

    // 用于存储8个体素的坐标
    std::array<Int16Triple, 8> cubeImageIndices;

    Cube(int cellIndexX, int cellIndexY, int cellIndexZ)
        : CellIndexX(cellIndexX), CellIndexY(cellIndexY), CellIndexZ(cellIndexZ),
        cubeImageIndices{}  
    {
        for (int i = 0; i < 8; ++i) {
            cubeImageIndices[i] = Int16Triple(
                cellIndexX + PointIndexToPointDelta[i].x,
                cellIndexY + PointIndexToPointDelta[i].y,
                cellIndexZ + PointIndexToPointDelta[i].z
            );
        }
    }
};

const std::array<Int16Triple, 8> Cube::PointIndexToPointDelta = { {
    Int16Triple(0, 1, 1),
    Int16Triple(0, 1, 0),
    Int16Triple(0, 0, 0),
    Int16Triple(0, 0, 1),
    Int16Triple(1, 1, 1),
    Int16Triple(1, 1, 0),
    Int16Triple(1, 0, 0),
    Int16Triple(1, 0, 1)
} };

const std::array<byte, 8> Cube::PointIndexToFlag = { {
    VULF, VULB, VLLB, VLLF, VURF, VURB, VLRB, VLRF
} };

const std::array<std::array<int, 2>, 12> Cube::EdgeIndexToEdgeVertexIndex = { {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
} };