#pragma once
#include <VG/vg.h>

struct BufferDraw
{
    const vg::Buffer* buffer;
    int vertexByteOffset;
    int indexByteOffset;
    int indexType;
    int indexCount;

    BufferDraw(const vg::Buffer& buffer, int vertexByteOffset, int indexByteOffset, int indexType, int indexCount) :
        buffer(&buffer), vertexByteOffset(vertexByteOffset), indexByteOffset(indexByteOffset), indexType(indexType), indexCount(indexCount)
    {}
};