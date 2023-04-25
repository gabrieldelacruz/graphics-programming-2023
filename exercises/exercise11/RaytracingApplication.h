#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/renderer/Renderer.h>
#include <ituGL/camera/CameraController.h>
#include <ituGL/utils/DearImGui.h>

class Material;
class Texture2DObject;
class FramebufferObject;

class RaytracingApplication : public Application
{
public:
    RaytracingApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private:
    void InitializeCamera();
    void InitializeMaterial();
    void InitializeFramebuffer();
    void InitializeRenderer();

    std::shared_ptr<Material> CreateCopyMaterial();
    std::shared_ptr<Material> CreateRaytracingMaterial(const char* fragmentShaderPath);

    void InvalidateScene();

    void RenderGUI();

private:
    // Helper object for debug GUI
    DearImGui m_imGui;

    // Frame counter
    unsigned int m_frameCount;

    // World position for sphere center
    glm::vec3 m_sphereCenter;

    // World matrix for cube
    glm::mat4 m_boxMatrix;

    // Camera controller
    CameraController m_cameraController;

    // Renderer
    Renderer m_renderer;

    // Materials
    std::shared_ptr<Material> m_material;

    // Framebuffer
    std::shared_ptr<Texture2DObject> m_sceneTexture;
    std::shared_ptr<FramebufferObject> m_sceneFramebuffer;
};
