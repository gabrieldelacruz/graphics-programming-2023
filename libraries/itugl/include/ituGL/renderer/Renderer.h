#pragma once

#include <ituGL/core/DeviceGL.h>
#include <ituGL/renderer/RenderPass.h>
#include <ituGL/geometry/Drawcall.h>
#include <ituGL/geometry/Mesh.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <span>
#include <functional>

class Camera;
class Light;
class ShaderProgram;
class Material;
class VertexArrayObject;
class Drawcall;
class Model;

class Renderer
{
public:
    struct DrawcallInfo
    {
        DrawcallInfo(const Material& material, unsigned int worldMatrixIndex, const VertexArrayObject& vao, const Drawcall& drawcall)
            : material(material), worldMatrixIndex(worldMatrixIndex), vao(vao), drawcall(drawcall)
        {
        }

        const Material& material;
        unsigned int worldMatrixIndex;
        const VertexArrayObject& vao;
        const Drawcall& drawcall;
    };

    using DrawcallCollection = std::vector<DrawcallInfo>;

    using UpdateTransformsFunction = std::function<void(const ShaderProgram&, const glm::mat4&, const Camera&, bool)>;
    using UpdateLightsFunction = std::function<bool(const ShaderProgram&, std::span<const Light* const>, unsigned int&)>;

public:
    Renderer(DeviceGL& device);

    const DeviceGL& GetDevice() const { return m_device; }
    DeviceGL& GetDevice() { return m_device; }

    int AddRenderPass(std::unique_ptr<RenderPass> renderPass);

    bool HasCamera() const;
    const Camera& GetCurrentCamera() const;
    void SetCurrentCamera(const Camera& camera);

    std::span<const Light* const> GetLights() const;
    void AddLight(const Light& light);

    std::span<const DrawcallInfo> GetDrawcalls(unsigned int collectionIndex) const;
    void AddModel(const Model& model, const glm::mat4& worldMatrix);

    const Mesh& GetFullscreenMesh() const;

    void RegisterShaderProgram(std::shared_ptr<const ShaderProgram> shaderProgramPtr,
        const UpdateTransformsFunction& updateTransformFunction,
        const UpdateLightsFunction& updateLightsFunction);

    void UpdateTransforms(std::shared_ptr<const ShaderProgram> shaderProgramPtr, const glm::mat4& worldMatrix, bool cameraChanged = true) const;
    void UpdateTransforms(std::shared_ptr<const ShaderProgram> shaderProgramPtr, unsigned int worldMatrixIndex, bool cameraChanged = true) const;

    UpdateLightsFunction GetDefaultUpdateLightsFunction(const ShaderProgram& shaderProgram);
    bool UpdateLights(std::shared_ptr<const ShaderProgram> shaderProgramPtr, std::span<const Light* const> lights, unsigned int& lightIndex) const;

    void PrepareDrawcall(const DrawcallInfo& drawcallInfo);

    void SetLightingRenderStates(bool firstPass);

    void Render();

private:
    void Reset();

    void InitializeFullscreenMesh();

private:
    DeviceGL& m_device;

    const Camera *m_currentCamera;

    std::shared_ptr<const Material> m_currentMaterial;

    std::vector<const Light*> m_lights;

    std::vector<glm::mat4> m_worldMatrices;

    std::vector<DrawcallCollection> m_drawcallCollections;

    std::unordered_map<std::shared_ptr<const ShaderProgram>, UpdateTransformsFunction> m_updateTransformsFunctions;
    std::unordered_map<std::shared_ptr<const ShaderProgram>, UpdateLightsFunction> m_updateLightsFunctions;

    Mesh m_fullscreenMesh;

    std::vector<std::unique_ptr<RenderPass>> m_passes;
};
