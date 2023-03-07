#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    // (C++) 1
    // Get the GLFWwindow object encapsulated by this object
    inline const GLFWwindow* GetInternalWindow() const { return m_window; }
    inline GLFWwindow* GetInternalWindow() { return m_window; }

    // A window is valid only if the internal window is valid
    inline bool IsValid() const { return m_window != nullptr; }

    // Get the current dimensions (width and height) of the window
    void GetDimensions(int& width, int& height) const;

    // Get the ratio between width and height of the window
    float GetAspectRatio() const;

    // Tell the window that it should close
    void Close();

    // Get if the window should be closed this frame
    bool ShouldClose() const;

    // Swaps the front and back buffers of the window
    void SwapBuffers();

public:
    // Pressed state of a button or key
    enum class PressedState
    {
        Pressed = GLFW_PRESS,
        Released = GLFW_RELEASE,
        Repeated = GLFW_REPEAT,
    };

    // Get the pressed state of a key
    PressedState GetKeyState(int keyCode) const;

    // Helper functions to query the pressed state of a key
    bool IsKeyPressed(int keyCode) const { return GetKeyState(keyCode) == PressedState::Pressed; }
    bool IsKeyReleased(int keyCode) const { return GetKeyState(keyCode) == PressedState::Released; }
    bool IsKeyRepeated(int keyCode) const { return GetKeyState(keyCode) == PressedState::Repeated; }

    // Main buttons of the mouse
    enum class MouseButton
    {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    };

    // Get the pressed state of a mouse button
    PressedState GetMouseButtonState(MouseButton button) const;

    // Helper functions to query the pressed state of a mouse button
    bool IsMouseButtonPressed(MouseButton button) const { return GetMouseButtonState(button) == PressedState::Pressed; }
    bool IsMouseButtonReleased(MouseButton button) const { return GetMouseButtonState(button) == PressedState::Released; }

    // Get if the mouse is visible
    bool IsMouseVisible() const;
    // Set if the mouse is visible
    void SetMouseVisible(bool visible) const;

    // Get the mouse position, in pixels or in NDC coordinates
    glm::vec2 GetMousePosition(bool normalized = false) const;
    // Set the mouse position, in pixels or in NDC coordinates
    void SetMousePosition(glm::vec2 mousePosition, bool normalized = false) const;


private:
    // Pointer to a GLFW window object. Its lifetime should match the lifetime of this object
    GLFWwindow* m_window;
};
