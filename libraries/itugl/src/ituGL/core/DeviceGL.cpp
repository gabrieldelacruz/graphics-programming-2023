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

// Callback called when the framebuffer changes size
void DeviceGL::FrameBufferResized(GLFWwindow* window, GLsizei width, GLsizei height)
{
    if (m_instance)
    {
        // Adjust the viewport when the framebuffer is resized
        m_instance->SetViewport(0, 0, width, height);
    }
}

// Clear the framebuffer with the specified color, depth and stencil
void DeviceGL::Clear(bool clearColor, const Color& color, bool clearDepth, GLdouble depth, bool clearStencil, GLint stencil)
{
    GLbitfield mask = 0;
    if (clearColor)
    {
        glClearColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        mask |= GL_COLOR_BUFFER_BIT;
    }
    if (clearDepth)
    {
        glClearDepth(depth);
        mask |= GL_DEPTH_BUFFER_BIT;
    }
    if (clearStencil)
    {
        glClearStencil(stencil);
        mask |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(mask);
}

// Get if a feature is enabled
bool DeviceGL::IsFeatureEnabled(GLenum feature) const
{
    return glIsEnabled(feature);
}

// enable / disable a feature
void DeviceGL::SetFeatureEnabled(GLenum feature, bool enabled)
{
    if (enabled)
    {
        glEnable(feature);
    }
    else
    {
        glDisable(feature);
    }
}

// enable / disable wireframe mode
void DeviceGL::SetWireframeEnabled(bool enabled)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// enable / disable v-sync
void DeviceGL::SetVSyncEnabled(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}
