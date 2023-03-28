#pragma once

#include <ituGL/renderer/RenderPass.h>

#include <ituGL/texture/Texture2DObject.h>
#include <ituGL/texture/FramebufferObject.h>
#include <memory>

class GBufferRenderPass : public RenderPass
{
public:
    GBufferRenderPass(int width, int height, int drawcallCollectionIndex = 0);

    void Render() override;

    const std::shared_ptr<Texture2DObject> GetDepthTexture() const { return m_depthTexture; }
    const std::shared_ptr<Texture2DObject> GetAlbedoTexture() const { return m_albedoTexture; }
    const std::shared_ptr<Texture2DObject> GetNormalTexture() const { return m_normalTexture; }
    const std::shared_ptr<Texture2DObject> GetOthersTexture() const { return m_othersTexture; }

private:
    void InitTextures(int width, int height);
    void InitFramebuffer();

private:
    int m_drawcallCollectionIndex;

    std::shared_ptr<Texture2DObject> m_depthTexture;
    std::shared_ptr<Texture2DObject> m_albedoTexture;
    std::shared_ptr<Texture2DObject> m_normalTexture;
    std::shared_ptr<Texture2DObject> m_othersTexture;

    FramebufferObject m_framebuffer;
};
