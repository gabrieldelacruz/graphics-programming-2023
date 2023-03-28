#include <ituGL/shader/ShaderProgram.h>

#include <ituGL/shader/Shader.h>
#include <ituGL/texture/TextureObject.h>
#include <cassert>

#ifndef NDEBUG
ShaderProgram::Handle ShaderProgram::s_usedHandle = ShaderProgram::NullHandle;
#endif

ShaderProgram::ShaderProgram() : Object(NullHandle)
{
    Handle& handle = GetHandle();
    handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    if (IsValid())
    {
        Handle& handle = GetHandle();
        glDeleteProgram(handle);
        handle = NullHandle;
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept : Object(std::move(shaderProgram))
{
}

ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram) noexcept
{
    Object::operator=(std::move(shaderProgram));
    return *this;
}

// Bind should not be called for ShaderProgram
void ShaderProgram::Bind() const
{
    // Assert if it gets called
    assert(false);
}

// Build (Attach and link) a shader program with a compute shader
bool ShaderProgram::Build(const Shader& computeShader)
{
    assert(computeShader.IsType(Shader::ComputeShader));
    AttachShader(computeShader);
    return Link();
}

// Build (Attach and link) all shaders provided for the rasterization pipeline
bool ShaderProgram::Build(const Shader& vertexShader, const Shader& fragmentShader,
    const Shader* tesselationControlShader, const Shader* tesselationEvaluationShader,
    const Shader* geometryShader)
{
    assert(vertexShader.IsType(Shader::VertexShader));
    AttachShader(vertexShader);

    assert(fragmentShader.IsType(Shader::FragmentShader));
    AttachShader(fragmentShader);

    if (tesselationControlShader)
    {
        assert(tesselationEvaluationShader);
        assert(tesselationControlShader->IsType(Shader::TesselationControlShader));
        AttachShader(*tesselationControlShader);
    }

    if (tesselationEvaluationShader)
    {
        assert(tesselationEvaluationShader->IsType(Shader::TesselationEvaluationShader));
        AttachShader(*tesselationEvaluationShader);
    }

    if (geometryShader)
    {
        assert(geometryShader->IsType(Shader::GeometryShader));
        AttachShader(*geometryShader);
    }

    return Link();
}

// Attach a shader to be linked
void ShaderProgram::AttachShader(const Shader& shader)
{
    assert(IsValid());
    assert(!IsLinked());
    assert(shader.IsValid());
    assert(shader.IsCompiled());
    glAttachShader(GetHandle(), shader.GetHandle());
}

// Link currently attached shaders
bool ShaderProgram::Link()
{
    assert(IsValid());
    glLinkProgram(GetHandle());
    return IsLinked();
}

// Check if shaders have been linked to create a valid program
bool ShaderProgram::IsLinked() const
{
    assert(IsValid());

    GLint success;
    glGetProgramiv(GetHandle(), GL_LINK_STATUS, &success);
    return success;
}

// Get a string with linking error messages
// The max length of the string returned is determined by the capacity of the span
void ShaderProgram::GetLinkingErrors(std::span<char> errors) const
{
    assert(IsValid());
    glGetProgramInfoLog(GetHandle(), static_cast<GLsizei>(errors.size()), nullptr, errors.data());
}

// Set the shader program as the active one to be used for rendering
void ShaderProgram::Use() const
{
    assert(IsValid());
    assert(IsLinked());
    Handle handle = GetHandle();
    glUseProgram(handle);
#ifndef NDEBUG
    s_usedHandle = handle;
#endif
}

// Find an attribute location by name
ShaderProgram::Location ShaderProgram::GetAttributeLocation(const char* name) const
{
    assert(IsValid());
    assert(IsLinked());
    return glGetAttribLocation(GetHandle(), name);
}

// Find a uniform location by name
ShaderProgram::Location ShaderProgram::GetUniformLocation(const char* name) const
{
    assert(IsValid());
    assert(IsLinked());
    return glGetUniformLocation(GetHandle(), name);
}

// Get how many uniforms exist in this shader program
unsigned int ShaderProgram::GetUniformCount() const
{
    GLint uniformCount;
    glGetProgramiv(GetHandle(), GL_ACTIVE_UNIFORMS, &uniformCount);
    return uniformCount;
}

// Get information about a specific uniform
void ShaderProgram::GetUniformInfo(unsigned int index, int& size, GLenum& glType, std::span<char> uniformName) const
{
    glGetActiveUniform(GetHandle(), index, uniformName.size(), nullptr, &size, &glType, uniformName.data());
}

// All the different combinations of Get/SetUniform
template<>
void ShaderProgram::GetUniform<GLint>(Location location, std::span<GLint> value) const
{
    assert(IsValid());
    assert(IsLinked());
    glGetnUniformiv(GetHandle(), location, static_cast<GLsizei>(value.size_bytes()), value.data());
}

template<>
void ShaderProgram::GetUniform<GLuint>(Location location, std::span<GLuint> value) const
{
    assert(IsValid());
    assert(IsLinked());
    glGetnUniformuiv(GetHandle(), location, static_cast<GLsizei>(value.size_bytes()), value.data());
}

template<>
void ShaderProgram::GetUniform<GLfloat>(Location location, std::span<GLfloat> value) const
{
    assert(IsValid());
    assert(IsLinked());
    glGetnUniformfv(GetHandle(), location, static_cast<GLsizei>(value.size_bytes()), value.data());
}

template<>
void ShaderProgram::GetUniform<GLdouble>(Location location, std::span<GLdouble> value) const
{
    assert(IsValid());
    assert(IsLinked());
    glGetnUniformdv(GetHandle(), location, static_cast<GLsizei>(value.size_bytes()), value.data());
}

template<>
void ShaderProgram::SetUniforms<GLint, 1>(Location location, const GLint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform1iv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLint, 2>(Location location, const GLint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform2iv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLint, 3>(Location location, const GLint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform3iv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLint, 4>(Location location, const GLint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform4iv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLuint, 1>(Location location, const GLuint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform1uiv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLuint, 2>(Location location, const GLuint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform2uiv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLuint, 3>(Location location, const GLuint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform3uiv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLuint, 4>(Location location, const GLuint* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform4uiv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 1>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform1fv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 2>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform2fv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 3>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform3fv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 4>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform4fv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLdouble, 1>(Location location, const GLdouble* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform1dv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLdouble, 2>(Location location, const GLdouble* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform2dv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLdouble, 3>(Location location, const GLdouble* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform3dv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLdouble, 4>(Location location, const GLdouble* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniform4dv(location, count, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 2, 2>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix2fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 2, 3>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix2x3fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 2, 4>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix2x4fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 3, 2>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix3x2fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 3, 3>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix3fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 3, 4>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix3x4fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 4, 2>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix4x2fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 4, 3>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix4x3fv(location, count, false, values);
}

template<>
void ShaderProgram::SetUniforms<GLfloat, 4, 4>(Location location, const GLfloat* values, GLsizei count) const
{
    assert(IsValid());
    assert(IsUsed());
    glUniformMatrix4fv(location, count, false, values);
}

void ShaderProgram::SetTexture(Location location, GLint textureUnit, const TextureObject& texture) const
{
    assert(IsValid());
    assert(IsUsed());
    TextureObject::SetActiveTexture(textureUnit);
    texture.Bind();
    SetUniform(location, textureUnit);
}
