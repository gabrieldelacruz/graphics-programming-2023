#pragma once

#include <ituGL/texture/TextureObject.h>
#include <ituGL/core/Data.h>

// Texture object with 6 faces (square 2D textures with the same size)
class TextureCubemapObject : public TextureObjectBase<TextureObject::TextureCubemap>
{
public:
    enum class Face
    {
        Left   = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        Right  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        Bottom = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        Top    = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        Front  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        Back   = GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };

public:
    TextureCubemapObject();

    // Initialize all the sides of the texture with a specific format
    void SetImage(GLint level, GLsizei side, Format format, InternalFormat internalFormat);

    // Initialize the texture2D with a specific format and initial data
    template <typename T>
    void SetImage(GLint level, Face face, GLsizei side,
        Format format, InternalFormat internalFormat,
        std::span<const T> data, Data::Type type = Data::Type::None);
};

// Set image with data in bytes
template <>
void TextureCubemapObject::SetImage<std::byte>(GLint level, Face face, GLsizei side, Format format, InternalFormat internalFormat, std::span<const std::byte> data, Data::Type type);

// Template method to set image with any kind of data
template <typename T>
inline void TextureCubemapObject::SetImage(GLint level, Face face, GLsizei side,
    Format format, InternalFormat internalFormat, std::span<const T> data, Data::Type type)
{
    if (type == Data::Type::None)
    {
        type = Data::GetType<T>();
    }
    SetImage(level, face, side, format, internalFormat, Data::GetBytes(data), type);
}

