#include <ituGL/renderer/GBufferRenderPass.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/shader/Material.h>
#include <ituGL/geometry/VertexArrayObject.h>
#include <ituGL/renderer/Renderer.h>
#include <ituGL/texture/Texture2DObject.h>
#include <ituGL/texture/FramebufferObject.h>

GBufferRenderPass::GBufferRenderPass(int width, int height, int drawcallCollectionIndex)
    : m_drawcallCollectionIndex(drawcallCollectionIndex)
{
    InitTextures(width, height);
    InitFramebuffer();
}

void GBufferRenderPass::InitFramebuffer()
{
    std::shared_ptr<FramebufferObject> targetFramebuffer = std::make_shared<FramebufferObject>();

    targetFramebuffer->Bind();

    targetFramebuffer->SetTexture(FramebufferObject::Target::Draw, FramebufferObject::Attachment::Depth, *m_depthTexture);

    // Set the albedo texture as color attachment 0
    targetFramebuffer->SetTexture(FramebufferObject::Target::Draw, FramebufferObject::Attachment::Color0, *m_albedoTexture);

    // Set the normal texture as color attachment 1
    targetFramebuffer->SetTexture(FramebufferObject::Target::Draw, FramebufferObject::Attachment::Color1, *m_normalTexture);

    // Set the others texture as color attachment 2
    targetFramebuffer->SetTexture(FramebufferObject::Target::Draw, FramebufferObject::Attachment::Color2, *m_othersTexture);

    // Set the draw buffers used by the framebuffer (all attachments except depth)
    targetFramebuffer->SetDrawBuffers(std::array<FramebufferObject::Attachment, 3>(
        {
            FramebufferObject::Attachment::Color0,
            FramebufferObject::Attachment::Color1,
            FramebufferObject::Attachment::Color2
        }));

    m_targetFramebuffer = targetFramebuffer;

    FramebufferObject::Unbind();
}

void GBufferRenderPass::InitTextures(int width, int height)
{
    // Depth: Set the min and magfilter as nearest
    m_depthTexture = std::make_shared<Texture2DObject>();
    m_depthTexture->Bind();
    m_depthTexture->SetImage(0, width, height, TextureObject::FormatDepth, TextureObject::InternalFormatDepth);
    m_depthTexture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_NEAREST);
    m_depthTexture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_NEAREST);

    // Albedo: Bind the newly created texture, set the image, and the min and magfilter as nearest
    m_albedoTexture = std::make_shared<Texture2DObject>();
    m_albedoTexture->Bind();
    m_albedoTexture->SetImage(0, width, height, TextureObject::FormatRGBA, TextureObject::InternalFormatSRGBA8);
    m_albedoTexture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_NEAREST);
    m_albedoTexture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_NEAREST);

    // Normal: Bind the newly created texture, set the image and the min and magfilter as nearest
    m_normalTexture = std::make_shared<Texture2DObject>();
    m_normalTexture->Bind();
    m_normalTexture->SetImage(0, width, height, TextureObject::FormatRG, TextureObject::InternalFormatRG16F);
    m_normalTexture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_NEAREST);
    m_normalTexture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_NEAREST);

    // Others: Bind the newly created texture, set the image and the min and magfilter as nearest
    m_othersTexture = std::make_shared<Texture2DObject>();
    m_othersTexture->Bind();
    m_othersTexture->SetImage(0, width, height, TextureObject::FormatRGBA, TextureObject::InternalFormatSRGBA8);
    m_othersTexture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_NEAREST);
    m_othersTexture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_NEAREST);

    Texture2DObject::Unbind();
}

void GBufferRenderPass::Render()
{
    Renderer& renderer = GetRenderer();

    const Camera& camera = renderer.GetCurrentCamera();
    const auto& lights = renderer.GetLights();
    const auto& drawcallCollection = renderer.GetDrawcalls(m_drawcallCollectionIndex);

    renderer.GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    bool wasSRGB = renderer.GetDevice().IsFeatureEnabled(GL_FRAMEBUFFER_SRGB);
    renderer.GetDevice().EnableFeature(GL_FRAMEBUFFER_SRGB);

    // for all drawcalls
    for (const Renderer::DrawcallInfo& drawcallInfo : drawcallCollection)
    {
        assert(drawcallInfo.material.GetBlendEquationColor() == Material::BlendEquation::None);
        assert(drawcallInfo.material.GetBlendEquationAlpha() == Material::BlendEquation::None);
        assert(drawcallInfo.material.GetDepthWrite());

        // Prepare drawcall (similar to forward)
        renderer.PrepareDrawcall(drawcallInfo);

        // Render drawcall
        drawcallInfo.drawcall.Draw();
    }

    renderer.GetDevice().SetFeatureEnabled(GL_FRAMEBUFFER_SRGB, wasSRGB);
}
