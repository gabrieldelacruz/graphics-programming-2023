#pragma once

#include <ituGL/core/Object.h>
#include <span>

// Abstract OpenGL object that encapsulates a Texture
// There are different subtypes depending on the target
class TextureObject : public Object
{
public:
    // Texture target: Subtype of texture (2D, 3D, array...)
    enum Target : GLenum;

    // High level format of the texture
    enum Format : GLenum;

    // Actual format of the texture data
    enum InternalFormat : GLint;

    // Texture parameters of different types
    enum class ParameterFloat : GLenum;
    enum class ParameterInt : GLenum;
    enum class ParameterEnum : GLenum;
    enum class ParameterEnumVector : GLenum;
    enum class ParameterColor : GLenum;

public:
    TextureObject();
    virtual ~TextureObject();

    // (C++) 8
    // Move semantics
    TextureObject(TextureObject&&) = default;
    TextureObject& operator = (TextureObject&&) = default;

    // (C++) 3
    // Use the same Bind method from the base class
    // It was not inherited implicitly because we defined another method named Bind (below)
    using Object::Bind;

    // Each derived class will return its Target
    virtual Target GetTarget() const = 0;

    // Generate mipmaps automatically for this texture
    void GenerateMipmap();

    // Get value of the texture parameter of type float
    void GetParameter(ParameterFloat pname, GLfloat& param) const;
    // Set value of the texture parameter of type float
    void SetParameter(ParameterFloat pname, GLfloat param);

    // Get value of the texture parameter of type int
    void GetParameter(ParameterInt pname, GLint& param) const;
    // Set value of the texture parameter of type int
    void SetParameter(ParameterInt pname, GLint param);

    // Get value of the texture parameter of type enum
    void GetParameter(ParameterEnum pname, GLenum& param) const;
    // Set value of the texture parameter of type enum
    void SetParameter(ParameterEnum pname, GLenum param);

    // Get value of the texture parameter of type enum vector
    void GetParameter(ParameterEnumVector pname, std::span<GLenum> params) const;
    // Set value of the texture parameter of type enum vector
    void SetParameter(ParameterEnumVector pname, std::span<const GLenum> params);

    // Get value of the texture parameter of type color
    void GetParameter(ParameterColor pname, std::span<GLfloat, 4> params) const;
    // Set value of the texture parameter of type color
    void SetParameter(ParameterColor pname, std::span<const GLfloat, 4> params);

    // Get number of componentes (1-4) of a specific texture format)
    static int GetComponentCount(Format format);

    // Get number of components of the data type of the texture (packed components count as 1)
    static int GetDataComponentCount(InternalFormat internalFormat);

    // Set active texture unit
    static void SetActiveTexture(GLint textureUnit);

protected:
    // Bind the specific target. Used by the Bind() method in derived classes
    void Bind(Target target) const;
    // Unbind the specific target. It is static because we don´t need any objects to do it
    static void Unbind(Target target);

#ifndef NDEBUG
    // Get active texture unit
    static GLint GetActiveTexture();

    //Check if the combination of Format and InternalFormat is valid
    static bool IsValidFormat(Format format, InternalFormat internalFormat);
#endif

};

// (C++) 5
// Templated TextureObject derived class based on the Target type
// Implements methods that are common to all targets
template<TextureObject::Target T>
class TextureObjectBase : public TextureObject
{
public:
    inline TextureObjectBase() {}

    // Return the templated enum value T
    inline Target GetTarget() const override { return T; }

    // When binding this object, we bind it to the corresponding Target
    void Bind() const override;

    // When unbinding this class, we unbind the corresponding Target
    static inline void Unbind();

#ifndef NDEBUG
    // Check if there is any TextureObject currently bound to this target
    inline static bool IsAnyBound() { return s_boundHandle != Object::NullHandle; }
#endif

protected:
#ifndef NDEBUG
    // Check if this TextureObject is currently bound to this target
    inline bool IsBound() const override { return s_boundHandle == GetHandle(); }

    // Handle of the TextureObject that is currently bound to this target
    static Handle s_boundHandle;
#endif
};

template<TextureObject::Target T>
Object::Handle TextureObjectBase<T>::s_boundHandle = Object::NullHandle;

template<TextureObject::Target T>
void TextureObjectBase<T>::Bind() const
{
    TextureObject::Bind(T);
#ifndef NDEBUG
    s_boundHandle = GetHandle();
#endif
}

template<TextureObject::Target T>
void TextureObjectBase<T>::Unbind()
{
    TextureObject::Unbind(T);
#ifndef NDEBUG
    s_boundHandle = NullHandle;
#endif
}


// TextureObject enums

enum TextureObject::Target : GLenum
{
    Texture1D = GL_TEXTURE_1D,
    Texture1DArray = GL_TEXTURE_1D_ARRAY,
    Texture2D = GL_TEXTURE_2D,
    Texture2DArray = GL_TEXTURE_2D_ARRAY,
    Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
    Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    Texture3D = GL_TEXTURE_3D,
    TextureCubemap = GL_TEXTURE_CUBE_MAP,
    TextureCubemapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
    TextureBuffer = GL_TEXTURE_BUFFER,
    TextureRectangle = GL_TEXTURE_RECTANGLE
};

enum TextureObject::Format : GLenum
{
    FormatInvalid = GL_NONE,
    FormatR = GL_RED,
    FormatRG = GL_RG,
    FormatRGB = GL_RGB,
    FormatBGR = GL_BGR,
    FormatRGBA = GL_RGBA,
    FormatBGRA = GL_BGRA,
    FormatDepth = GL_DEPTH_COMPONENT,
    FormatDepthStencil = GL_DEPTH_STENCIL
};

