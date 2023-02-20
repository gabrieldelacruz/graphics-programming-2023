#include <ituGL/core/Object.h>

// The constructor only assigns the handle
Object::Object(Handle handle) : m_handle(handle)
{
}

// The destructor is empty, we only need it to define it as virtual and ensure it gets invoked in derived classes
Object::~Object()
{
}

Object::Object(Object&& object) noexcept : m_handle(object.m_handle)
{
    object.m_handle = NullHandle;
}

Object& Object::operator = (Object&& object) noexcept
{
    this->~Object();
    m_handle = object.m_handle;
    object.m_handle = NullHandle;
    return *this;
}
