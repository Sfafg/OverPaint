#include "Vertex.h"

using namespace vg;
using namespace vg::cmd;

VertexBinding &Vertex::GetBindingDescription() {
    static VertexBinding bindingDescription(0, sizeof(Vertex));

    return bindingDescription;
}

std::vector<VertexAttribute> &Vertex::GetAttributeDescriptions() {
    static std::vector<VertexAttribute> attributeDescriptions = {
        VertexAttribute(0, 0, Format::RG32SFLOAT, offsetof(Vertex, position)),
        VertexAttribute(1, 0, Format::RGB32SFLOAT, offsetof(Vertex, color)),
    };

    return attributeDescriptions;
}
