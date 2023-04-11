#pragma once

#include <ituGL/renderer/RenderPass.h>

#include <memory>

class Material;
class FramebufferObject;

class PostFXRenderPass: public RenderPass
{
public:
    PostFXRenderPass(std::shared_ptr<Material> material, std::shared_ptr<const FramebufferObject> targetFramebuffer = nullptr);

    void Render() override;

private:
    std::shared_ptr<Material> m_material;
    std::shared_ptr<FramebufferObject> m_framebuffer;
};
