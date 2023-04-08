#pragma once

#include <ituGL/scene/SceneVisitor.h>
#include <string>

class DearImGui;
class SceneCamera;
class SceneLight;
class SceneModel;
class Transform;

class ImGuiSceneVisitor : public SceneVisitor
{
public:
    ImGuiSceneVisitor(DearImGui& imGui, const char *windowName);

    void VisitCamera(SceneCamera& sceneCamera) override;

    void VisitLight(SceneLight& sceneLight) override;

    void VisitModel(SceneModel& sceneModel) override;

private:
    void VisitTransform(Transform& transform);

private:
    DearImGui& m_imGui;
    std::string m_windowName;
};
