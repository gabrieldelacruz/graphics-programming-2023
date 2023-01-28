#include <ituGL/core/DeviceGL.h>

#include <ituGL/application/Window.h>
#include <GLFW/glfw3.h>
#include <cassert>

DeviceGL* DeviceGL::m_instance = nullptr;

DeviceGL::DeviceGL() : m_contextLoaded(false)
{
}

DeviceGL::~DeviceGL()
{
}

// Set the window that OpenGL will use for rendering
void DeviceGL::SetCurrentWindow(Window& window)
{
}

// Set the dimensions of the viewport
void DeviceGL::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
}

// Poll the events in the window event queue
void DeviceGL::PollEvents()
{
}

// Clear the framebuffer with the specified color
void DeviceGL::Clear(float r, float g, float b, float a)
{
}

// Callback called when the framebuffer changes size
void DeviceGL::FrameBufferResized(GLFWwindow* window, GLsizei width, GLsizei height)
{
}
