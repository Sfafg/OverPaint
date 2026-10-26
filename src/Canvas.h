#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "VG/VG.h"

struct Canvas {
    vg::Buffer strokes;

    Canvas();

    void Stroke(glm::vec2 p1, glm::vec2 p2, float pressure);
};