enum TextureObject::InternalFormat : GLint
{
    InternalFormatInvalid = GL_NONE,
    // Basic
    InternalFormatR = GL_RED,
    InternalFormatRG = GL_RG,
    InternalFormatRGB = GL_RGB,
    InternalFormatRGBA = GL_RGBA,
    // 8-bit unsigned normalized
    InternalFormatR8 = GL_R8,
    InternalFormatRG8 = GL_RG8,
    InternalFormatRGB8 = GL_RGB8,
    InternalFormatRGBA8 = GL_RGBA8,
    // 16-bit unsigned normalized
    InternalFormatR16 = GL_R16,
    InternalFormatRG16 = GL_RG16,
    InternalFormatRGB16 = GL_RGB16,
    InternalFormatRGBA16 = GL_RGBA16,
    // 8-bit signed normalized
    InternalFormatR8SNorm = GL_R8_SNORM,
    InternalFormatRG8SNorm = GL_RG8_SNORM,
    InternalFormatRGB8SNorm = GL_RGB8_SNORM,
    InternalFormatRGBA8SNorm = GL_RGBA8_SNORM,
    // 16-bit signed normalized
    InternalFormatR16SNorm = GL_R16_SNORM,
    InternalFormatRG16SNorm = GL_RG16_SNORM,
    InternalFormatRGB16SNorm = GL_RGB16_SNORM,
    InternalFormatRGBA16SNorm = GL_RGBA16_SNORM,
    // 16-bit float
    InternalFormatR16F = GL_R16F,
    InternalFormatRG16F = GL_RG16F,
    InternalFormatRGB16F = GL_RGB16F,
    InternalFormatRGBA16F = GL_RGBA16F,
    // 32-bit float
    InternalFormatR32F = GL_R32F,
    InternalFormatRG32F = GL_RG32F,
    InternalFormatRGB32F = GL_RGB32F,
    InternalFormatRGBA32F = GL_RGBA32F,
    // sRGB
    InternalFormatSRGB8 = GL_SRGB8,
    InternalFormatSRGBA8 = GL_SRGB8_ALPHA8,
    // Compressed
    InternalFormatRCompressed = GL_COMPRESSED_RED,
    InternalFormatRGCompressed = GL_COMPRESSED_RG,
    InternalFormatRGBCompressed = GL_COMPRESSED_RGB,
    InternalFormatRGBACompressed = GL_COMPRESSED_RGBA,
    InternalFormatSRGBCompressed = GL_COMPRESSED_SRGB,
    InternalFormatSRGBACompressed = GL_COMPRESSED_SRGB_ALPHA,
    // Depth Stencil
    InternalFormatDepth = GL_DEPTH_COMPONENT,
    InternalFormatDepth16 = GL_DEPTH_COMPONENT16,
    InternalFormatDepth24 = GL_DEPTH_COMPONENT24,
    InternalFormatDepth32 = GL_DEPTH_COMPONENT32,
    InternalFormatDepth32F = GL_DEPTH_COMPONENT32F,
    InternalFormatDepthStencil = GL_DEPTH_STENCIL,
    InternalFormatDepth24Stencil8 = GL_DEPTH24_STENCIL8,
    InternalFormatDepth32FStencil8 = GL_DEPTH32F_STENCIL8,
    // Others
    InternalFormatR11G11B10 = GL_R11F_G11F_B10F,
    InternalFormatRGB10A2 = GL_RGB10_A2,
    // And many more....
};

enum class TextureObject::ParameterFloat : GLenum
{
    MinLod = GL_TEXTURE_MIN_LOD,
    MaxLod = GL_TEXTURE_MAX_LOD,
    LodBias = GL_TEXTURE_LOD_BIAS,
};

enum class TextureObject::ParameterInt : GLenum
{
    BaseLevel = GL_TEXTURE_BASE_LEVEL,
    MaxLevel = GL_TEXTURE_MAX_LEVEL,
};

enum class TextureObject::ParameterEnum : GLenum
{
    MinFilter = GL_TEXTURE_MIN_FILTER, // GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
    MagFilter = GL_TEXTURE_MAG_FILTER, // GL_NEAREST, GL_LINEAR
    WrapS = GL_TEXTURE_WRAP_S, // GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT
    WrapT = GL_TEXTURE_WRAP_T, // GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT
    WrapR = GL_TEXTURE_WRAP_R, // GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT
    SwizzleRed = GL_TEXTURE_SWIZZLE_R,   // GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_ZERO, GL_ONE
    SwizzleGreen = GL_TEXTURE_SWIZZLE_G, // GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_ZERO, GL_ONE
    SwizzleBlue = GL_TEXTURE_SWIZZLE_B,  // GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_ZERO, GL_ONE
    SwizzleAlpha = GL_TEXTURE_SWIZZLE_A, // GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_ZERO, GL_ONE
    DepthStencilMode = GL_DEPTH_STENCIL_TEXTURE_MODE, // GL_DEPTH_COMPONENT, GL_STENCIL_INDEX
};

enum class TextureObject::ParameterEnumVector : GLenum
{
    SwizzleRGBA = GL_TEXTURE_SWIZZLE_RGBA,
};

enum class TextureObject::ParameterColor : GLenum
{
    BorderColor = GL_TEXTURE_BORDER_COLOR,
};

