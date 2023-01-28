#pragma once

#include <ituGL/core/Object.h>

class VertexAttribute;

// Vertex Array Object (VAO) is an OpenGL Object that stores all of the state needed to supply vertex dataç
// Data is provided as a set of VertexAttributes
class VertexArrayObject : public Object
{
public:
    VertexArrayObject();
    virtual ~VertexArrayObject();

    // Implements the Bind required by Object
    void Bind() const override;
    // Unbinds currently bound VertexArrayObject
    static void Unbind();

    // Sets what VertexAttribute is assigned to location, and how to access the data:
    // offset: where to start looking in the buffer
    // stride: how far each element is from the previous one. Default value 0 will use the attribute size
    void SetAttribute(GLuint location, const VertexAttribute& attribute, GLint offset, GLsizei stride = 0);
};
