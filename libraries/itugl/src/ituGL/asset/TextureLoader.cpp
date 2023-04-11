#include <ituGL/asset/TextureLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::span<const std::byte> TextureLoaderUtils::LoadTexture2DData(const char* path, int& width, int& height, Data::Type& dataType, TextureObject::Format format, TextureObject::InternalFormat internalFormat, bool flipVertical)
{
    std::span<const std::byte> dataSpan;

    int componentCount = TextureObject::GetComponentCount(format);
    int originalComponentCount;

    // Set flip vertical on load if needed
    stbi_set_flip_vertically_on_load(flipVertical ? 1 : 0);

    if (IsHDR(internalFormat))
    {
        float* data = stbi_loadf(path, &width, &height, &originalComponentCount, componentCount);
        std::span<const float> dataSpanFloat(data, data ? width * height * componentCount : 0);
        dataSpan = Data::GetBytes(dataSpanFloat);
        dataType = Data::Type::Float;
    }
    else
    {
        unsigned char* data = stbi_load(path, &width, &height, &originalComponentCount, componentCount);
        std::span<const unsigned char> dataSpanByte(data, data ? width * height * componentCount : 0);
        dataSpan = Data::GetBytes(dataSpanByte);
        dataType = Data::Type::UByte;
    }
    return dataSpan;
}

void TextureLoaderUtils::FreeTexture2DData(std::span<const std::byte> data)
{
    const void* dataPtr = data.data();
    stbi_image_free(const_cast<void*>(dataPtr));
}

bool TextureLoaderUtils::IsHDR(TextureObject::InternalFormat internalFormat)
{
    switch (internalFormat)
    {
    case TextureObject::InternalFormatR16F:
    case TextureObject::InternalFormatRG16F:
    case TextureObject::InternalFormatRGB16F:
    case TextureObject::InternalFormatRGBA16F:
    case TextureObject::InternalFormatR32F:
    case TextureObject::InternalFormatRG32F:
    case TextureObject::InternalFormatRGB32F:
    case TextureObject::InternalFormatRGBA32F:
        return true;
    default:
        return false;
    }
}
