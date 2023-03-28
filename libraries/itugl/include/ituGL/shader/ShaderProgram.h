#pragma once

#include <ituGL/core/Object.h>

// Include the glm types for vectors and matrices
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>

#include <span>

class Shader;
class TextureObject;

// ShaderProgram is an OpenGL Object that represents all the shaders needed to draw primitives
class ShaderProgram : public Object
{
public:
    // (C++) 2
    // Declare the type used for uniform locations
    using Location = GLint;

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    // (C++) 8
    // Move semantics
    ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
    ShaderProgram& operator = (ShaderProgram&& shaderProgram) noexcept;

    // Implements the Bind required by Object. Shaders and shader programs don't use Bind()
    void Bind() const override;

    // Build (Attach and link) a shader program with a compute shader
    bool Build(const Shader& computeShader);

    // Build (Attach and link) a shader program with vertex and fragment shaders
    inline bool Build(const Shader& vertexShader, const Shader& fragmentShader)
    {
        return Build(vertexShader, fragmentShader, nullptr, nullptr, nullptr);
    }

    // Build (Attach and link) a shader program with vertex and fragment shaders + geometry shader
    inline bool Build(const Shader& vertexShader, const Shader& fragmentShader, const Shader& geometryShader)
    {
        return Build(vertexShader, fragmentShader, nullptr, nullptr, &geometryShader);
    }

    // Build (Attach and link) a shader program with vertex and fragment shaders + tesselation shaders
    inline bool Build(const Shader& vertexShader, const Shader& fragmentShader,
        const Shader* tesselationControlShader, const Shader& tesselationEvaluationShader)
    {
        return Build(vertexShader, fragmentShader, tesselationControlShader, &tesselationEvaluationShader, nullptr);
    }

    // Build (Attach and link) a shader program with vertex and fragment shaders + tesselation shaders + geometry shader
    inline bool Build(const Shader& vertexShader, const Shader& fragmentShader,
        const Shader* tesselationControlShader, const Shader& tesselationEvaluationShader,
        const Shader& geometryShader)
    {
        return Build(vertexShader, fragmentShader, tesselationControlShader, &tesselationEvaluationShader, &geometryShader);
    }

    // Check if shaders have been linked to create a valid program
    bool IsLinked() const;

    // Get a string with linking error messages
    // The max length of the string returned is determined by the capacity of the span
    void GetLinkingErrors(std::span<char> errors) const;

    // Find an attribute location by name
    Location GetAttributeLocation(const char* name) const;

    // Find a uniform location by name
    Location GetUniformLocation(const char *name) const;

    // Get how many uniforms exist in this shader program
    unsigned int GetUniformCount() const;

    // Get information about a specific uniform
    void GetUniformInfo(unsigned int index, int& size, GLenum& glType, std::span<char> uniformName) const;

    // Template method combinations to simplify getting uniforms
    template<typename T>
    void GetUniform(Location location, T& value) const;
    template<typename T, int N>
    void GetUniform(Location location, glm::vec<N, T>& value) const;
    template<typename T, int C, int R>
    void GetUniform(Location location, glm::mat<C, R, T>& value) const;
    template<typename T>
    void GetUniforms(Location location, std::span<T> values) const;

    // Template method combinations to simplify setting uniforms
    template<typename T>
    void SetUniform(Location location, const T& value) const;
    template<typename T>
    void SetUniforms(Location location, std::span<const T> values) const;
    template<typename T, int N>
    void SetUniforms(Location location, std::span<const glm::vec<N, T>> values) const;
    template<typename T, int C, int R>
    void SetUniforms(Location location, std::span<const glm::mat<C, R, T>> values) const;

    // Set texture value for a texture uniform
    void SetTexture(Location location, GLint textureUnit, const TextureObject& texture) const;

    // Set the shader program as the active one to be used for rendering
    void Use() const;

private:
    // Build (Attach and link) all shaders provided for the rasterization pipeline
    bool Build(const Shader& vertexShader, const Shader& fragmentShader,
        const Shader* tesselationControlShader, const Shader* tesselationEvaluationShader,
        const Shader* geometryShader);

    // Attach a shader to be linked
    void AttachShader(const Shader& shader);

    // Link currently attached shaders
    bool Link();

