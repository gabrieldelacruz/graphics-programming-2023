#include <ituGL/core/BufferObject.h>

// Create the object initially null, get object handle and generate 1 buffer
BufferObject::BufferObject() : Object(NullHandle)
{
    Handle& handle = GetHandle();
    // (todo) 00.1: Generate 1 buffer
}

// Get object handle and delete 1 buffer
BufferObject::~BufferObject()
{
    Handle& handle = GetHandle();
    // (todo) 00.1: Delete 1 buffer
}

// Bind the buffer handle to the specific target
void BufferObject::Bind(Target target) const
{
    Handle handle = GetHandle();
    // (todo) 00.1: Bind buffer
}

// Bind the null handle to the specific target
void BufferObject::Unbind(Target target)
{
    Handle handle = NullHandle;
    // (todo) 00.1: Bind null buffer
}

// Get buffer Target and allocate buffer data
void BufferObject::AllocateData(size_t size, Usage usage)
{
    Target target = GetTarget();
    // (todo) 00.1: Allocate without initial data (you can use nullptr instead)
}

// Get buffer Target and allocate buffer data
void BufferObject::AllocateData(std::span<const std::byte> data, Usage usage)
{
    Target target = GetTarget();
    // (todo) 00.1: Allocate with initial data, specifying the size in bytes and the pointer to the data
}

// Get buffer Target and set buffer subdata
void BufferObject::UpdateData(std::span<const std::byte> data, size_t offset)
{
    // (todo) 00.5: Update buffer subdata
}
