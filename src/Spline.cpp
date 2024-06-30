#include "Spline.h"
#include <iostream>

static const double PI = 3.14159265359;
static const double RAD = 1 / 180.0f * PI;

glm::vec2 polar(float angle, float radius)
{
    return { sin(angle * RAD) * radius, cos(angle * RAD) * radius };
}
glm::vec2 slerp(glm::vec2 p1, glm::vec2 p2, float k)
{
    float dot = glm::dot(p1, p2);
    if (abs(dot) > 0.99)
    {
        p2 = glm::normalize(p2 + glm::vec2(p2.y * 0.01, -p2.x * 0.01));
        dot = glm::dot(p1, p2);
    }

    float theta = acos(dot) * k;
    glm::vec2 relativeVec = glm::normalize(p2 - p1 * dot);
    return p1 * (float) cos(theta) + relativeVec * (float) sin(theta);
}

int GetCapacity(const vg::Buffer& buffer)
{
    return buffer.GetSize() / (sizeof(Vertex) + sizeof(int));
}

const ControlPoint ControlPoint::Reset = { {0,0},-1,{0,0,0,0} };

Spline::Spline(int initialCapacity) : m_bufferMemory(nullptr), resetCount(0), singlePointCount(0)
{
    BufferResize(initialCapacity);
    m_updateSpans.Add(0);
}

Spline::~Spline()
{
    m_vertexBuffer.UnmapMemory();
}

void Spline::AddControlPoint(const ControlPoint& point)
{
    AddControlPoint(point, m_points.size());
}

void Spline::AddControlPoint(const ControlPoint& point, int index)
{
    if (point.IsReset())
    {
        if (index == 0 || m_points[index - 1].IsReset()) return;
        if (index != m_points.size() && m_points[index].IsReset()) return;
    }
    if (index != 0 && m_points[index - 1].pos == point.pos)return;
    if (index != m_points.size() && m_points[index].pos == point.pos)return;

    m_points.insert(m_points.begin() + index, point);
    m_updateSpans.Add({ index - 2 , m_points.size() });

    if (point.IsReset())
    {
        resetCount++;
        if (index != m_points.size() - 1)
            singlePointCount += IsSingle(index - 1) + IsSingle(index + 1);
    }
    else
    {
        if (index != 0 && (index + 1 >= m_points.size() || m_points[index + 1].IsReset()) && (index < 2 || m_points[index - 2].IsReset()) && !m_points[index - 1].IsReset())
            singlePointCount--;
        if (index != m_points.size() - 1 && (index <= 0 || m_points[index - 1].IsReset()) && (index >= m_points.size() - 2 || m_points[index + 2].IsReset()) && !m_points[index + 1].IsReset())
            singlePointCount--;
        singlePointCount += IsSingle(index);
    }
}

void Spline::RemoveControlPoint()
{
    RemoveControlPoint(m_points.size() - 1);
}
void Spline::RemoveControlPoint(int index)
{
    if (m_points[index].IsReset())
    {
        resetCount--;
        singlePointCount -= IsSingle(index - 1) + IsSingle(index + 1);
    }
    else
        singlePointCount -= IsSingle(index);

    m_points.erase(m_points.begin() + index);
    singlePointCount += IsSingle(index - 1) + IsSingle(index);
    m_updateSpans.Add({ index - 2, (int) m_points.size() });

    // If removal put two resets next to one another or as first point.
    if (index < m_points.size() && m_points[index].IsReset() && (index == 0 || m_points[index - 1].IsReset()))
        RemoveControlPoint(index);
}

void Spline::ReplaceControlPoint(const ControlPoint& point, int index)
{
    if (point.IsReset() && index == 0)
        return RemoveControlPoint(index);

    if (point.IsReset() && (m_points[index - 1].IsReset() || (index != m_points.size() - 1 && m_points[index + 1].IsReset())))
        return RemoveControlPoint(index);

    bool changedPointType = point.IsReset() != m_points[index].IsReset();

    m_points[index] = point;
    if (point.IsReset())
    {
        resetCount += point.IsReset();
        singlePointCount += IsSingle(index - 1) + IsSingle(index + 1);
    }
    else
        singlePointCount += IsSingle(index);

    if (changedPointType)
        m_updateSpans.Add({ index - 2, GetControlPointSize() });
    else
        m_updateSpans.Add({ index - 2, index + 2 });
}

ControlPoint* Spline::GetRange(int start, int end)
{
    m_updateSpans.Add({ start - 1 , end });
    return m_points.data() + start;
}

const ControlPoint* Spline::GetRangeC(int start, int end) const
{
    return m_points.data() + start;
}
int Spline::GetControlPointSize()const
{
    return m_points.size();
}

