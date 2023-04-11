#include <ituGL/asset/Texture2DLoader.h>

#include <cassert>

Texture2DLoader::Texture2DLoader()
    : m_flipVertical(false)
{
}

Texture2DLoader::Texture2DLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat)
    : TextureLoader(format, internalFormat)
    , m_flipVertical(false)
{
}

Texture2DObject Texture2DLoader::Load(const char* path)
{
    Texture2DObject texture2D;

    // Load texture data using stbimage library
    int width, height;
    Data::Type dataType;
    std::span<const std::byte> data = LoadTexture2DData(path, width, height, dataType, m_flipVertical);

    // If data was loaded, copy it to the texture object
    assert(!data.empty());
    if (!data.empty())
    {
        texture2D.Bind();
        texture2D.SetImage<std::byte>(0, width, height, m_format, m_internalFormat, data, dataType);

        texture2D.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR);
        texture2D.SetParameter(TextureObject::ParameterEnum::MagFilter, GL_LINEAR);

        // Generate mipmap if needed
        if (m_generateMipmap)
        {
            texture2D.GenerateMipmap();
            texture2D.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR_MIPMAP_LINEAR);

            // Adjust mip levels
            texture2D.SetParameter(TextureObject::ParameterFloat::MinLod, 0.0f);
            float maxLod = 1.0f + std::floorf(std::log2f(static_cast<float>(std::max(width, height))));
            texture2D.SetParameter(TextureObject::ParameterFloat::MaxLod, maxLod);
        }

        texture2D.Unbind();

        // Free loaded data (not needed anymore)
        FreeTexture2DData(data);
    }
    return texture2D;
}

std::shared_ptr<Texture2DObject> Texture2DLoader::LoadTextureShared(const char* path,
    TextureObject::Format format, TextureObject::InternalFormat internalFormat, bool generateMipmap, bool flipVertical)
{
    Texture2DLoader loader(format, internalFormat);
    loader.SetGenerateMipmap(generateMipmap);
    loader.SetFlipVertical(flipVertical);
    return loader.LoadShared(path);
}
