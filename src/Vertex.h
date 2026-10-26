#pragma once
#include <glm/glm.hpp>
#include "VG/VG.h"

struct Vertex {
    glm::vec2 position;
    glm::vec3 color;

    static vg::VertexBinding &GetBindingDescription();
    static std::vector<vg::VertexAttribute> &GetAttributeDescriptions();
};
