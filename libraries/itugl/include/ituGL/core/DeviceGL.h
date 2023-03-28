#pragma once

#include <ituGL/core/Color.h>
#include <glad/glad.h>

class Window;
struct GLFWwindow;

// Class that represent the device where we run OpenGL
// Implemented as a Singleton pattern, as there can only be one
class DeviceGL
{
public:
    DeviceGL();
    ~DeviceGL();

    // Singleton method to get a reference to the instance. Will crash if there is none
    inline static DeviceGL& GetInstance() { return *m_instance; }

    // Singleton method to get a pointer to the instance
    inline static DeviceGL* GetInstancePointer() { return m_instance; }

    // Check if device is initialized
    inline bool IsReady() const { return m_contextLoaded; }

    // Set the window that OpenGL will use for rendering
    void SetCurrentWindow(Window &window);

    // Set the dimensions of the viewport
    void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);

    // Poll the events in the window event queue
    void PollEvents();

    // Clear the framebuffer with the specified color
    inline void Clear(const Color& color) { Clear(true, color, false, 0.0, false, 0); }
    // Clear the framebuffer with the specified color and depth
    inline void Clear(bool clearColor, const Color& color, bool clearDepth, GLdouble depth) { Clear(clearColor, color, clearDepth, depth, false, 0); }
    // Clear the framebuffer with the specified color, depth and stencil
    void Clear(bool clearColor, const Color& color, bool clearDepth, GLdouble depth, bool clearStencil, GLint stencil);

    // Get if a feature is enabled
    bool IsFeatureEnabled(GLenum feature) const;
    // enable / disable a feature
    void SetFeatureEnabled(GLenum feature, bool enabled);
    inline void EnableFeature(GLenum feature) { SetFeatureEnabled(feature, true); }
    inline void DisableFeature(GLenum feature) { SetFeatureEnabled(feature, false); }

    // enable / disable wireframe mode
    void SetWireframeEnabled(bool enabled);

    // enable / disable v-sync
    void SetVSyncEnabled(bool enabled);

private:
    // Has a context been loaded? We use the context of the current window
    bool m_contextLoaded;

private:
    // Singleton instance
    static DeviceGL* m_instance;

    // Callback called when the framebuffer changes size
    static void FrameBufferResized(GLFWwindow* window, GLsizei width, GLsizei height);
};
