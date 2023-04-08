#include <ituGL/asset/Texture2DLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

    // Set flip vertical on load if needed
    stbi_set_flip_vertically_on_load(m_flipVertical ? 1 : 0);

    // Load texture data using stbimage library
    int componentCount = TextureObject::GetComponentCount(m_format);
    int width, height, originalComponentCount;
    unsigned char* data = stbi_load(path, &width, &height, &originalComponentCount, componentCount);

    // If data was loaded, copy it to the texture object
    assert(data);
    if (data)
    {
        texture2D.Bind();
        int dataSize = width * height * componentCount;
        texture2D.SetImage<unsigned char>(0, width, height, m_format, m_internalFormat, std::span(data, dataSize));

        texture2D.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR);
        texture2D.SetParameter(TextureObject::ParameterEnum::MagFilter, GL_LINEAR);

        // Generate mipmap if needed
        if (m_generateMipmap)
        {
            texture2D.GenerateMipmap();
            texture2D.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR_MIPMAP_LINEAR);

            // Adjust mip levels
            texture2D.SetParameter(TextureObject::ParameterFloat::MinLod, 0.0f);
            float maxLod = 1.0f + std::floor(std::log2(std::max(width, height)));
            texture2D.SetParameter(TextureObject::ParameterFloat::MaxLod, maxLod);
        }

        texture2D.Unbind();

        // Free loaded data (not needed anymore)
        stbi_image_free(data);
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
