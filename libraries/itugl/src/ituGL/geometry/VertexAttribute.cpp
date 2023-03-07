#include <ituGL/geometry/VertexAttribute.h>

VertexAttribute::VertexAttribute(Data::Type type, int components, Semantic semantic)
    : VertexAttribute(type, components, false, semantic)
{
}

VertexAttribute::VertexAttribute(Data::Type type, int components, bool normalized, Semantic semantic)
    : m_type(type)
    , m_components(components)
    , m_normalized(normalized)
    , m_semantic(semantic)
{
}

int VertexAttribute::GetLocationSize() const
{
    // For matrix attributes, we would need 1 for each row, but we won´t be using matrices for attributes.
    return 1;
}

VertexAttribute::Layout::Layout(const VertexAttribute& attribute, GLint offset, GLsizei stride)
    : m_attribute(attribute)
    , m_offset(offset)
    , m_stride(stride)
{
}
