#include <ituGL/texture/TextureObject.h>

#include <cassert>

TextureObject::TextureObject() : Object(NullHandle)
{
    Handle& handle = GetHandle();
    glGenTextures(1, &handle);
}

TextureObject::~TextureObject()
{
    Handle& handle = GetHandle();
    glDeleteTextures(1, &handle);
}

#ifndef NDEBUG
GLint TextureObject::GetActiveTexture()
{
    GLint activeTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
    return activeTexture - GL_TEXTURE0;
}
#endif

void TextureObject::SetActiveTexture(GLint textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
}

void TextureObject::Bind(Target target) const
{
    Handle handle = GetHandle();
    glBindTexture(target, handle);
}

void TextureObject::Unbind(Target target)
{
    Handle handle = NullHandle;
    glBindTexture(target, handle);
}

void TextureObject::GenerateMipmap()
{
    assert(IsBound());
    glGenerateMipmap(GetTarget());
}

void TextureObject::GetParameter(ParameterFloat pname, GLfloat& param) const
{
    assert(IsBound());
    glGetTexParameterfv(GetTarget(), static_cast<GLenum>(pname), &param);
}

void TextureObject::SetParameter(ParameterFloat pname, GLfloat param)
{
    assert(IsBound());
    glTexParameterf(GetTarget(), static_cast<GLenum>(pname), param);
}

void TextureObject::GetParameter(ParameterInt pname, GLint& param) const
{
    assert(IsBound());
    glGetTexParameteriv(GetTarget(), static_cast<GLenum>(pname), &param);
}

void TextureObject::SetParameter(ParameterInt pname, GLint param)
{
    assert(IsBound());
    glTexParameteri(GetTarget(), static_cast<GLenum>(pname), param);
}

void TextureObject::GetParameter(ParameterEnum pname, GLenum& param) const
{
    assert(IsBound());
    glGetTexParameterIuiv(GetTarget(), static_cast<GLenum>(pname), &param);
}

void TextureObject::SetParameter(ParameterEnum pname, GLenum param)
{
    assert(IsBound());
    glTexParameteri(GetTarget(), static_cast<GLenum>(pname), param);
}

void TextureObject::GetParameter(ParameterEnumVector pname, std::span<GLenum> params) const
{
    assert(IsBound());
    glGetTexParameterIuiv(GetTarget(), static_cast<GLenum>(pname), params.data());
}

void TextureObject::SetParameter(ParameterEnumVector pname, std::span<const GLenum> params)
{
    assert(IsBound());
    glTexParameterIuiv(GetTarget(), static_cast<GLenum>(pname), params.data());
}

void TextureObject::GetParameter(ParameterColor pname, std::span<GLfloat, 4> params) const
{
    assert(IsBound());
    glGetTexParameterfv(GetTarget(), static_cast<GLenum>(pname), params.data());
}

void TextureObject::SetParameter(ParameterColor pname, std::span<const GLfloat, 4> params)
{
    assert(IsBound());
    glTexParameterfv(GetTarget(), static_cast<GLenum>(pname), params.data());
}

#ifndef NDEBUG
bool TextureObject::IsValidFormat(Format format, InternalFormat internalFormat)
{
    switch (internalFormat)
    {
    case InternalFormatR:
    case InternalFormatR8:
    case InternalFormatR16:
    case InternalFormatR8SNorm:
    case InternalFormatR16SNorm:
    case InternalFormatR16F:
    case InternalFormatR32F:
    case InternalFormatRCompressed:
        return format == FormatR;
    case InternalFormatRG:
    case InternalFormatRG8:
    case InternalFormatRG16:
    case InternalFormatRG8SNorm:
    case InternalFormatRG16SNorm:
    case InternalFormatRG16F:
    case InternalFormatRG32F:
    case InternalFormatRGCompressed:
        return format == FormatRG;
    case InternalFormatRGB:
    case InternalFormatRGB8:
    case InternalFormatRGB16:
    case InternalFormatRGB8SNorm:
    case InternalFormatRGB16SNorm:
    case InternalFormatRGB16F:
    case InternalFormatRGB32F:
    case InternalFormatSRGB8:
    case InternalFormatRGBCompressed:
    case InternalFormatSRGBCompressed:
    case InternalFormatR11G11B10:
        return format == FormatRGB || format == FormatBGR;
    case InternalFormatRGBA:
    case InternalFormatRGBA8:
    case InternalFormatRGBA16:
    case InternalFormatRGBA8SNorm:
    case InternalFormatRGBA16SNorm:
    case InternalFormatRGBA16F:
    case InternalFormatRGBA32F:
    case InternalFormatSRGBA8:
    case InternalFormatRGBACompressed:
    case InternalFormatSRGBACompressed:
    case InternalFormatRGB10A2:
        return format == FormatRGBA || format == FormatBGRA;
    case InternalFormatDepth:
    case InternalFormatDepth16:
    case InternalFormatDepth24:
    case InternalFormatDepth32:
    case InternalFormatDepth32F:
        return format == FormatDepth;
    case InternalFormatDepthStencil:
    case InternalFormatDepth24Stencil8:
    case InternalFormatDepth32FStencil8:
        return format == FormatDepthStencil;
    default:
        //Unknown format
        return false;
    }
}

int TextureObject::GetComponentCount(Format format)
{
    switch (format)
    {
    case FormatR:
    case FormatDepth:
        return 1;
    case FormatRG:
    case FormatDepthStencil:
        return 2;
    case FormatRGB:
    case FormatBGR:
        return 3;
    case FormatRGBA:
    case FormatBGRA:
        return 4;
    default:
        //Unknown format
        return 0;
    }
}

int TextureObject::GetDataComponentCount(InternalFormat internalFormat)
{
    switch (internalFormat)
    {
    case InternalFormatR:
    case InternalFormatR8:
    case InternalFormatR8SNorm:
    case InternalFormatR16:
    case InternalFormatR16SNorm:
    case InternalFormatR16F:
    case InternalFormatR32F:
    case InternalFormatRCompressed:
    case InternalFormatR11G11B10:
    case InternalFormatRGB10A2:
    case InternalFormatDepth:
    case InternalFormatDepth16:
    case InternalFormatDepth24:
    case InternalFormatDepth32:
    case InternalFormatDepth32F:
    case InternalFormatDepthStencil:
    case InternalFormatDepth24Stencil8:
        return 1;
    case InternalFormatRG:
    case InternalFormatRG8:
    case InternalFormatRG8SNorm:
    case InternalFormatRG16:
    case InternalFormatRG16SNorm:
    case InternalFormatRG16F:
    case InternalFormatRG32F:
    case InternalFormatRGCompressed:
        return 2;
    case InternalFormatRGB:
    case InternalFormatRGB8:
    case InternalFormatRGB8SNorm:
    case InternalFormatRGB16:
    case InternalFormatRGB16SNorm:
    case InternalFormatRGB16F:
    case InternalFormatRGB32F:
    case InternalFormatSRGB8:
    case InternalFormatRGBCompressed:
    case InternalFormatSRGBCompressed:
        return 3;
    case InternalFormatRGBA:
    case InternalFormatRGBA8:
    case InternalFormatRGBA8SNorm:
    case InternalFormatRGBA16:
    case InternalFormatRGBA16SNorm:
    case InternalFormatRGBA16F:
    case InternalFormatRGBA32F:
    case InternalFormatSRGBA8:
    case InternalFormatRGBACompressed:
    case InternalFormatSRGBACompressed:
        return 4;
    default:
        //Unknown format
        return 0;
    }
}
#endif
