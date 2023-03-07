#pragma once

class Window;

class DearImGui
{
public:
    class Window
    {
    public:
        ~Window();
        operator bool() const;
    private:
        friend DearImGui;
        Window(const char* name);
    private:
        bool m_open;
    };

public:
    DearImGui();
    ~DearImGui();

    void Initialize(::Window& window);
    void Cleanup();

    void BeginFrame();
    void EndFrame();

    Window UseWindow(const char* name);
};
