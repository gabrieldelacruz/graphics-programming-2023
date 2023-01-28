#pragma once

#include <ituGL/core/BufferObject.h>
#include <ituGL/core/Data.h>

// Vertex Buffer Object (VBO) is the common term for a BufferObject when it is used as a source for vertex array data
class VertexBufferObject : public BufferObjectBase<BufferObject::ArrayBuffer>
{
public:
    VertexBufferObject();

    // (C++) 3
    // Use the same AllocateData methods from the base class
    using BufferObject::AllocateData;
    // Additionally, provide AllocateData methods with StaticDraw as default usage
    void AllocateData(size_t size);
    void AllocateData(std::span<const std::byte> data);
    // Additionally, provide AllocateData template method for any type of data span, specifying the VertexAttribute that it contains
    template<typename T>
    void AllocateData(std::span<const T> data, Usage usage = Usage::StaticDraw);
    template<typename T>
    inline void AllocateData(std::span<T> data, Usage usage = Usage::StaticDraw) { AllocateData(std::span<const T>(data), usage); }

    // (C++) 3
    // Use the same UpdateData methods from the base class
    using BufferObject::UpdateData;
    // Additionally, provide UpdateData template method for any type of data span
    template<typename T>
    void UpdateData(std::span<const T> data, size_t offsetBytes = 0);
    template<typename T>
    inline void UpdateData(std::span<T> data, size_t offsetBytes = 0) { UpdateData(std::span<const T>(data), offsetBytes); }
};


// Call the base implementation with the span converted to bytes
template<typename T>
void VertexBufferObject::AllocateData(std::span<const T> data, Usage usage)
{
    AllocateData(Data::GetBytes(data), usage);
}

// Call the base implementation with the span converted to bytes
template<typename T>
void VertexBufferObject::UpdateData(std::span<const T> data, size_t offsetBytes)
{
    UpdateData(Data::GetBytes(data), offsetBytes);
}
