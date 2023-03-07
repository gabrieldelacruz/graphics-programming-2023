#pragma once

#include <ituGL/geometry/VertexAttribute.h>
#include <vector>
#include <cassert>

// Helper class that defines a set of attributes used by the vertices
class VertexFormat
{
public:
    class LayoutIterator;

public:
    VertexFormat();

    // Gets the size in bytes of all the attributes together
    inline size_t GetSize() const { return m_size; }

    // Gets the number of attributes
    inline int GetAttributeCount() const { return static_cast<int>(m_attributes.size()); }
    // Gets a specific attribute
    inline VertexAttribute GetAttribute(int index) const { return m_attributes[index]; }

    // Removes all the attributes
    void Clear();

    // Adds a new attribute (for integer types)
    template<typename T>
    void AddVertexAttribute(int components, bool normalized, VertexAttribute::Semantic semantic = VertexAttribute::Semantic::Unknown);

    // Adds a new attribute (for floating point types)
    template<typename T>
    void AddVertexAttribute(int components, VertexAttribute::Semantic semantic = VertexAttribute::Semantic::Unknown);

    // Adds a new attribute
    void AddVertexAttribute(Data::Type type, int components, bool normalized, VertexAttribute::Semantic semantic);

    // Iterator at the first attribute, can be interleaved or contiguous
    LayoutIterator LayoutBegin(int vertexCount, bool interleaved);

    // Iterator at the end of all attributes
    LayoutIterator LayoutEnd();

private:
    std::vector<VertexAttribute> m_attributes;
    size_t m_size;
};

// Iterator for VertexAttribute::Layout, for contiguous or interleaved data inside a single VBO
class VertexFormat::LayoutIterator
{
public:
    LayoutIterator(const VertexFormat& vertexFormat);
    LayoutIterator(const VertexFormat& vertexFormat, int vertexCount, bool interleaved);

    inline const VertexAttribute::Layout& operator * () const { return m_attributeLayout; }
    inline const VertexAttribute::Layout* operator -> () const { return &m_attributeLayout; }

    LayoutIterator& operator++ (int);

    friend bool operator == (const LayoutIterator& a, const LayoutIterator& b);
    friend bool operator != (const LayoutIterator& a, const LayoutIterator& b);

protected:
    // Reference to the vertex format that created it
    const VertexFormat& m_vertexFormat;
    // Number of vertices in this data set
    int m_vertexCount;
    // Index of the current attribute
    int m_index;
    // Layout of the current attribute
    VertexAttribute::Layout m_attributeLayout;
};

template<typename T>
void VertexFormat::AddVertexAttribute(int components, bool normalized, VertexAttribute::Semantic semantic)
{
    Data::Type type = Data::GetType<T>();
    assert(type != Data::Type::Float && type != Data::Type::Double && type != Data::Type::Half);
    AddVertexAttribute(type, components, normalized, semantic);
}

template<typename T>
void VertexFormat::AddVertexAttribute(int components, VertexAttribute::Semantic semantic)
{
    Data::Type type = Data::GetType<T>();
    assert(type == Data::Type::Float || type == Data::Type::Double || type == Data::Type::Half);
    AddVertexAttribute(type, components, false, semantic);
}
