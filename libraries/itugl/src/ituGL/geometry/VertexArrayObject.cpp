#include <ituGL/geometry/VertexArrayObject.h>

#include <ituGL/geometry/VertexAttribute.h>
#include <cassert>

#ifndef NDEBUG
#include <ituGL/geometry/VertexBufferObject.h>   // To assert that there is a VertexBufferObject bound

VertexArrayObject::Handle VertexArrayObject::s_boundHandle = VertexArrayObject::NullHandle;
#endif

// Create the object initially null, get object handle and generate 1 vertex array
VertexArrayObject::VertexArrayObject() : Object(NullHandle)
{
    Handle& handle = GetHandle();
    glGenVertexArrays(1, &handle);
}

// Get object handle and delete 1 vertex array
VertexArrayObject::~VertexArrayObject()
{
    Handle& handle = GetHandle();
    glDeleteVertexArrays(1, &handle);
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& vao) noexcept : Object(std::move(vao))
{
}

VertexArrayObject& VertexArrayObject::operator = (VertexArrayObject&& vao) noexcept
{
    Object::operator=(std::move(vao));
    return *this;
}

// Bind the vertex array handle to the specific target
void VertexArrayObject::Bind() const
{
    Handle handle = GetHandle();
    glBindVertexArray(handle);
#ifndef NDEBUG
    s_boundHandle = handle;
#endif
}

// Bind the null handle to the specific target
void VertexArrayObject::Unbind()
{
    Handle handle = NullHandle;
    glBindVertexArray(handle);
#ifndef NDEBUG
    s_boundHandle = handle;
#endif
}

// Sets the VertexAttribute pointer and enables the VertexAttribute in that location
void VertexArrayObject::SetAttribute(GLuint location, const VertexAttribute& attribute, GLint offset, GLsizei stride)
{
    assert(IsBound());
    assert(VertexBufferObject::IsAnyBound());

    // Get the attribute properties in OpenGL expected format
    GLint components = attribute.GetComponents();
    GLenum type = static_cast<GLenum>(attribute.GetType());
    GLboolean normalized = attribute.IsNormalized() ? GL_TRUE : GL_FALSE;

    // Compute the attribute pointer
    const unsigned char* pointer = nullptr; // Actual base pointer is in VBO
    pointer += offset;

    // Set the VertexAttribute pointer in this location
    glVertexAttribPointer(location, components, type, normalized, stride, pointer);

    // Finally, we enable the VertexAttribute in this location
    glEnableVertexAttribArray(location);
}
