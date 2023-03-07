#pragma once

#include <ituGL/asset/AssetLoader.h>
#include <ituGL/texture/TextureObject.h>

// Base class for all Texture asset loaders
template<typename T>
class TextureLoader : public AssetLoader<T>
{
public:
    TextureLoader();
    TextureLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat);

    inline TextureObject::Format GetFormat() const { return m_format; }
    inline void SetFormat(TextureObject::Format format) { m_format = format; }

    inline TextureObject::InternalFormat GetInternalFormat() const { return m_internalFormat; }
    inline void SetInternalFormat(TextureObject::InternalFormat internalFormat) { m_internalFormat = internalFormat; }

    inline bool GetGenerateMipmap() const { return m_generateMipmap; }
    inline void SetGenerateMipmap(bool generateMipmap) { m_generateMipmap = generateMipmap; }

protected:
    // Format to apply to the loaded textures
    TextureObject::Format m_format;

    // Internal format of the loaded textures
    TextureObject::InternalFormat m_internalFormat;

    // If the texture object should generate mipmaps after
    bool m_generateMipmap;
};

template<typename T>
TextureLoader<T>::TextureLoader()
    : TextureLoader<T>(TextureObject::Format::FormatInvalid, TextureObject::InternalFormat::InternalFormatInvalid)
{
}

template<typename T>
TextureLoader<T>::TextureLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat)
    : m_format(format), m_internalFormat(internalFormat), m_generateMipmap(false)
{
}
