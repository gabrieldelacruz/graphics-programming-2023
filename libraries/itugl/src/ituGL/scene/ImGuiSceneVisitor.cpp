#include <ituGL/scene/ImGuiSceneVisitor.h>

#include <ituGL/utils/DearImGui.h>
#include <ituGL/scene/SceneCamera.h>
#include <ituGL/scene/SceneLight.h>
#include <ituGL/scene/SceneModel.h>
#include <ituGL/scene/Transform.h>
#include <ituGL/lighting/Light.h>
#include <imgui.h>

ImGuiSceneVisitor::ImGuiSceneVisitor(DearImGui& imGui, const char* windowName) : m_imGui(imGui), m_windowName(windowName)
{
}

void ImGuiSceneVisitor::VisitCamera(SceneCamera& sceneCamera)
{
    if (auto window = m_imGui.UseWindow(m_windowName.c_str()))
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        if (ImGui::CollapsingHeader(sceneCamera.GetName().c_str()))
        {
            ImGui::PushID(&sceneCamera);
            ImGui::Indent();

            VisitTransform(*sceneCamera.GetTransform());
            if (sceneCamera.GetTransform()->IsDirty())
            {
                sceneCamera.MatchCameraToTransform();
            }

            ImGui::Separator();

            ImGui::Text("Camera stats");

            ImGui::Unindent();
            ImGui::PopID();
        }
        ImGui::PopStyleColor();
    }
}

void ImGuiSceneVisitor::VisitLight(SceneLight& sceneLight)
{
    if (auto window = m_imGui.UseWindow(m_windowName.c_str()))
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.1f, 1.0f));
        if (ImGui::CollapsingHeader(sceneLight.GetName().c_str()))
        {
            ImGui::PushID(&sceneLight);
            ImGui::Indent();

            VisitTransform(*sceneLight.GetTransform());
            if (sceneLight.GetTransform()->IsDirty())
            {
                sceneLight.MatchLightToTransform();
            }

            ImGui::Separator();

            Light& light = *sceneLight.GetLight();

            glm::vec3 color = light.GetColor();
            if (ImGui::ColorEdit3("Color", &color[0]))
                light.SetColor(color);

            float intensity = light.GetIntensity();
            if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100000.0f))
                light.SetIntensity(intensity);

            ImGui::Unindent();
            ImGui::PopID();
        }
        ImGui::PopStyleColor();
    }
}

void ImGuiSceneVisitor::VisitModel(SceneModel& sceneModel)
{
    if (auto window = m_imGui.UseWindow(m_windowName.c_str()))
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.5f, 1.0f));
        if (ImGui::CollapsingHeader(sceneModel.GetName().c_str()))
        {
            ImGui::PushID(&sceneModel);
            ImGui::Indent();

            VisitTransform(*sceneModel.GetTransform());
            ImGui::Separator();

            ImGui::Text("Model stats");
            ImGui::Unindent();
            ImGui::PopID();
        }
        ImGui::PopStyleColor();
    }
}

void ImGuiSceneVisitor::VisitTransform(Transform& transform)
{
    glm::vec3 translation = transform.GetTranslation();
    glm::vec3 rotation = transform.GetRotation();
    glm::vec3 scale = transform.GetScale();

    if (ImGui::DragFloat3("Translation", &translation[0], 0.1f))
    {
        transform.SetTranslation(translation);
    }
    if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f))
    {
        transform.SetRotation(rotation);
    }
    if (ImGui::DragFloat3("Scale", &scale[0], 0.1f, 0.0f, 100000.0f))
    {
        transform.SetScale(scale);
    }
}
