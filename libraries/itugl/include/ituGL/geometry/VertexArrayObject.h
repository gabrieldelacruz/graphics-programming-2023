#pragma once

#include <ituGL/core/Object.h>

class VertexAttribute;

// Vertex Array Object (VAO) is an OpenGL Object that stores all of the state needed to supply vertex data
// Data is provided as a set of VertexAttributes
class VertexArrayObject : public Object
{
public:
    VertexArrayObject();
    virtual ~VertexArrayObject();

    // (C++) 8
    // Move semantics
    VertexArrayObject(VertexArrayObject&& vao) noexcept;
    VertexArrayObject& operator = (VertexArrayObject&& vao) noexcept;

    // Implements the Bind required by Object
    void Bind() const override;
    // Unbinds currently bound VertexArrayObject
    static void Unbind();

    // Sets what VertexAttribute is assigned to location, and how to access the data:
    // offset: where to start looking in the buffer
    // stride: how far each element is from the previous one. Default value 0 will use the attribute size
    void SetAttribute(GLuint location, const VertexAttribute& attribute, GLint offset, GLsizei stride = 0);

#ifndef NDEBUG
    // Check if there is any VertexArrayObject currently bound
    inline static bool IsAnyBound() { return s_boundHandle != Object::NullHandle; }
#endif

protected:

#ifndef NDEBUG
    // Check if this VertexArrayObject is currently bound
    inline bool IsBound() const override { return s_boundHandle == GetHandle(); }

    // Handle of the VertexArrayObject that is currently bound
    static Handle s_boundHandle;
#endif
};
