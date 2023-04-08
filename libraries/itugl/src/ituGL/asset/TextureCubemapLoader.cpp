#include <ituGL/asset/TextureCubemapLoader.h>

#include <cassert>
#include <stb_image.h>

TextureCubemapLoader::TextureCubemapLoader()
    : m_flipVertical(false)
{
}

TextureCubemapLoader::TextureCubemapLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat)
    : TextureLoader(format, internalFormat)
    , m_flipVertical(false)
{
}

TextureCubemapObject TextureCubemapLoader::Load(const char* path)
{
    TextureCubemapObject textureCubemap;

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
        assert(width % 4 == 0);
        assert(height % 3 == 0);
        assert(width / 4 == height / 3);

        int side = width / 4;

        textureCubemap.Bind();

        std::vector<unsigned char> faceData(side * side * componentCount);
        std::span<const unsigned char> dataSpan(data, width * height * componentCount);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Left,   dataSpan, faceData, 0, 1, side);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Right,  dataSpan, faceData, 2, 1, side);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Bottom, dataSpan, faceData, 1, 2, side);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Top,    dataSpan, faceData, 1, 0, side);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Front,  dataSpan, faceData, 3, 1, side);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Back,   dataSpan, faceData, 1, 1, side);

        textureCubemap.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR);
        textureCubemap.SetParameter(TextureObject::ParameterEnum::MagFilter, GL_LINEAR);

        // Generate mipmap if needed
        if (m_generateMipmap)
        {
            textureCubemap.GenerateMipmap();
            textureCubemap.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR_MIPMAP_LINEAR);

            // Adjust mip levels
            textureCubemap.SetParameter(TextureObject::ParameterFloat::MinLod, 0.0f);
            float maxLod = 1.0f + std::floor(std::log2(std::max(width, height)));
            textureCubemap.SetParameter(TextureObject::ParameterFloat::MaxLod, maxLod);
        }

        // Clamp to edge to avoid filtering on the edges
        textureCubemap.SetParameter(TextureObject::ParameterEnum::WrapR, GL_CLAMP_TO_EDGE);
        textureCubemap.SetParameter(TextureObject::ParameterEnum::WrapS, GL_CLAMP_TO_EDGE);
        textureCubemap.SetParameter(TextureObject::ParameterEnum::WrapT, GL_CLAMP_TO_EDGE);

        textureCubemap.Unbind();

        // Free loaded data (not needed anymore)
        stbi_image_free(data);
    }
    return textureCubemap;
}

std::shared_ptr<TextureCubemapObject> TextureCubemapLoader::LoadTextureShared(const char* path,
    TextureObject::Format format, TextureObject::InternalFormat internalFormat, bool generateMipmap, bool flipVertical)
{
    TextureCubemapLoader loader(format, internalFormat);
    loader.SetGenerateMipmap(generateMipmap);
    loader.SetFlipVertical(flipVertical);
    return loader.LoadShared(path);
}

void TextureCubemapLoader::LoadFace(TextureCubemapObject& textureCubemap, TextureCubemapObject::Face face, std::span<const unsigned char> dataSrc, std::span<unsigned char> dataDst, int x, int y, int side)
{
    int componentCount = TextureObject::GetComponentCount(m_format);
    int rowSize = side * componentCount;
    int stride = 4 * rowSize;
    int srcOffset = y * side * stride + x * rowSize;
    int dstOffset = 0;
    for (int i = 0; i < side; ++i)
    {
        memcpy(&dataDst[dstOffset], &dataSrc[srcOffset], rowSize);
        srcOffset += stride;
        dstOffset += rowSize;
    }

    textureCubemap.SetImage<unsigned char>(0, face, side, m_format, m_internalFormat, dataDst);
}
