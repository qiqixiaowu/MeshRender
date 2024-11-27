#pragma once

#include <iostream>
#include <fstream>
#include <vector>
 
using byte = unsigned char;

class BitMap3d 
{
public:
    static const byte WHITE = 255;
    static const byte BLACK = 0;

    std::vector<byte> data;
    int width;
    int height;
    int depth;

    BitMap3d(int width, int height, int depth, const std::vector<byte>& vec)
        : width(width), height(height), depth(depth), data(vec)
	{
	}

    void SetPixel(int x, int y, int z, byte v) 
    {
        data[x + y * width + z * width * height] = v;
    }

    byte GetPixel(int x, int y, int z) const 
    {
        return data[x + y * width + z * width * height];
    }

};