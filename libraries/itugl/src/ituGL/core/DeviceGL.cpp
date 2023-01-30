#include <ituGL/core/DeviceGL.h>

#include <ituGL/application/Window.h>
#include <GLFW/glfw3.h>
#include <cassert>

DeviceGL* DeviceGL::m_instance = nullptr;

DeviceGL::DeviceGL() : m_contextLoaded(false)
{
    m_instance = this;

    // Init GLFW
    glfwInit();
}

DeviceGL::~DeviceGL()
{
    m_instance = nullptr;

    // Terminate GLFW
    glfwTerminate();
}

// Set the window that OpenGL will use for rendering
void DeviceGL::SetCurrentWindow(Window& window)
{
    GLFWwindow* glfwWindow = window.GetInternalWindow();
    glfwMakeContextCurrent(glfwWindow);

    // Load required GL libraries and initialize the context
    m_contextLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (m_contextLoaded)
    {
        // Set callback to be called when the window is resized
        glfwSetFramebufferSizeCallback(glfwWindow, FrameBufferResized);
    }
}

// Set the dimensions of the viewport
void DeviceGL::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
}

// Poll the events in the window event queue
void DeviceGL::PollEvents()
{
    glfwPollEvents();
}

// Clear the framebuffer with the specified color
void DeviceGL::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Callback called when the framebuffer changes size
void DeviceGL::FrameBufferResized(GLFWwindow* window, GLsizei width, GLsizei height)
{
    if (m_instance)
    {
        // Adjust the viewport when the framebuffer is resized
        m_instance->SetViewport(0, 0, width, height);
    }
}
