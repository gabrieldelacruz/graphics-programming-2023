#include <ituGL/utils/DearImGui.h>

#include <ituGL/application/Window.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

DearImGui::DearImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

DearImGui::~DearImGui()
{
    ImGui::DestroyContext();
}

void DearImGui::Initialize(::Window& window)
{
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.GetInternalWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

void DearImGui::Cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

void DearImGui::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DearImGui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

DearImGui::Window DearImGui::UseWindow(const char* name)
{
    return name;
}

DearImGui::Window::Window(const char* name) : m_open(false)
{
    m_open = ImGui::Begin(name);
}

DearImGui::Window::~Window()
{
    ImGui::End();
}

DearImGui::Window::operator bool() const
{
    return m_open;
}
