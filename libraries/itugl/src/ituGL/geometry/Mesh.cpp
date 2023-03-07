#include <ituGL/geometry/Mesh.h>

Mesh::Mesh()
{
}

unsigned int Mesh::AddVertexData(size_t size)
{
    unsigned int vboIndex = GetVertexBufferCount();
    VertexBufferObject& vbo = m_vbos.emplace_back();
    vbo.Bind();
    vbo.AllocateData(size);
    return vboIndex;
}

unsigned int Mesh::AddVertexArray()
{
    unsigned int vaoIndex = GetVertexArrayCount();
    m_vaos.emplace_back();
    return vaoIndex;
}

unsigned int Mesh::AddSubmesh(unsigned int vaoIndex, const Drawcall& drawcall)
{
    unsigned int submeshIndex = GetSubmeshCount();
    Submesh& submesh = m_submeshes.emplace_back();
    submesh.vaoIndex = vaoIndex;
    submesh.drawcall = drawcall;
    return submeshIndex;
}

unsigned int Mesh::AddSubmesh(unsigned int vaoIndex,
    Drawcall::Primitive primitive, GLint first, GLsizei count, Data::Type eboType)
{
    return AddSubmesh(vaoIndex, Drawcall(primitive, count, eboType, first));
}

// Bind the VAO and render the drawcall of the submesh
void Mesh::DrawSubmesh(int submeshIndex) const
{
    const Submesh& submesh = GetSubmesh(submeshIndex);
    const VertexArrayObject& vao = GetVertexArray(submesh.vaoIndex);
    vao.Bind();
    submesh.drawcall.Draw();
    //VertexArrayObject::Unbind(); // No need to unbind
}

void Mesh::SetupVertexAttribute(VertexArrayObject& vao, const VertexAttribute::Layout& attributeLayout, GLuint& location, const SemanticMap& locations)
{
    const VertexAttribute& attribute = attributeLayout.GetAttribute();

    auto itLocation = locations.find(attribute.GetSemantic());
    if (itLocation != locations.end())
    {
        location = itLocation->second;
    }

    vao.SetAttribute(location, attribute, attributeLayout.GetOffset(), attributeLayout.GetStride());
    location += attribute.GetLocationSize();
}
