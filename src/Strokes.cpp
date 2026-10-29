#include "Strokes.h"
#include "VG/../NamedResources.h"

Stroke::Stroke(int vertexCap, int indexCap)
    : vertexBuffer(vg::BufferUsage::VertexBuffer, vertexCap), indexBuffer(vg::BufferUsage::IndexBuffer, indexCap) {
    vg::Debug::SetName(vertexBuffer.GetBuffer(), "Stroke VertexBuffer");
    vg::Debug::SetName(indexBuffer.GetBuffer(), "Stroke IndexBuffer");
}

void Stroke::AddPoint(glm::vec2 pos, glm::vec2 direction, float thickness) {
    if (hasPoint && glm::dot(pos - lastPoint, pos - lastPoint) <= 0.01f)
        return;

    if (glm::dot(direction, direction) <= 0.000001f)
        direction = hasPoint ? pos - lastPoint : glm::vec2(1.0f, 0.0f);

    glm::vec2 normal = glm::normalize(glm::vec2(direction.y, -direction.x));
    glm::vec2 p1 = pos + normal * thickness;
    glm::vec2 p2 = pos - normal * thickness;

    indexBuffer.Add(vertexBuffer.GetSize());
    indexBuffer.Add(vertexBuffer.GetSize() + 1);
    vertexBuffer.Add(Vertex(p1, glm::vec3(1, 1, 1)));
    vertexBuffer.Add(Vertex(p2, glm::vec3(1, 1, 1)));
    lastPoint = pos;
    hasPoint = true;
    vg::Debug::SetName(vertexBuffer.GetBuffer(), "Stroke VertexBuffer");
    vg::Debug::SetName(indexBuffer.GetBuffer(), "Stroke IndexBuffer");
}

const vg::Buffer &Stroke::VertexBuffer() const { return vertexBuffer.GetBuffer(); }
const vg::Buffer &Stroke::IndexBuffer() const { return indexBuffer.GetBuffer(); }
int Stroke::IndexCount() const { return indexBuffer.GetSize(); }

std::vector<Stroke *> Strokes::strokes;

void Strokes::AddStroke(Stroke *stroke) { strokes.push_back(stroke); }

void Strokes::RemoveStroke(Stroke *stroke) {
    for (auto i = strokes.begin(); i != strokes.end(); i++)
        if (*i == stroke) {
            strokes.erase(i);
            return;
        }
}

const std::vector<Stroke *> &Strokes::GetStrokes() { return strokes; }
