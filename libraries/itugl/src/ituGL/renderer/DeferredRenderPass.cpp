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

        const Mesh* mesh = &m_fullscreenMesh;
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
    VertexFormat vertexFormat;
    vertexFormat.AddVertexAttribute<float>(3, VertexAttribute::Semantic::Position);

    // Large triangle covering the entire screen
    std::vector<glm::vec3> fullscreenVertices;
    fullscreenVertices.emplace_back(-1.0f, -1.0f, 0.0f);
    fullscreenVertices.emplace_back( 3.0f, -1.0f, 0.0f);
    fullscreenVertices.emplace_back(-1.0f,  3.0f, 0.0f);
    m_fullscreenMesh.AddSubmesh<glm::vec3, VertexFormat::LayoutIterator>(Drawcall::Primitive::Triangles, fullscreenVertices, vertexFormat.LayoutBegin(3, false), vertexFormat.LayoutEnd());
}
