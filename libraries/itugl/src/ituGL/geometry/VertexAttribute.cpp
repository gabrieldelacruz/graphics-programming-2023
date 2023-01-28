#include <ituGL/geometry/VertexAttribute.h>

VertexAttribute::VertexAttribute(Data::Type type, int components, bool normalized)
    : m_type(type)
    , m_components(components)
    , m_normalized(normalized)
{
}
