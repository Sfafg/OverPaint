#pragma once
#include <VG/vg.h>
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"
#include "BufferDraw.h"
#include "SpanSet.h"

struct ControlPoint
{
    glm::vec2 pos;
    float size;
    glm::vec<4, char> col;

    ControlPoint(glm::vec2 pos, float size, glm::vec<4, char> col) :pos(pos), size(size), col(col) {}
    bool IsReset() const { return size == Reset.size; }

    const static ControlPoint Reset;
};

class Spline
{
public:
    Spline(int initialCapacity);
    ~Spline();

    void AddControlPoint(const ControlPoint& point);
    void AddControlPoint(const ControlPoint& point, int index);
    void RemoveControlPoint();
    void RemoveControlPoint(int index);
    void ReplaceControlPoint(const ControlPoint& point, int index);
    ControlPoint* GetRange(int start, int end);
    const ControlPoint* GetRangeC(int start, int end) const;
    int GetControlPointSize() const;

    operator BufferDraw();

private:
    void BufferResize(int newCapacity);
    bool IsSingle(int index) const;
private:
    const int resolution = 15;
    std::vector<ControlPoint> m_points;
    int resetCount;
    int singlePointCount;
    vg::Buffer m_vertexBuffer;
    char* m_bufferMemory;
    SpanSet<int> m_updateSpans;
};