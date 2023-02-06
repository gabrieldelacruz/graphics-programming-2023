#pragma once

#include <GLFW/glfw3.h>

struct GLFWwindow;

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

    // Tell the window that it should close
    void Close();

    // Get if the window should be closed this frame
    bool ShouldClose() const;

    // Swaps the front and back buffers of the window
    void SwapBuffers();

public:
    enum class KeyState
    {
        Pressed = GLFW_PRESS,
        Released = GLFW_RELEASE,
        Repeated = GLFW_REPEAT,
    };

    KeyState GetKeyState(int keyCode) const;
    bool IsKeyPressed(int keyCode) const { return GetKeyState(keyCode) == KeyState::Pressed; }
    bool IsKeyReleased(int keyCode) const { return GetKeyState(keyCode) == KeyState::Released; }
    bool IsKeyRepeated(int keyCode) const { return GetKeyState(keyCode) == KeyState::Repeated; }

private:
    // Pointer to a GLFW window object. Its lifetime should match the lifetime of this object
    GLFWwindow* m_window;
};
