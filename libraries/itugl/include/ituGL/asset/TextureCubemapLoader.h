#pragma once

#include <ituGL/asset/TextureLoader.h>
#include <ituGL/texture/TextureCubemapObject.h>

// Asset loader for TextureCubemapObject
class TextureCubemapLoader : public TextureLoader<TextureCubemapObject>
{
public:
    TextureCubemapLoader();
    TextureCubemapLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat);

    // Load the texture from the path
    TextureCubemapObject Load(const char* path) override;

    // Helper to easily load a shared texture
    static std::shared_ptr<TextureCubemapObject> LoadTextureShared(const char* path,
        TextureObject::Format format, TextureObject::InternalFormat internalFormat,
        bool generateMipmap = true, bool flipVertical = false);

    inline bool GetFlipVertical() const { return m_flipVertical; }
    inline void SetFlipVertical(bool flipVertical) { m_flipVertical = flipVertical; }

private:
    void LoadFace(TextureCubemapObject& textureCubemap, TextureCubemapObject::Face face, std::span<const unsigned char> dataSrc, std::span<unsigned char> dataDst, int x, int y, int side);

private:
    // If true, the texture will be flipped vertically on load
    // This option exists because some systems define the vertical origin as "up", and others as "down"
    bool m_flipVertical;
};
