#include <ituGL/renderer/Renderer.h>

#include <ituGL/shader/Material.h>
#include <ituGL/geometry/VertexArrayObject.h>
#include <ituGL/geometry/Drawcall.h>
#include <ituGL/geometry/Mesh.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/renderer/RenderPass.h>
#include <span>
#include <algorithm>
#include <cassert>

Renderer::Renderer(DeviceGL& device) : m_device(device), m_currentCamera(nullptr), m_drawcallCollections(1)
{
}

const Camera& Renderer::GetCurrentCamera() const
{
    return *m_currentCamera;
}

void Renderer::SetCurrentCamera(const Camera& camera)
{
    m_currentCamera = &camera;
}

void Renderer::Render()
{
    assert(m_currentCamera);

    for (auto& pass : m_passes)
    {
        pass->Render();
    }

    Reset();
}

void Renderer::Reset()
{
    m_lights.clear();

    for (auto& collection : m_drawcallCollections)
    {
        collection.clear();
    }

    m_currentCamera = nullptr;
}

int Renderer::AddRenderPass(std::unique_ptr<RenderPass> renderPass)
{
    int passIndex = static_cast<int>(m_passes.size());
    renderPass->SetRenderer(this);
    m_passes.push_back(std::move(renderPass));
    // After moving renderPass, the local variable is empty and unusable, pass is now owned by m_passes
    return passIndex;
}

void Renderer::RegisterShaderProgram(std::shared_ptr<const ShaderProgram> shaderProgramPtr,
    const UpdateTransformsFunction& updateTransformFunction,
    const UpdateLightsFunction& updateLightsFunction)
{
    assert(shaderProgramPtr);

    if (updateTransformFunction)
    {
        m_updateTransformsFunctions[shaderProgramPtr] = updateTransformFunction;
    }

    if (updateLightsFunction)
    {
        m_updateLightsFunctions[shaderProgramPtr] = updateLightsFunction;
    }
}

void Renderer::UpdateTransforms(std::shared_ptr<const ShaderProgram> shaderProgramPtr, unsigned int worldMatrixIndex, bool cameraChanged) const
{
    const glm::mat4& worldMatrix = m_worldMatrices[worldMatrixIndex];
    UpdateTransforms(shaderProgramPtr, worldMatrix);
}

void Renderer::UpdateTransforms(std::shared_ptr<const ShaderProgram> shaderProgramPtr, const glm::mat4& worldMatrix, bool cameraChanged) const
{
    const auto& itFind = m_updateTransformsFunctions.find(shaderProgramPtr);
    if (itFind != m_updateTransformsFunctions.end())
    {
        itFind->second(*shaderProgramPtr, worldMatrix, *m_currentCamera, cameraChanged);
    }
}

bool Renderer::UpdateLights(std::shared_ptr<const ShaderProgram> shaderProgramPtr, std::span<const Light* const> lights, unsigned int& lightIndex) const
{
    const auto& itFind = m_updateLightsFunctions.find(shaderProgramPtr);
    if (itFind != m_updateLightsFunctions.end())
    {
        return itFind->second(*shaderProgramPtr, lights, lightIndex);
    }
    return false;
}

std::span<const Light* const> Renderer::GetLights() const
{
    return m_lights;
}

void Renderer::AddLight(const Light* light)
{
    m_lights.push_back(light);
}

std::span<const Renderer::DrawcallInfo> Renderer::GetDrawcalls(unsigned int collectionIndex) const
{
    return m_drawcallCollections[collectionIndex];
}

void Renderer::AddModel(const Model& model, const glm::mat4& worldMatrix)
{
    unsigned int worldMatrixIndex = static_cast<unsigned int>(m_worldMatrices.size());
    m_worldMatrices.push_back(worldMatrix);

    const Mesh& mesh = model.GetMesh();
    for (int submeshIndex = 0; submeshIndex < mesh.GetSubmeshCount(); ++submeshIndex)
    {
        DrawcallInfo drawcallInfo(model.GetMaterial(submeshIndex), worldMatrixIndex,
            mesh.GetSubmeshVertexArray(submeshIndex), mesh.GetSubmeshDrawcall(submeshIndex));

        for (DrawcallCollection& collection : m_drawcallCollections)
        {
            collection.push_back(drawcallInfo);
        }
    }
}

void Renderer::PrepareDrawcall(const DrawcallInfo& drawcallInfo)
{
    std::shared_ptr<const ShaderProgram> shaderProgram = drawcallInfo.material.GetShaderProgram();

    // TODO: Room for optimization here, caching current material, current worldMatrixIndex and current VAO

    // Setup material
    drawcallInfo.material.Use();

    // Setup world matrix
    // Setup camera
    UpdateTransforms(shaderProgram, drawcallInfo.worldMatrixIndex);

    // Setup VAO
    drawcallInfo.vao.Bind();
}

void Renderer::SetLightingRenderStates(bool firstPass)
{
    // Set the render states for the first and additional lights
    m_device.SetFeatureEnabled(GL_BLEND, !firstPass);
    // TODO: This should not be hardcoded here
    glDepthFunc(firstPass ? GL_LESS : GL_EQUAL);
    glBlendFunc(GL_ONE, GL_ONE);
}
