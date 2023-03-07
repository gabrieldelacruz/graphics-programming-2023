#pragma once

#include <ituGL/geometry/VertexBufferObject.h>
#include <ituGL/geometry/ElementBufferObject.h>
#include <ituGL/geometry/VertexArrayObject.h>
#include <ituGL/geometry/VertexAttribute.h>
#include <ituGL/geometry/Drawcall.h>
#include <ituGL/shader/ShaderProgram.h>
#include <vector>
#include <unordered_map>

// Class that groups several VBO, EBO and VAO that are part of the same object
// Can contain several drawcalls using the data in those objects
class Mesh
{
public:
    // Maps vertex attribute semantics with their location on a shader program
    using SemanticMap = std::unordered_map<VertexAttribute::Semantic, ShaderProgram::Location>;

public:
    Mesh();

    // Adds a new VBO with uninitialized data
    unsigned int AddVertexData(size_t size);

    // Adds a new VBO and initializes it with data
    template<typename T>
    unsigned int AddVertexData(std::span<const T> vertices);

    // Adds a new VBO and initializes it with data
    template<typename T>
    unsigned int AddElementData(std::span<const T> elements);

    // Adds a new VAO. It is your responsability to set the attribute pointers and the EBO, if needed
    unsigned int AddVertexArray();

