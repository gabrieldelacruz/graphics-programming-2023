#include <ituGL/texture/FramebufferObject.h>

#include <ituGL/texture/Texture2DObject.h>
#include <cassert>

FramebufferObject::FramebufferObject() : Object(NullHandle)
{
    Handle& handle = GetHandle();
    glGenFramebuffers(1, &handle);
}

FramebufferObject::~FramebufferObject()
{
    Handle& handle = GetHandle();
    glDeleteFramebuffers(1, &handle);
}

void FramebufferObject::Bind() const
{
    Bind(Target::Both);
}

void FramebufferObject::Bind(Target target) const
{
    Handle handle = GetHandle();
    glBindFramebuffer(static_cast<GLenum>(target), handle);
}

void FramebufferObject::Unbind()
{
    Unbind(Target::Both);
}

void FramebufferObject::Unbind(Target target)
{
    Handle handle = NullHandle;
    glBindFramebuffer(static_cast<GLenum>(target), handle);
}

void FramebufferObject::SetTexture(Target target, Attachment attachment, const Texture2DObject& texture, int level)
{
    glFramebufferTexture2D(static_cast<GLenum>(target), static_cast<GLenum>(attachment), texture.GetTarget(), texture.GetHandle(), level);
}

void FramebufferObject::SetDrawBuffers(std::span<const Attachment> attachments)
{
    glDrawBuffers(attachments.size(), reinterpret_cast<const GLenum*>(attachments.data()));
}
