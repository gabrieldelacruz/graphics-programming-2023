#include <ituGL/renderer/Renderer.h>

#include <ituGL/shader/Material.h>
#include <ituGL/geometry/VertexFormat.h>
#include <ituGL/geometry/VertexArrayObject.h>
#include <ituGL/geometry/Drawcall.h>
#include <ituGL/geometry/Mesh.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/lighting/Light.h>
#include <ituGL/renderer/RenderPass.h>
#include <span>
#include <algorithm>
#include <cassert>

Renderer::Renderer(DeviceGL& device) : m_device(device), m_currentCamera(nullptr), m_drawcallCollections(1)
{
    InitializeFullscreenMesh();

    device.EnableFeature(GL_FRAMEBUFFER_SRGB);
    device.EnableFeature(GL_DEPTH_TEST);
    device.EnableFeature(GL_CULL_FACE);
    device.EnableFeature(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    device.SetVSyncEnabled(true);
}

bool Renderer::HasCamera() const
{
    return m_currentCamera;
}

const Camera& Renderer::GetCurrentCamera() const
{
    return *m_currentCamera;
}

void Renderer::SetCurrentCamera(const Camera& camera)
{
    m_currentCamera = &camera;
}

const Mesh& Renderer::GetFullscreenMesh() const
{
    return m_fullscreenMesh;
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

Renderer::UpdateLightsFunction Renderer::GetDefaultUpdateLightsFunction(const ShaderProgram& shaderProgram)
{
    // Get lighting related uniform locations
    ShaderProgram::Location lightIndirectLocation = shaderProgram.GetUniformLocation("LightIndirect");
    ShaderProgram::Location lightColorLocation = shaderProgram.GetUniformLocation("LightColor");
    ShaderProgram::Location lightPositionLocation = shaderProgram.GetUniformLocation("LightPosition");
    ShaderProgram::Location lightDirectionLocation = shaderProgram.GetUniformLocation("LightDirection");
    ShaderProgram::Location lightAttenuationLocation = shaderProgram.GetUniformLocation("LightAttenuation");

    return [=](const ShaderProgram& shaderProgram, std::span<const Light* const> lights, unsigned int& lightIndex) -> bool
    {
        bool needsRender = lightIndex == 0;

        shaderProgram.SetUniform(lightIndirectLocation, lightIndex == 0 ? 1 : 0);

        if (lightIndex < lights.size())
        {
            const Light& light = *lights[lightIndex];
            shaderProgram.SetUniform(lightColorLocation, light.GetColor() * light.GetIntensity());
            shaderProgram.SetUniform(lightPositionLocation, light.GetPosition());
            shaderProgram.SetUniform(lightDirectionLocation, light.GetDirection());
            shaderProgram.SetUniform(lightAttenuationLocation, light.GetAttenuation());
            needsRender = true;
        }
        else
        {
            // Disable light
            shaderProgram.SetUniform(lightColorLocation, glm::vec3(0.0f));
        }

        lightIndex++;

        return needsRender;
    };
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

void Renderer::AddLight(const Light& light)
{
    m_lights.push_back(&light);
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

void Renderer::InitializeFullscreenMesh()
{
    VertexFormat vertexFormat;
    vertexFormat.AddVertexAttribute<float>(3, VertexAttribute::Semantic::Position);

    // Large triangle covering the entire screen
    std::vector<glm::vec3> fullscreenVertices;
    fullscreenVertices.emplace_back(-1.0f, -1.0f, 0.0f);
    fullscreenVertices.emplace_back(3.0f, -1.0f, 0.0f);
    fullscreenVertices.emplace_back(-1.0f, 3.0f, 0.0f);
    m_fullscreenMesh.AddSubmesh<glm::vec3, VertexFormat::LayoutIterator>(Drawcall::Primitive::Triangles, fullscreenVertices, vertexFormat.LayoutBegin(3, false), vertexFormat.LayoutEnd());
}
