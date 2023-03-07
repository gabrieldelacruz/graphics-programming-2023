#include <ituGL/geometry/VertexFormat.h>

VertexFormat::VertexFormat() : m_size(0u)
{
}

void VertexFormat::Clear()
{
    m_attributes.clear();
    m_size = 0;
}

void VertexFormat::AddVertexAttribute(Data::Type type, int components, bool normalized, VertexAttribute::Semantic semantic)
{
    m_attributes.emplace_back(type, components, normalized, semantic);
    int attributeSize = m_attributes.back().GetSize();
    m_size += attributeSize;
}

VertexFormat::LayoutIterator VertexFormat::LayoutBegin(int vertexCount, bool interleaved)
{
    return LayoutIterator(*this, vertexCount, interleaved);
}

VertexFormat::LayoutIterator VertexFormat::LayoutEnd()
{
    return LayoutIterator(*this);
}

VertexFormat::LayoutIterator::LayoutIterator(const VertexFormat& vertexFormat, int vertexCount, bool interleaved)
    : m_vertexFormat(vertexFormat)
    , m_vertexCount(vertexCount)
    , m_index(0)
    , m_attributeLayout(vertexFormat.GetAttribute(0), 0, interleaved ? static_cast<int>(vertexFormat.GetSize()) : 0)
{
}

VertexFormat::LayoutIterator::LayoutIterator(const VertexFormat& vertexFormat)
    : m_vertexFormat(vertexFormat)
    , m_vertexCount(0)
    , m_index(vertexFormat.GetAttributeCount())
    , m_attributeLayout(vertexFormat.GetAttribute(0), 0, 0)
{
}

VertexFormat::LayoutIterator& VertexFormat::LayoutIterator::operator++ (int)
{
    m_index++;
    int stride = m_attributeLayout.GetStride();
    int offset = m_attributeLayout.GetOffset();
    int attributeSize = m_attributeLayout.GetAttribute().GetSize();
    offset += stride != 0 ? attributeSize : m_vertexCount * attributeSize;
    if (m_index < m_vertexFormat.GetAttributeCount())
    {
        m_attributeLayout = VertexAttribute::Layout(m_vertexFormat.GetAttribute(m_index), offset, stride);
    }
    return *this;
}

bool operator == (const VertexFormat::LayoutIterator& a, const VertexFormat::LayoutIterator& b)
{
    return (&a.m_vertexFormat == &b.m_vertexFormat) && (a.m_index == b.m_index);
}

bool operator != (const VertexFormat::LayoutIterator& a, const VertexFormat::LayoutIterator& b)
{
    return !(a == b);
}
