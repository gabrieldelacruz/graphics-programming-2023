#pragma once

#include <ituGL/core/Object.h>
#include <span>

class Texture2DObject;

// Abstract OpenGL object that encapsulates a Framebuffer
class FramebufferObject : public Object
{
public:
    enum class Target : GLenum;
    enum class Attachment : GLenum;

public:
    FramebufferObject();
    virtual ~FramebufferObject();

    // (C++) 8
    // Move semantics
    FramebufferObject(FramebufferObject&&) = default;
    FramebufferObject& operator = (FramebufferObject&&) = default;

    void Bind() const override;
    void Bind(Target target) const;
    static void Unbind();
    static void Unbind(Target target);

    void SetTexture(Target target, Attachment attachment, const Texture2DObject& texture, int level = 0);

    void SetDrawBuffers(std::span<const Attachment> attachments);

private:

};

enum class FramebufferObject::Target : GLenum
{
    Read = GL_READ_FRAMEBUFFER,
    Draw = GL_DRAW_FRAMEBUFFER,
    Both = GL_FRAMEBUFFER,
};

enum class FramebufferObject::Attachment : GLenum
{
    Depth = GL_DEPTH_ATTACHMENT,
    Color0 = GL_COLOR_ATTACHMENT0,
    Color1 = GL_COLOR_ATTACHMENT1,
    Color2 = GL_COLOR_ATTACHMENT2,
    Color3 = GL_COLOR_ATTACHMENT3,
    Color4 = GL_COLOR_ATTACHMENT4,
    Color5 = GL_COLOR_ATTACHMENT5,
    Color6 = GL_COLOR_ATTACHMENT6,
    Color7 = GL_COLOR_ATTACHMENT7,
};
