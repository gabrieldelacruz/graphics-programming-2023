#pragma once

#include <ituGL/core/Object.h>
#include <span>

class BufferObject : public Object
{
public:
    // Buffer target: What the buffer will be used for
    enum Target : GLenum
    {
        // Vertex Buffer Object
        ArrayBuffer = GL_ARRAY_BUFFER,
        // Element Buffer Object
        ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
        // TODO: There are more types, add them when they are supported
    };

    // Usage: How the buffer will be used
    // Hints to OpenGL so it can optimize the operations on this buffer
    // Combinations of 2 different properties. Unified in one value to be consistent with OpenGL:
    // - Based on how frequently the user will be changing the buffer's data:
    //   * STATIC: The user will set the data once
    //   * DYNAMIC: The user will set the data occasionally
    //   * STREAM: The user will be changing the data after every use.Or almost every use
    // - Based on what the user will be doing with the buffer:
    //   * DRAW: The user will be writing data to the buffer, but the user will not read it
    //   * READ: The user will not be writing data, but the user will be reading it back
    //   * COPY: The user will be neither writing nor reading the data
    enum Usage : GLenum
    {
        StaticDraw = GL_STATIC_DRAW,    StaticRead = GL_STATIC_READ,    StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,  DynamicRead = GL_DYNAMIC_READ,  DynamicCopy = GL_DYNAMIC_COPY,
        StreamDraw = GL_STREAM_DRAW,    StreamRead = GL_STREAM_READ,    StreamCopy = GL_STREAM_COPY
    };

public:
    BufferObject();
    virtual ~BufferObject();

    // (C++) 8
    // Move semantics
    BufferObject(BufferObject&& bufferObject) noexcept;
    BufferObject& operator = (BufferObject&& bufferObject) noexcept;

    // (C++) 3
    // Use the same Bind method from the base class
    // It was not inherited implicitly because we defined another method named Bind (below)
    using Object::Bind;

    // Each derived class will return its Target
    virtual Target GetTarget() const = 0;

    // Allocate the buffer, specifying the size and the usage. We can also provide initial contents
    void AllocateData(size_t size, Usage usage);
    void AllocateData(std::span<const std::byte> data, Usage usage);

    // Modify the contents of the buffer, starting at offset
    void UpdateData(std::span<const std::byte> data, size_t offset = 0);

protected:
    // Bind the specific target. Used by the Bind() method in derived classes
    void Bind(Target target) const;
    // Unbind the specific target. It is static because we don´t need any objects to do it
    static void Unbind(Target target);
};

// (C++) 5
// Templated BufferObject derived class based on the Target type
// Implements methods that are common to all targets
template<BufferObject::Target T>
class BufferObjectBase : public BufferObject
{
public:
    inline BufferObjectBase() {}

    // Return the templated enum value T
    inline Target GetTarget() const override { return T; }

    // When binding this object, we bind it to the corresponding Target
    void Bind() const override;

    // When unbinding this class, we unbind the corresponding Target
    static void Unbind();

#ifndef NDEBUG
    // Check if there is any BufferObject currently bound to this target
    inline static bool IsAnyBound() { return s_boundHandle != Object::NullHandle; }
#endif

protected:
#ifndef NDEBUG
    // Check if this BufferObject is currently bound to this target
    inline bool IsBound() const override { return s_boundHandle == GetHandle(); }

    // Handle of the buffer object that is currently bound to this target
    static Handle s_boundHandle;
#endif
};

#ifndef NDEBUG
template<BufferObject::Target T>
Object::Handle BufferObjectBase<T>::s_boundHandle = Object::NullHandle;
#endif

template<BufferObject::Target T>
void BufferObjectBase<T>::Bind() const
{
    BufferObject::Bind(T);
#ifndef NDEBUG
    s_boundHandle = GetHandle();
#endif
}

template<BufferObject::Target T>
void BufferObjectBase<T>::Unbind()
{
    BufferObject::Unbind(T);
#ifndef NDEBUG
    s_boundHandle = NullHandle;
#endif
}