#include <ituGL/application/Window.h>

// Create the internal GLFW window. We provide some hints about it to OpenGL
Window::Window(int width, int height, const char* title) : m_window(nullptr)
{
    // Set some hints for window creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

// If we have an internal GLFW window, destroy it
Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
}

// Get the current dimensions (width and height) of the window
void Window::GetDimensions(int& width, int& height) const
{
    glfwGetWindowSize(m_window, &width, &height);
}

// Get the ratio between width and height of the window
float Window::GetAspectRatio() const
{
    int width, height;
    GetDimensions(width, height);
    return static_cast<float>(width) / height;
}

// Tell the window that it should close
void Window::Close()
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

// Get if the window should be closed this frame
bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

// Swaps the front and back buffers of the window
void Window::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

Window::PressedState Window::GetKeyState(int keyCode) const
{
    return static_cast<PressedState>(glfwGetKey(m_window, keyCode));
}

Window::PressedState Window::GetMouseButtonState(MouseButton button) const
{
    return static_cast<PressedState>(glfwGetMouseButton(m_window, static_cast<int>(button)));
}

bool Window::IsMouseVisible() const
{
    return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
}

void Window::SetMouseVisible(bool visible) const
{
    glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

glm::vec2 Window::GetMousePosition(bool normalized) const
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    glm::vec2 mousePosition(static_cast<float>(x), static_cast<float>(y));

    if (normalized)
    {
        int width, height;
        GetDimensions(width, height);
        mousePosition.x = mousePosition.x / width * 2.0f - 1.0f;
        mousePosition.y = mousePosition.y / -height * 2.0f + 1.0f;
    }
    return mousePosition;
}

void Window::SetMousePosition(glm::vec2 mousePosition, bool normalized) const
{
    if (normalized)
    {
        int width, height;
        GetDimensions(width, height);
        mousePosition.x = (mousePosition.x * 0.5f + 0.5f) * width;
        mousePosition.y = (mousePosition.y * 0.5f - 0.5f) * -height;
    }

    glfwSetCursorPos(m_window, mousePosition.x, mousePosition.y);
}
