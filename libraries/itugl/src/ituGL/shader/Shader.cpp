#include <ituGL/shader/Shader.h>

#include <cassert>

Shader::Shader(Type type) : Object(NullHandle)
{
    Handle& handle = GetHandle();
    handle = glCreateShader(type);
}

Shader::~Shader()
{
    if (IsValid())
    {
        Handle& handle = GetHandle();
        glDeleteShader(handle);
        handle = NullHandle;
    }
}

// Bind should not be called for Shader
void Shader::Bind() const
{
    // Assert if it gets called
    assert(false);
}

// Get shader type of this shader
Shader::Type Shader::GetType() const
{
    assert(IsValid());

    GLint type;
    glGetShaderiv(GetHandle(), GL_SHADER_TYPE, &type);
    return static_cast<Type>(type);
}

// Set the source code of the shader
void Shader::SetSource(std::span<const char*> source)
{
    assert(IsValid());

    GLsizei count = static_cast<GLsizei>(source.size());
    const char** sources = source.data();

    // (todo) 02.1: Set the shader source code

}

// Compile the shader source code
bool Shader::Compile()
{
    assert(IsValid());

    Handle handle = GetHandle();

    // (todo) 02.1: Compile the shader


    return IsCompiled();
}

// Check if the shader has been successfully compiled
bool Shader::IsCompiled() const
{
    assert(IsValid());

    GLint success;
    glGetShaderiv(GetHandle(), GL_COMPILE_STATUS, &success);
    return success;
}

// Get compilation error messages in case of a failure
void Shader::GetCompilationErrors(std::span<char> errors) const
{
    assert(IsValid());

    glGetShaderInfoLog(GetHandle(), static_cast<int>(errors.size()), nullptr, errors.data());
}
