#pragma once

#include <glm/vec2.hpp>
#include <memory>

class SceneCamera;
class Window;
class DearImGui;

class CameraController
{
public:
    CameraController();

    inline bool IsEnabled() const { return m_enabled; }
    inline void SetEnabled(bool enabled) { m_enabled = enabled; }

    inline std::shared_ptr<SceneCamera> GetCamera() { return m_camera; }
    inline std::shared_ptr<const SceneCamera> GetCamera() const { return m_camera; }
    inline void SetCamera(std::shared_ptr<SceneCamera> camera) { m_camera = camera; }

    void Update(const Window& window, float deltaTime);

    void DrawGUI(DearImGui& imGui);

private:
    void UpdateEnabled(const Window& window);
    void UpdateTranslation(const Window& window, float deltaTime);
    void UpdateRotation(const Window& window, float deltaTime);

private:
    bool m_enabled;
    bool m_enablePressed;
    std::shared_ptr<SceneCamera> m_camera;
    glm::vec2 m_mousePosition;
    float m_translationSpeed;
    float m_rotationSpeed;
};