    // (C++) 7
    // Adds a new VAO, with data stored in a single VBO inside the mesh, and an iterator for the attributes
    // vboIndex is the index inside m_vbos of the VBO to be used
    template<typename TIterator>
    unsigned int AddVertexArray(unsigned int vboIndex, TIterator& it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    // (C++) 7
    // Adds a new VAO, with data stored in several VBOs inside the mesh, and an iterator for the attributes
    // vboIndices are the indices inside m_vbos of the VBOs to be used
    template<typename TIterator>
    unsigned int AddVertexArray(std::span<unsigned int> vboIndices, TIterator& it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    // Adds a new submesh, with the index of the VAO to be bound, and the Drawcall parameters
    unsigned int AddSubmesh(unsigned int vaoIndex, const Drawcall& drawcall);

    // Adds a new submesh, with the index of the VAO to be bound, and the parameters to create a Drawcall
    unsigned int AddSubmesh(unsigned int vaoIndex, Drawcall::Primitive primitive, GLint first, GLsizei count, Data::Type eboType);

    // (C++) 7
    // Adds a new submesh, adding a new VAO that uses a single VBO, no EBO, and providing the parameters to create a Drawcall
    // vboIndex is the index inside m_vbos of the VBO to be used
    template<typename TIterator>
    unsigned int AddSubmesh(Drawcall::Primitive primitive, int firstVertex, int vertexCount,
        unsigned int vboIndex,
        TIterator it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    // (C++) 7
    // Adds a new submesh, adding a new VAO that uses several VBOs, no EBO, and providing the parameters to create a Drawcall
    // vboIndices are the indices inside m_vbos of the VBOs to be used
    template<typename TIterator>
    unsigned int AddSubmesh(Drawcall::Primitive primitive, int firstVertex, int vertexCount,
        std::span<unsigned int> vboIndices,
        TIterator it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());
 
    // (C++) 7
    // Adds a new submesh, adding a new VAO that uses a single VBO and an EBO,and providing the parameters to create a Drawcall
    // vboIndex is the index inside m_vbos of the VBO to be used
    // eboIndex is the index inside m_ebos of the EBO to be used
    template<typename TIterator>
    unsigned int AddSubmesh(Drawcall::Primitive primitive, int firstElement, int elementCount, Data::Type elementType,
        unsigned int vboIndex, unsigned int eboIndex,
        TIterator it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    // (C++) 7
    // Adds a new submesh, adding a new VAO that uses several VBOs and an EBO, and providing the parameters to create a Drawcall
    // vboIndices are the indices inside m_vbos of the VBOs to be used
    // eboIndex is the index inside m_ebos of the EBO to be used
    template<typename TIterator>
    unsigned int AddSubmesh(Drawcall::Primitive primitive, int firstElement, int elementCount, Data::Type elementType,
        std::span<unsigned int> vboIndices, unsigned int eboIndex,
        TIterator it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    // (C++) 7
    // Adds a new submesh, adding a new VAO and a new VBO initialized with the vertex data
    template<typename TVertex, typename TIterator>
    unsigned int AddSubmesh(Drawcall::Primitive primitive,
        std::span<const TVertex> vertices,
        TIterator it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    // (C++) 7
    // Adds a new submesh, adding a new VAO, a new VBO initialized with the vertex data and an EBO initialized with the element data
    template<typename TVertex, typename TElement, typename TIterator>
    unsigned int AddSubmesh(Drawcall::Primitive primitive,
        std::span<const TVertex> vertices, std::span<const TElement> elements,
        TIterator it, const TIterator itEnd, const SemanticMap& locations = SemanticMap());

    inline unsigned int GetVertexBufferCount() const { return static_cast<unsigned int>(m_vbos.size()); }
    inline const VertexBufferObject& GetVertexBuffer(unsigned int vboIndex) const { return m_vbos[vboIndex]; }

    inline unsigned int GetElementBufferCount() const { return static_cast<unsigned int>(m_ebos.size()); }
    inline const ElementBufferObject& GetElementBuffer(unsigned int eboIndex) const { return m_ebos[eboIndex]; }

    inline unsigned int GetVertexArrayCount() const { return static_cast<unsigned int>(m_vaos.size()); }
    inline const VertexArrayObject& GetVertexArray(unsigned int vaoIndex) const { return m_vaos[vaoIndex]; }

    inline unsigned int GetSubmeshCount() const { return static_cast<unsigned int>(m_submeshes.size()); }
    inline const VertexArrayObject& GetSubmeshVertexArray(unsigned int submeshIndex) const { return m_vaos[m_submeshes[submeshIndex].vaoIndex]; }
    inline const Drawcall& GetSubmeshDrawcall(unsigned int submeshIndex) const { return m_submeshes[submeshIndex].drawcall; }

    // Draws a submesh
    void DrawSubmesh(int submeshIndex) const;

private:

    // Helper structure that contains a drawcall and its VAO to be bound
    struct Submesh
    {
        unsigned int vaoIndex;
        Drawcall drawcall;
    };

private:

    inline VertexBufferObject& GetVertexBuffer(unsigned int vboIndex) { return m_vbos[vboIndex]; }

    inline ElementBufferObject& GetElementBuffer(unsigned int eboIndex) { return m_ebos[eboIndex]; }

    inline VertexArrayObject& GetVertexArray(unsigned int vaoIndex) { return m_vaos[vaoIndex]; }

    inline const Submesh& GetSubmesh(unsigned int submeshIndex) const { return m_submeshes[submeshIndex]; }
    inline Submesh& GetSubmesh(unsigned int submeshIndex) { return m_submeshes[submeshIndex]; }

    // Set a vertex attribute in a VAO, using the specified layout, and increases the location index according to the size of the attribute
    void SetupVertexAttribute(VertexArrayObject& vao, const VertexAttribute::Layout& attributeLayout, GLuint& location, const SemanticMap& locations);

private:
    // All the VBOs used in this mesh
    std::vector<VertexBufferObject> m_vbos;

    // All the EBOs used in this mesh
    std::vector<ElementBufferObject> m_ebos;

    // All the VAOs used in this mesh
    std::vector<VertexArrayObject> m_vaos;

    // Submeshes contained in this mesh
    std::vector<Submesh> m_submeshes;
};

template<typename T>
unsigned int Mesh::AddVertexData(std::span<const T> vertices)
{
    unsigned int vboIndex = GetVertexBufferCount();
    VertexBufferObject& vbo = m_vbos.emplace_back();
    vbo.Bind();
    vbo.AllocateData<T>(vertices);
    vbo.Unbind();
    return vboIndex;
}

template<typename T>
unsigned int Mesh::AddElementData(std::span<const T> elements)
{
    unsigned int index = GetElementBufferCount();
    ElementBufferObject& ebo = m_ebos.emplace_back();
    ebo.Bind();
    ebo.AllocateData(elements);
    ebo.Unbind();
    return index;
}

template<typename TIterator>
unsigned int Mesh::AddVertexArray(unsigned int vboIndex, TIterator& it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vaoIndex = AddVertexArray();

    VertexArrayObject& vao = GetVertexArray(vaoIndex);
    vao.Bind();

    GLuint location = 0;
    const VertexBufferObject& vbo = GetVertexBuffer(vboIndex);
    vbo.Bind();
    while (it != itEnd)
    {
        SetupVertexAttribute(vao, *it, location, locations);
        it++;
    }

    VertexBufferObject::Unbind();
    VertexArrayObject::Unbind();

    return vaoIndex;
}

template<typename TIterator>
unsigned int Mesh::AddVertexArray(std::span<unsigned int> vboIndices, TIterator& it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vaoIndex = AddVertexArray();

    const VertexArrayObject& vao = GetVertexArray(vaoIndex);
    vao.Bind();

    GLuint location = 0;
    int i = 0;
    int vboIndex = -1;
    while (it != itEnd)
    {
        if (i < vboIndices.size() && vboIndex != vboIndices[i])
        {
            vboIndex = vboIndices[i];
            VertexBufferObject& vbo = m_vbos[vboIndex];
            vbo.Bind();
            i++;
        }
        SetupVertexAttribute(vao, *it, location, locations);
        it++;
    }

    VertexBufferObject::Unbind();
    VertexArrayObject::Unbind();

    return vaoIndex;
}

template<typename TIterator>
unsigned int Mesh::AddSubmesh(Drawcall::Primitive primitive, int firstVertex, int vertexCount,
    unsigned int vboIndex,
    TIterator it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vaoIndex = AddVertexArray(vboIndex, it, itEnd, locations);
    return AddSubmesh(vaoIndex, primitive, firstVertex, vertexCount, Data::Type::None);
}

template<typename TIterator>
unsigned int Mesh::AddSubmesh(Drawcall::Primitive primitive, int firstVertex, int vertexCount,
    std::span<unsigned int> vboIndices,
    TIterator it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vaoIndex = AddVertexArray(vboIndices, it, itEnd, locations);
    return AddSubmesh(vaoIndex, primitive, firstVertex, vertexCount, Data::Type::None);
}

template<typename TIterator>
unsigned int Mesh::AddSubmesh(Drawcall::Primitive primitive, int firstElement, int elementCount, Data::Type elementType,
    unsigned int vboIndex, unsigned int eboIndex,
    TIterator it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vaoIndex = AddVertexArray(vboIndex, it, itEnd, locations);

    VertexArrayObject& vao = GetVertexArray(vaoIndex);
    vao.Bind();

    const ElementBufferObject& ebo = GetElementBuffer(eboIndex);
    ebo.Bind();

    VertexArrayObject::Unbind();
    ElementBufferObject::Unbind();

    return AddSubmesh(vaoIndex, primitive, firstElement, elementCount, elementType);
}

template<typename TIterator>
unsigned int Mesh::AddSubmesh(Drawcall::Primitive primitive, int firstElement, int elementCount, Data::Type elementType,
    std::span<unsigned int> vboIndices, unsigned int eboIndex,
    TIterator it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vaoIndex = AddVertexArray(vboIndices, it, itEnd, locations);

    VertexArrayObject& vao = m_vaos[vaoIndex];
    vao.Bind();

    ElementBufferObject& ebo = m_ebos[eboIndex];
    ebo.Bind();

    VertexArrayObject::Unbind();
    ElementBufferObject::Unbind();

    return AddSubmesh(vaoIndex, primitive, firstElement, elementCount, elementType);
}

template<typename TVertex, typename TIterator>
unsigned int Mesh::AddSubmesh(Drawcall::Primitive primitive,
    std::span<const TVertex> vertices,
    TIterator it, const TIterator itEnd, const SemanticMap& locations)
{
    unsigned int vboIndex = AddVertexData(vertices);
    return AddSubmesh(primitive, 0, static_cast<int>(vertices.size()), vboIndex, it, itEnd, locations);
}

template<typename TVertex, typename TElement, typename TIterator>
unsigned int Mesh::AddSubmesh(Drawcall::Primitive primitive,
    std::span<const TVertex> vertices, std::span<const TElement> elements,
    TIterator it, const TIterator itEnd, const SemanticMap& locations)
{
    int vboIndex = AddVertexData(vertices);
    int eboIndex = AddElementData(elements);
    return AddSubmesh(primitive, 0, static_cast<int>(elements.size()), Data::GetType<TElement>(), vboIndex, eboIndex, it, itEnd, locations);
}

