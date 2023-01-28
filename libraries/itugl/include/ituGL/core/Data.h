#pragma once

#include <glad/glad.h>
#include <span>

class Data
{
public:
    // Enum type to encapsulate the different types supported by OpenGL
    enum class Type : GLushort
    {
        None = GL_NONE,
        Float = GL_FLOAT,
        Fixed = GL_FIXED,
        Half = GL_HALF_FLOAT,
        Double = GL_DOUBLE,
        Byte = GL_BYTE,
        UByte = GL_UNSIGNED_BYTE,
        Short = GL_SHORT,
        UShort = GL_UNSIGNED_SHORT,
        Int = GL_INT,
        UInt = GL_UNSIGNED_INT,
        // And more...
    };

public:
    // (C++ 3)
    // Data class is static, so we delete the constructor
    Data() = delete;

    // Templated method to convert from types to the enum Type
    template<typename T>
    static Type GetType();
    template<typename T>
    static Type GetType(const T&);

    // Get size in bytes for each Type
    static unsigned int GetTypeSize(Type type);

    // Convert data to a span of bytes
    template <typename T>
    static std::span<std::byte> GetBytes(T& data);
    template <typename T>
    static std::span<const std::byte> GetBytes(const T& data);
    template <typename T>
    static std::span<std::byte> GetBytes(std::span<T> data);
    template <typename T>
    static std::span<const std::byte> GetBytes(std::span<const T> data);
};

// Template methods implementation to perform the conversion to bytes, not relevant to the course

template <typename T>
std::span<std::byte> Data::GetBytes(T& data)
{
    return std::span<std::byte>(reinterpret_cast<std::byte*>(&data), sizeof(T));
}

template <typename T>
std::span<const std::byte> Data::GetBytes(const T& data)
{
    return std::span<const std::byte>(reinterpret_cast<const std::byte*>(&data), sizeof(T));
}

template <typename T>
std::span<std::byte> Data::GetBytes(std::span<T> data)
{
    return std::span<std::byte>(reinterpret_cast<std::byte*>(data.data()), data.size_bytes());
}

template <typename T>
std::span<const std::byte> Data::GetBytes(std::span<const T> data)
{
    return std::span<const std::byte>(reinterpret_cast<const std::byte*>(data.data()), data.size_bytes());
}

template<typename T>
inline Data::Type Data::GetType(const T&) { return GetType<T>(); }

template<> inline Data::Type Data::GetType<GLfloat>() { return Type::Float; }
template<> inline Data::Type Data::GetType<GLdouble>() { return Type::Double; }
template<> inline Data::Type Data::GetType<GLbyte>() { return Type::Byte; }
template<> inline Data::Type Data::GetType<GLubyte>() { return Type::UByte; }
template<> inline Data::Type Data::GetType<GLshort>() { return Type::Short; }
template<> inline Data::Type Data::GetType<GLushort>() { return Type::UShort; }
template<> inline Data::Type Data::GetType<GLint>() { return Type::Int; }
template<> inline Data::Type Data::GetType<GLuint>() { return Type::UInt; }
