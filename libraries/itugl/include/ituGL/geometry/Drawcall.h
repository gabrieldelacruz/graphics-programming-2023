#pragma once

#include <ituGL/core/Data.h>

// Helper class to store the parameters of a drawcall
class Drawcall
{
public:
    enum class Primitive : GLenum
    {
        Invalid = GL_NONE,
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        LineLoop = GL_LINE_LOOP,
        LinesAdjacency = GL_LINES_ADJACENCY,
        LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN,
        TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
        TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        Patches = GL_PATCHES
    };

public:
    Drawcall();
    Drawcall(Primitive primitive, GLsizei count, GLint first = 0);
    Drawcall(Primitive primitive, GLsizei count, Data::Type eboType, GLint first = 0);

    // Check if the drawcall is valid
    inline bool IsValid() const { return m_primitive != Primitive::Invalid && m_count > 0; }

    // Execute the drawcall
    void Draw() const;

private:
    // Type of primitive to be rendered
    Primitive m_primitive;

    // Position of the first vertex or element that we want to render
    GLint m_first;

    // Number of vertices or elements that we want to render
    GLsizei m_count;

    // Data type of the elements in the EBO (int, uint, short, byte, etc.). A value of None means no EBO
    Data::Type m_eboType;
};
