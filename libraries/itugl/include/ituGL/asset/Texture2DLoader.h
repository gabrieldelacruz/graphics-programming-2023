#pragma once

#include <ituGL/asset/TextureLoader.h>
#include <ituGL/texture/Texture2DObject.h>

// Asset loader for Texture2DObject
class Texture2DLoader : public TextureLoader<Texture2DObject>
{
public:
    Texture2DLoader();
    Texture2DLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat);

    // Load the texture from the path
    Texture2DObject Load(const char* path) override;

    // Helper to easily load a shared texture
    static std::shared_ptr<Texture2DObject> LoadTextureShared(const char* path,
        TextureObject::Format format, TextureObject::InternalFormat internalFormat,
        bool generateMipmap = true, bool flipVertical = false);

    inline bool GetFlipVertical() const { return m_flipVertical; }
    inline void SetFlipVertical(bool flipVertical) { m_flipVertical = flipVertical; }

private:
    // If true, the texture will be flipped vertically on load
    // This option exists because some systems define the vertical origin as "up", and others as "down"
    bool m_flipVertical;
};
