#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/lighting/PointLight.h>
#include <ituGL/renderer/Renderer.h>
#include <ituGL/utils/DearImGui.h>
#include <vector>

class Texture2DObject;
class Light;

class FirefliesApplication : public Application
{
public:
    FirefliesApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private:
    void InitializeForwardMaterials();
    void InitializeDeferredMaterials();
    void InitializeModels();
    void InitializeCamera();
    void InitializeLights();
    void InitializeRenderer();

    Renderer::UpdateLightsFunction GetUpdateLightsFunction(std::shared_ptr<ShaderProgram> shaderProgramPtr);

    void UpdateFireflies();

    void AddFirefly(glm::vec2 position);

    float Random01();
    float RandomRange(float from, float to);
    Color RandomColor();

    void RenderGUI();

private:
    enum class RenderMode
    {
        Forward,
        Deferred
    };
    RenderMode m_renderMode;

    // Helper object for debug GUI
    DearImGui m_imGui;

    // Camera controller parameters
    Camera m_camera;

    // Default materials
    std::shared_ptr<Material> m_forwardMaterial;
    std::shared_ptr<Material> m_gbufferMaterial;
    std::shared_ptr<Material> m_deferredMaterial;

    // Loaded models
    Model m_floorModel;
    Model m_fireflyModel;

    // Fireflies collection
    struct Firefly
    {
        PointLight pointLight;
        glm::mat4 worldMatrix;
        float rotationSpeed;
    };
    std::vector<Firefly> m_fireflies;

    // True if mouse has been pressed and not released (to avoid creating one firefly each frame)
    bool m_mouseClicked;

    // Add light variables
    glm::vec3 m_ambientColor;
    glm::vec3 m_lightColor;
    float m_lightIntensity;
    bool m_useRandomColor;

    // Renderer
    Renderer m_renderer;
};
