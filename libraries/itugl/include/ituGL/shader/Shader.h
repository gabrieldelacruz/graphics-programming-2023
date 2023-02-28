#pragma once

#include <ituGL/core/Object.h>

#include <span>

// Shader is an OpenGL Object that represents a program that runs on the GPU
// There are different types, with different requirements. See Lecture 2: Shaders for more information
class Shader : public Object
{
public:
    // Type of the shader
    enum Type : GLenum
    {
        ComputeShader = GL_COMPUTE_SHADER,
        VertexShader = GL_VERTEX_SHADER,
        TesselationControlShader = GL_TESS_CONTROL_SHADER,
        TesselationEvaluationShader = GL_TESS_EVALUATION_SHADER,
        GeometryShader = GL_GEOMETRY_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER
    };

public:
    Shader(Type type);
    virtual ~Shader();

    // (C++) 8
    // Move semantics
    Shader(Shader&& shader) noexcept;
    Shader& operator = (Shader&& shader) noexcept;

    // Implements the Bind required by Object. Shaders and shader programs don't use Bind()
    void Bind() const override;

    // Get shader type of this shader
    Type GetType() const;

    // Test if this shader is of specific type
    inline bool IsType(Type type) const { return GetType() == type; }

    // Set the source code of the shader (single source)
    inline void SetSource(const char* source) { SetSource(std::span(&source, 1)); }

    // Set the source code of the shader (multiple sources)
    void SetSource(std::span<const char*> source);

    // Compile the shader source code
    bool Compile();

    // Check if the shader has been successfully compiled
    bool IsCompiled() const;

    // Get compilation error messages in case of a failure
    void GetCompilationErrors(std::span<char> errors) const;
};