    // Helper template method for getting uniforms
    template<typename T>
    void GetUniform(Location location, std::span<T> value) const;

    // Helper template methods for setting uniforms
    template<typename T, int N>
    void SetUniforms(Location location, const T* values, GLsizei count) const;
    template<typename T, int C, int R>
    void SetUniforms(Location location, const T* values, GLsizei count) const;

private:
#ifndef NDEBUG
    inline bool IsUsed() const { return s_usedHandle == GetHandle(); }
    static Handle s_usedHandle;
#endif
};

// Declaration and definitions of template methods. Don't lose time looking into these unless you are really interested
// Done to simplify the mess of different get/set methods that depend on the type

template<> void ShaderProgram::GetUniform<GLint>(Location location, std::span<GLint> valueBytes) const;
template<> void ShaderProgram::GetUniform<GLuint>(Location location, std::span<GLuint> valueBytes) const;
template<> void ShaderProgram::GetUniform<GLfloat>(Location location, std::span<GLfloat> valueBytes) const;
template<> void ShaderProgram::GetUniform<GLdouble>(Location location, std::span<GLdouble> valueBytes) const;

template<typename T>
void ShaderProgram::GetUniform(Location location, T& value) const
{
    GetUniform(location, std::span(value, 1));
}

template<typename T, int N>
void ShaderProgram::GetUniform(Location location, glm::vec<N, T>& value) const
{
    GetUniform(location, std::span(value[0], N));
}

template<typename T, int C, int R>
void ShaderProgram::GetUniform(Location location, glm::mat<C, R, T>& value) const
{
    for (int i = 0; i < R; ++i)
    {
        GetUniform(location, std::span(value[i], C));
    }
}

template<typename T>
void ShaderProgram::GetUniforms(ShaderProgram::Location location, std::span<T> values) const
{
    for (auto& value : values)
        GetUniform(location++, value);
}

template<typename T>
void ShaderProgram::SetUniform(Location location, const T& value) const
{
    SetUniforms(location, std::span(&value, 1));
}

template<typename T>
void ShaderProgram::SetUniforms(Location location, std::span<const T> values) const
{
    SetUniforms<T, 1>(location, &values[0], static_cast<GLsizei>(values.size()));
}

template<typename T, int N>
void ShaderProgram::SetUniforms(Location location, std::span<const glm::vec<N, T>> values) const
{
    SetUniforms<T, N>(location, &values[0][0], static_cast<GLsizei>(values.size()));
}

template<typename T, int C, int R>
void ShaderProgram::SetUniforms(Location location, std::span<const glm::mat<C, R, T>> values) const
{
    SetUniforms<T, C, R>(location, &values[0][0][0], static_cast<GLsizei>(values.size()));
}

template<> void ShaderProgram::SetUniforms<GLint, 1>(Location location, const GLint* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLint, 2>(Location location, const GLint* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLint, 3>(Location location, const GLint* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLint, 4>(Location location, const GLint* values, GLsizei count) const;

template<> void ShaderProgram::SetUniforms<GLuint, 1>(Location location, const GLuint* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLuint, 2>(Location location, const GLuint* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLuint, 3>(Location location, const GLuint* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLuint, 4>(Location location, const GLuint* values, GLsizei count) const;

template<> void ShaderProgram::SetUniforms<GLfloat, 1>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 2>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 3>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 4>(Location location, const GLfloat* values, GLsizei count) const;

template<> void ShaderProgram::SetUniforms<GLdouble, 1>(Location location, const GLdouble* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLdouble, 2>(Location location, const GLdouble* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLdouble, 3>(Location location, const GLdouble* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLdouble, 4>(Location location, const GLdouble* values, GLsizei count) const;

template<> void ShaderProgram::SetUniforms<GLfloat, 2, 2>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 2, 3>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 2, 4>(Location location, const GLfloat* values, GLsizei count) const;

template<> void ShaderProgram::SetUniforms<GLfloat, 3, 2>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 3, 3>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 3, 4>(Location location, const GLfloat* values, GLsizei count) const;

template<> void ShaderProgram::SetUniforms<GLfloat, 4, 2>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 4, 3>(Location location, const GLfloat* values, GLsizei count) const;
template<> void ShaderProgram::SetUniforms<GLfloat, 4, 4>(Location location, const GLfloat* values, GLsizei count) const;