Spline::operator BufferDraw()
{
    if (m_updateSpans.size() == 0)
        return BufferDraw(m_vertexBuffer, 0, 0, 0, -1);

    m_updateSpans.Remove({ INT_MIN, -1 });
    m_updateSpans.Remove({ m_points.size(), INT_MAX });

    // std::cout << m_updateSpans << "\n";
    if (m_points.size() == 0)
        return BufferDraw(m_vertexBuffer, 0, 0, 0, 0);

    // Resize vertex buffer if needed.
    int implicitReset = !(m_points.end() - 1)->IsReset();
    int capCount = (resetCount + implicitReset) * 2;
    int freePoints = m_points.size() - resetCount * 3 + singlePointCount - 2 * implicitReset;
    int requiredVertexCount = capCount * resolution + freePoints * resolution * 2 + resetCount;
    if (GetCapacity(m_vertexBuffer) < requiredVertexCount)
        BufferResize(requiredVertexCount * 2);

    Vertex* vertices = (Vertex*) m_bufferMemory;
    int* indices = (int*) (vertices + GetCapacity(m_vertexBuffer));

    // Populate the vertex buffer.
    int vertexIndex = 0;
    for (auto i = m_points.begin(); i != m_points.end(); i++)
    {
        bool isBeginCap = i == m_points.begin() || (i - 1)->IsReset();
        bool isEndCap = i + 1 == m_points.end() || (i + 1)->IsReset();

        // Count how many resets happened so far.
        int controlPointIndex = i - m_points.begin();
        if (m_updateSpans.size() == 0)
            break;

        if (!m_updateSpans.Contains(controlPointIndex))
        {
            if (i->IsReset())
            {
                vertexIndex++;
                continue;
            }

            vertexIndex += resolution * (isBeginCap + isEndCap);
            if (!isBeginCap && !isEndCap)vertexIndex += resolution * 2;

            continue;
        }
        m_updateSpans.Remove(controlPointIndex);

        if (i->IsReset())
        {
            indices[vertexIndex] = -1;
            vertexIndex++;
            continue;
        }

        // Calculate Vector to the next point that is not the end point.
        glm::vec2 toNext = { 0,1 };
        glm::vec2 toPrev = { 0,1 };
        if (!isBeginCap && i->pos != (i - 1)->pos) toPrev = glm::normalize(i->pos - (i - 1)->pos);
        if (!isEndCap && i->pos != (i + 1)->pos) toNext = glm::normalize((i + 1)->pos - i->pos);

        glm::vec2 toPrevCross = { toPrev.y,-toPrev.x };
        glm::vec2 toNextCross = { toNext.y,-toNext.x };

        // Draw begin cap.
        if (isBeginCap)
        {
            float step = 180.0f / (resolution - 1);
            bool isResEven = resolution % 2 == 0;
            if (!isResEven)
            {
                vertices[vertexIndex].position = i->pos - toNext * i->size;
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;
            }
            for (int j = 0; j < resolution / 2; j++)
            {
                glm::vec2 p = polar(step * !isResEven + step * j, i->size);

                vertices[vertexIndex].position = i->pos - (toNextCross * p.x + toNext * p.y);
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;

                vertices[vertexIndex].position = i->pos - (toNextCross * -p.x + toNext * p.y);
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;
            }
        }

        // Draw the main line.
        if (!isBeginCap && !isEndCap)
        {
            glm::vec2 a = -toNextCross;
            glm::vec2 b = toPrevCross;
            glm::vec2 a1 = -toPrevCross;
            glm::vec2 b1 = toNextCross;
            for (int j = 0; j < resolution * 2; j += 2)
            {
                float k = j / ((resolution - 1.0f) * 2.0f);

                glm::vec2 p1 = slerp(a, a1, 1 - k);
                glm::vec2 p2 = slerp(b, b1, k);

                vertices[vertexIndex].position = p1 * i->size + i->pos;
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;

                vertices[vertexIndex].position = p2 * i->size + i->pos;
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;
            }
        }

        // Draw end cap.
        if (isEndCap)
        {
            float step = 180.0f / (resolution - 1);
            for (int j = 0; j < resolution / 2; j++)
            {
                glm::vec2 p = polar(-90 + step * j, i->size);
                vertices[vertexIndex].position = i->pos + (toPrevCross * p.x + toPrev * p.y);
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;

                vertices[vertexIndex].position = i->pos + (toPrevCross * -p.x + toPrev * p.y);
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;
            }
            if (resolution % 2 == 1)
            {
                vertices[vertexIndex].position = i->pos + toPrev * i->size;
                vertices[vertexIndex].color = i->col;
                indices[vertexIndex] = vertexIndex;
                vertexIndex++;
            }
        }
    }

    return BufferDraw(m_vertexBuffer, 0, (char*) indices - (char*) vertices, 1, requiredVertexCount);
}

void Spline::BufferResize(int newCapacity)
{
    vg::Buffer newBuffer((sizeof(Vertex) + sizeof(int)) * newCapacity, { vg::BufferUsage::IndexBuffer,vg::BufferUsage::VertexBuffer });
    vg::Allocate(&newBuffer, { vg::MemoryProperty::HostVisible });
    char* newBufferMemory = (char*) newBuffer.MapMemory();
    if (m_bufferMemory != nullptr)
    {
        int oldCapacity = GetCapacity(m_vertexBuffer);
        if (newCapacity >= oldCapacity)
        {
            memcpy(newBufferMemory, m_bufferMemory, sizeof(Vertex) * oldCapacity);
            memcpy(newBufferMemory + sizeof(Vertex) * newCapacity, m_bufferMemory + sizeof(Vertex) * oldCapacity, sizeof(int) * oldCapacity);
        }
        else
        {
            memcpy(newBufferMemory, m_bufferMemory, sizeof(Vertex) * newCapacity);
            memcpy(newBufferMemory + sizeof(Vertex) * newCapacity, m_bufferMemory + sizeof(Vertex) * newCapacity, sizeof(int) * newCapacity);
        }
    }

    std::swap(m_vertexBuffer, newBuffer);
    m_bufferMemory = newBufferMemory;
}
bool Spline::IsSingle(int index) const
{
    if (index < 0 || index >= m_points.size()) return false;
    if (m_points[index].IsReset()) return false;
    if ((index == 0 || m_points[index - 1].IsReset()) && (index == m_points.size() - 1 || m_points[index + 1].IsReset())) return true;
    return false;
}