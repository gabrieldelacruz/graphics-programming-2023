#include <ituGL/renderer/ShadowMapRenderPass.h>

#include <ituGL/renderer/Renderer.h>
#include <ituGL/lighting/Light.h>
#include <ituGL/camera/Camera.h>
#include <ituGL/shader/Material.h>
#include <ituGL/texture/Texture2DObject.h>
#include <ituGL/texture/FramebufferObject.h>

ShadowMapRenderPass::ShadowMapRenderPass(std::shared_ptr<Light> light, std::shared_ptr<const Material> material, int drawcallCollectionIndex)
    : m_light(light)
    , m_material(material)
    , m_drawcallCollectionIndex(drawcallCollectionIndex)
    , m_volumeCenter(0.0f)
    , m_volumeSize(1.0f)
{
    InitFramebuffer();
}

void ShadowMapRenderPass::SetVolume(glm::vec3 volumeCenter, glm::vec3 volumeSize)
{
    m_volumeCenter = volumeCenter;
    m_volumeSize = volumeSize;
}

void ShadowMapRenderPass::InitFramebuffer()
{
    std::shared_ptr<FramebufferObject> targetFramebuffer = std::make_shared<FramebufferObject>();

    targetFramebuffer->Bind();

    std::shared_ptr<const TextureObject> shadowMap = m_light->GetShadowMap();
    assert(shadowMap);
    targetFramebuffer->SetTexture(FramebufferObject::Target::Draw, FramebufferObject::Attachment::Depth, *shadowMap);

    FramebufferObject::Unbind();

    m_targetFramebuffer = targetFramebuffer;
}

void ShadowMapRenderPass::Render()
{
    Renderer& renderer = GetRenderer();
    DeviceGL& device = renderer.GetDevice();

    const auto& drawcallCollection = renderer.GetDrawcalls(m_drawcallCollectionIndex);

    device.Clear(false, Color(), true, 1.0f);

    // Use shadow map shader
    m_material->Use();
    std::shared_ptr<const ShaderProgram> shaderProgram = m_material->GetShaderProgram();

    // Backup current viewport
    glm::ivec4 currentViewport;
    device.GetViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);

    // Set viewport to texture size
    device.SetViewport(0, 0, 512, 512);

    // Backup current camera
    const Camera& currentCamera = renderer.GetCurrentCamera();

    // Set up light as the camera
    Camera lightCamera;
    InitLightCamera(lightCamera);
    renderer.SetCurrentCamera(lightCamera);

    // for all drawcalls
    bool first = true;
    for (const Renderer::DrawcallInfo& drawcallInfo : drawcallCollection)
    {
        // Bind the vao
        drawcallInfo.vao.Bind();

        // Set up object matrix
        renderer.UpdateTransforms(shaderProgram, drawcallInfo.worldMatrixIndex, first);

        // Render drawcall
        drawcallInfo.drawcall.Draw();

        first = false;
    }

    m_light->SetShadowMatrix(lightCamera.GetViewProjectionMatrix());

    // Restore viewport
    renderer.GetDevice().SetViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);

    // Restore current camera
    renderer.SetCurrentCamera(currentCamera);

    // Restore default framebuffer to avoid drawing to the shadow map
    renderer.SetCurrentFramebuffer(renderer.GetDefaultFramebuffer());
}

void ShadowMapRenderPass::InitLightCamera(Camera& lightCamera) const
{
    // View matrix
    glm::vec3 position = m_light->GetPosition(m_volumeCenter);
    lightCamera.SetViewMatrix(position, position + m_light->GetDirection());

    // Projection matrix
    glm::vec4 attenuation = m_light->GetAttenuation();
    switch (m_light->GetType())
    {
    case Light::Type::Directional:
        lightCamera.SetOrthographicProjectionMatrix(-0.5f * m_volumeSize, 0.5f * m_volumeSize);
        break;
    case Light::Type::Spot:
        lightCamera.SetPerspectiveProjectionMatrix(attenuation.w, 1.0f, 0.01f, attenuation.y);
        break;
    default:
        assert(false);
        break;
    }
}
