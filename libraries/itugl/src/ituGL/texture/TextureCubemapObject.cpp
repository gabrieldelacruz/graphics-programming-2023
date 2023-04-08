#include <ituGL/texture/TextureCubemapObject.h>

#include <cassert>

TextureCubemapObject::TextureCubemapObject()
{
}

template <>
void TextureCubemapObject::SetImage<std::byte>(GLint level, Face face, GLsizei side, Format format, InternalFormat internalFormat, std::span<const std::byte> data, Data::Type type)
{
    assert(IsBound());
    assert(data.empty() || type != Data::Type::None);
    assert(IsValidFormat(format, internalFormat));
    assert(data.empty() || data.size_bytes() == side * side * GetDataComponentCount(internalFormat) * Data::GetTypeSize(type));
    glTexImage2D(static_cast<GLenum>(face), level, internalFormat, side, side, 0, format, static_cast<GLenum>(type), data.data());
}

void TextureCubemapObject::SetImage(GLint level, GLsizei side, Format format, InternalFormat internalFormat)
{
    std::span<std::byte> empty;
    SetImage<std::byte>(level, Face::Left, side, format, internalFormat, empty, Data::Type::None);
    SetImage<std::byte>(level, Face::Right, side, format, internalFormat, empty, Data::Type::None);
    SetImage<std::byte>(level, Face::Bottom, side, format, internalFormat, empty, Data::Type::None);
    SetImage<std::byte>(level, Face::Top, side, format, internalFormat, empty, Data::Type::None);
    SetImage<std::byte>(level, Face::Front, side, format, internalFormat, empty, Data::Type::None);
    SetImage<std::byte>(level, Face::Back, side, format, internalFormat, empty, Data::Type::None);
}
