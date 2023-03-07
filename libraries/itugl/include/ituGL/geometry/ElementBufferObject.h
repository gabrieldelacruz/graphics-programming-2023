#pragma once

#include <ituGL/core/BufferObject.h>
#include <ituGL/core/Data.h>
#include <cassert>

// Element Buffer Object (VBO) is the common term for a BufferObject when it is storing a list of indices to vertices
class ElementBufferObject : public BufferObjectBase<BufferObject::ElementArrayBuffer>
{
public:
    ElementBufferObject();

    // AllocateData template method that replaces the size with the element count
    template<typename T>
    void AllocateData(size_t elementCount, Usage usage = Usage::StaticDraw);
    // AllocateData template method for any type of data span. Type must be one of the supported types
    template<typename T>
    void AllocateData(std::span<const T> data, Usage usage = Usage::StaticDraw);
    template<typename T>
    inline void AllocateData(std::span<T> data, Usage usage = Usage::StaticDraw) { AllocateData(std::span<const T>(data), usage); }

    // UpdateData template method for any type of data span. Type must be one of the supported types
    template<typename T>
    void UpdateData(std::span<const T> data, size_t offsetBytes = 0);

    // Returns the smallest type that can hold vertexCount indices
    static Data::Type GetSmallestType(unsigned int vertexCount);

#ifndef NDEBUG
    // Check if a data type is supported to be used as index
    static bool IsSupportedType(Data::Type type);
#endif
};

// Call the base implementation with the buffer size, computed with elementCount and size of T
template<typename T>
void ElementBufferObject::AllocateData(size_t elementCount, Usage usage)
{
    assert(IsSupportedType(Data::GetType<T>()));
    BufferObject::AllocateData(elementCount * sizeof(T), usage);
}

// Call the base implementation with the span converted to bytes
template<typename T>
void ElementBufferObject::AllocateData(std::span<const T> data, Usage usage)
{
    assert(IsSupportedType(Data::GetType<T>()));
    BufferObject::AllocateData(Data::GetBytes(data), usage);
}

// Call the base implementation with the span converted to bytes
template<typename T>
void ElementBufferObject::UpdateData(std::span<const T> data, size_t offsetBytes)
{
    assert(IsSupportedType(Data::GetType<T>()));
    BufferObject::UpdateData(Data::GetBytes(data), offsetBytes);
}
