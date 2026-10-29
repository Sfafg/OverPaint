#pragma once
#include "StructuredBuffer.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

struct Stroke {
    StructuredBuffer<Vertex> vertexBuffer;
    StructuredBuffer<uint16_t> indexBuffer;
    glm::vec2 lastPoint{ 0.0f };
    bool hasPoint = false;

    Stroke(int vertexCap, int indexCap);

    void AddPoint(glm::vec2 pos, glm::vec2 direction, float thickness);

    const vg::Buffer &VertexBuffer() const;
    const vg::Buffer &IndexBuffer() const;
    int IndexCount() const;
};
class Strokes {
    static std::vector<Stroke *> strokes;

  public:
    static void AddStroke(Stroke *stroke);
    static void RemoveStroke(Stroke *stroke);

    static const std::vector<Stroke *> &GetStrokes();
};
