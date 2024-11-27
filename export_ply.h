#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "cubemesh.h"
 
// 写入顶点信息
void AWriteV(std::ofstream& os, double v1, double v2, double v3, unsigned char r, unsigned char g, unsigned char b) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << v1 << " "
        << std::fixed << std::setprecision(1) << v2 << " "
        << std::fixed << std::setprecision(1) << v3 << " "
        << static_cast<int>(r) << " "
        << static_cast<int>(g) << " "
        << static_cast<int>(b) << "\n";
    os << ss.str();
}

// 写入面信息
void AWriteF(std::ofstream& os, int i1, int i2, int i3) {
    std::stringstream ss;
    ss << "3 " << i1 << " " << i2 << " " << i3 << "\n";
    os << ss.str();
}

// 输出PLY文件
void Output(const CubeMesh& mesh, const std::string& filePath) {
    std::ofstream os(filePath);
    if (!os.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    os << "ply\n"
        << "format ascii 1.0\n"
        << "comment VCGLIB generated\n"
        << "element vertex " << mesh.Vertices.size() << "\n"
        << "property float x\n"
        << "property float y\n"
        << "property float z\n"
        << "property uchar red\n"
        << "property uchar green\n"
        << "property uchar blue\n"
        << "element face " << mesh.Faces.size() << "\n"
        << "property list int int vertex_indices\n"
        << "end_header\n";

    for (size_t i = 0; i < mesh.Vertices.size(); ++i) {
        AWriteV(os, mesh.Vertices[i].x, mesh.Vertices[i].y, mesh.Vertices[i].z, 255, 255, 255);
    }

    for (size_t i = 0; i < mesh.Faces.size(); ++i) {
        AWriteF(os, mesh.Faces[i].P0Index, mesh.Faces[i].P1Index, mesh.Faces[i].P2Index);
    }

    os.close();
}