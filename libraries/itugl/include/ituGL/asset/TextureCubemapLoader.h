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
        bool generateMipmap = true);

private:
    void LoadFace(TextureCubemapObject& textureCubemap, TextureCubemapObject::Face face, std::span<const std::byte> dataSrc, std::span<std::byte> dataDst, int x, int y, int side, Data::Type dataType);
};

