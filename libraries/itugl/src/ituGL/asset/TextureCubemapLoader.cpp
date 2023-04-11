#include <ituGL/asset/TextureCubemapLoader.h>

#include <cassert>
#include <stb_image.h>

TextureCubemapLoader::TextureCubemapLoader()
{
}

TextureCubemapLoader::TextureCubemapLoader(TextureObject::Format format, TextureObject::InternalFormat internalFormat)
    : TextureLoader(format, internalFormat)
{
}

TextureCubemapObject TextureCubemapLoader::Load(const char* path)
{
    TextureCubemapObject textureCubemap;

    int width, height;
    Data::Type dataType;
    std::span<const std::byte> data = LoadTexture2DData(path, width, height, dataType);

    // If data was loaded, copy it to the texture object
    assert(!data.empty());
    if (!data.empty())
    {
        assert(width % 4 == 0);
        assert(height % 3 == 0);
        assert(width / 4 == height / 3);

        int side = width / 4;

        textureCubemap.Bind();

        int pixelSize = TextureObject::GetComponentCount(m_format) * Data::GetTypeSize(dataType);
        std::vector<std::byte> faceData(side * side * pixelSize);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Left,   data, faceData, 0, 1, side, dataType);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Right,  data, faceData, 2, 1, side, dataType);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Bottom, data, faceData, 1, 2, side, dataType);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Top,    data, faceData, 1, 0, side, dataType);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Front,  data, faceData, 3, 1, side, dataType);
        LoadFace(textureCubemap, TextureCubemapObject::Face::Back,   data, faceData, 1, 1, side, dataType);

        textureCubemap.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR);
        textureCubemap.SetParameter(TextureObject::ParameterEnum::MagFilter, GL_LINEAR);

        // Generate mipmap if needed
        if (m_generateMipmap)
        {
            textureCubemap.GenerateMipmap();
            textureCubemap.SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR_MIPMAP_LINEAR);

            // Adjust mip levels
            textureCubemap.SetParameter(TextureObject::ParameterFloat::MinLod, 0.0f);
            float maxLod = 1.0f + std::floorf(std::log2f(static_cast<float>(std::max(width, height))));
            textureCubemap.SetParameter(TextureObject::ParameterFloat::MaxLod, maxLod);
        }

        // Clamp to edge to avoid filtering on the edges
        textureCubemap.SetParameter(TextureObject::ParameterEnum::WrapR, GL_CLAMP_TO_EDGE);
        textureCubemap.SetParameter(TextureObject::ParameterEnum::WrapS, GL_CLAMP_TO_EDGE);
        textureCubemap.SetParameter(TextureObject::ParameterEnum::WrapT, GL_CLAMP_TO_EDGE);

        textureCubemap.Unbind();

        // Free loaded data (not needed anymore)
        FreeTexture2DData(data);
    }
    return textureCubemap;
}

std::shared_ptr<TextureCubemapObject> TextureCubemapLoader::LoadTextureShared(const char* path,
    TextureObject::Format format, TextureObject::InternalFormat internalFormat, bool generateMipmap)
{
    TextureCubemapLoader loader(format, internalFormat);
    loader.SetGenerateMipmap(generateMipmap);
    return loader.LoadShared(path);
}

void TextureCubemapLoader::LoadFace(TextureCubemapObject& textureCubemap, TextureCubemapObject::Face face, std::span<const std::byte> dataSrc, std::span<std::byte> dataDst, int x, int y, int side, Data::Type dataType)
{
    int pixelSize = TextureObject::GetComponentCount(m_format) * Data::GetTypeSize(dataType);
    int rowSize = side * pixelSize;
    int stride = 4 * rowSize;
    int srcOffset = y * side * stride + x * rowSize;
    int dstOffset = 0;
    for (int i = 0; i < side; ++i)
    {
        assert(srcOffset + rowSize <= dataSrc.size());
        assert(dstOffset + rowSize <= dataDst.size());
        memcpy(&dataDst[dstOffset], &dataSrc[srcOffset], rowSize);
        srcOffset += stride;
        dstOffset += rowSize;
    }

    textureCubemap.SetImage<std::byte>(0, face, side, m_format, m_internalFormat, dataDst, dataType);
}
