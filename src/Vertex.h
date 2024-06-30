#pragma once
#include "VG/vg.h"
#include <array>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec2 position;
    glm::vec<4, char> color;

    static vg::VertexBinding& getBindingDescription()
    {
        static vg::VertexBinding bindingDescription(0, sizeof(Vertex));

        return bindingDescription;
    }
    static std::array<vg::VertexAttribute, 2>& getAttributeDescriptions()
    {
        static std::array<vg::VertexAttribute, 2> attributeDescriptions = {
            vg::VertexAttribute(0,0,vg::Format::RG32SFLOAT,offsetof(Vertex,position)),
            vg::VertexAttribute(1,0,vg::Format::RGBA8UNORM,offsetof(Vertex,color))
        };

        return attributeDescriptions;
    }
};