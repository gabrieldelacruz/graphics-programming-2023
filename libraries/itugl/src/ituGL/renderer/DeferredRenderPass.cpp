#include <ituGL/renderer/DeferredRenderPass.h>

#include <ituGL/renderer/Renderer.h>
#include <ituGL/geometry/VertexFormat.h>
#include <ituGL/lighting/Light.h>
#include <ituGL/camera/Camera.h>
#include <ituGL/shader/Material.h>
#include <ituGL/texture/Texture2DObject.h>
#include <glm/gtx/transform.hpp>

DeferredRenderPass::DeferredRenderPass(std::shared_ptr<Material> material)
    : m_material(material)
{
    InitializeMeshes();
}

void DeferredRenderPass::Render()
{
    Renderer& renderer = GetRenderer();

    const Camera& camera = renderer.GetCurrentCamera();

    assert(m_material);
    m_material->Use();
    std::shared_ptr<const ShaderProgram> shaderProgram = m_material->GetShaderProgram();

    // Our fullscreen triangle is directly in clip coordinates.
    // Use the inverse view proj matrix to cancel view projection from the camera
    glm::mat4 fullscreenMatrix = glm::inverse(camera.GetViewProjectionMatrix());

    bool first = true;
    unsigned int lightIndex = 0;
    const auto& lights = renderer.GetLights();
    while (renderer.UpdateLights(shaderProgram, lights, lightIndex))
    {
        const Light* light = lightIndex <= lights.size() ? lights[lightIndex - 1] : nullptr;
        assert(first || light);

        const Mesh* mesh = &renderer.GetFullscreenMesh();
        glm::mat4 worldMatrix = fullscreenMatrix;

        // TODO: select different meshes depending on the type

        // Set the render states for the first and additional lights
        renderer.SetLightingRenderStates(first);

        renderer.UpdateTransforms(shaderProgram, fullscreenMatrix, first);
        mesh->DrawSubmesh(0);
        first = false;
    }
}

void DeferredRenderPass::InitializeMeshes()
{
    //TODO: Create meshes for spot and point lights
}
