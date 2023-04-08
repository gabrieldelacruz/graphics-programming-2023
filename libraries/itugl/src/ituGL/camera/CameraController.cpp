#include <ituGL/camera/CameraController.h>

#include <ituGL/scene/SceneCamera.h>
#include <ituGL/scene/Transform.h>
#include <ituGL/camera/Camera.h>
#include <ituGL/application/Window.h>
#include <ituGL/utils/DearImGui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

CameraController::CameraController()
    : m_enabled(false), m_enablePressed(false)
    , m_mousePosition(0.0f)
    , m_translationSpeed(2.0f), m_rotationSpeed(2.0f)
{
}

void CameraController::Update(const Window& window, float deltaTime)
{
    if (!m_camera || !m_camera->GetCamera() || !m_camera->GetTransform())
        return;

    UpdateEnabled(window);

    if (IsEnabled())
    {
        UpdateTranslation(window, deltaTime);
        UpdateRotation(window, deltaTime);
        m_camera->MatchCameraToTransform();
    }
}

void CameraController::UpdateEnabled(const Window& window)
{
    bool enablePressed = window.IsKeyPressed(GLFW_KEY_SPACE);
    if (enablePressed && !m_enablePressed)
    {
        m_enabled = !m_enabled;

        window.SetMouseVisible(!m_enabled);
        m_mousePosition = window.GetMousePosition(true);
    }
    m_enablePressed = enablePressed;
}

void CameraController::UpdateTranslation(const Window& window, float deltaTime)
{
    Transform& transform = *m_camera->GetTransform();
    glm::vec3 translation = transform.GetTranslation();

    glm::vec2 inputTranslation(0.0f);

    if (window.IsKeyPressed(GLFW_KEY_A))
        inputTranslation.x = -1.0f;
    else if (window.IsKeyPressed(GLFW_KEY_D))
        inputTranslation.x = 1.0f;

    if (window.IsKeyPressed(GLFW_KEY_W))
        inputTranslation.y = -1.0f;
    else if (window.IsKeyPressed(GLFW_KEY_S))
        inputTranslation.y = 1.0f;

    inputTranslation *= m_translationSpeed;
    inputTranslation *= deltaTime;

    // Double speed if SHIFT is pressed
    if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
        inputTranslation *= 2.0f;

    glm::vec3 right, up, forward;
    m_camera->GetCamera()->ExtractVectors(right, up, forward);
    translation += inputTranslation.x * right;
    translation += inputTranslation.y * forward;

    transform.SetTranslation(translation);
}

void CameraController::UpdateRotation(const Window& window, float deltaTime)
{
    Transform& transform = *m_camera->GetTransform();
    glm::vec3 rotation = transform.GetRotation();

    glm::vec2 mousePosition = window.GetMousePosition(true);
    glm::vec2 deltaMousePosition = mousePosition - m_mousePosition;
    m_mousePosition = mousePosition;

    glm::vec3 inputRotation(deltaMousePosition.y, -deltaMousePosition.x, 0.0f);

    inputRotation *= m_rotationSpeed;

    rotation += inputRotation;

    transform.SetRotation(rotation);
}

void CameraController::DrawGUI(DearImGui& imGui)
{
    if (auto window = imGui.UseWindow("Camera Controller"))
    {
        ImGui::SliderFloat("Translation speed", &m_translationSpeed, 0.0f, 5.0f);
        ImGui::SliderFloat("Rotation speed", &m_rotationSpeed, 0.0f, 5.0f);
    }
}
