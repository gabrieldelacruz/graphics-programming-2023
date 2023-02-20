#pragma once

#include <glad/glad.h>

class Object
{
public:
    // (C++) 2
    // Declare the type used for the object handle
    using Handle = GLuint;

public:
    Object(Handle handle);
    virtual ~Object();

    // (C++) 4
    // Make the object non-copyable, otherwise the same handle could be deleted twice
    Object(const Object&) = delete;
    void operator = (const Object&) = delete;

    // (C++) 8
    // Move semantics
    Object(Object&& object) noexcept;
    Object& operator = (Object&& object) noexcept;

    inline Handle GetHandle() const { return m_handle; }

    // An object is only valid if its handle is not null
    inline bool IsValid() const { return m_handle != NullHandle; }

    // Binding = Setting this object as the current one, so we can perform operations on it without passing it as an argument every time
    // Each type of object will use a different function to Bind, so we leave it as a pure virtual function
    virtual void Bind() const = 0;

protected:
    // Give access to derived classes to modify the handle
    inline Handle& GetHandle() { return m_handle; }

#ifndef NDEBUG
    // Check if this Object is currently bound
    virtual bool IsBound() const { return false; }
#endif

protected:
    // We define a constant to represent null handles. In OpenGL we use 0 to represent this
    static const Handle NullHandle = 0;

private:
    // Objects are stored inside OpenGL state machine. Our objects only contain the handle
    Handle m_handle;
};
