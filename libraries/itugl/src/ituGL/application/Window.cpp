#include <ituGL/application/Window.h>

// Create the internal GLFW window. We provide some hints about it to OpenGL
Window::Window(int width, int height, const char* title) : m_window(nullptr)
{
}

// If we have an internal GLFW window, destroy it
Window::~Window()
{
}

// Get the current dimensions (width and height) of the window
void Window::GetDimensions(int& width, int& height) const
{
}

// Tell the window that it should close
void Window::Close()
{
}

// Get if the window should be closed this frame
bool Window::ShouldClose() const
{
    return false;
}

// Swaps the front and back buffers of the window
void Window::SwapBuffers()
{
}
