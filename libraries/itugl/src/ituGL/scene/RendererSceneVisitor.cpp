#include <ituGL/scene/RendererSceneVisitor.h>

#include <ituGL/renderer/Renderer.h>
#include <ituGL/scene/SceneCamera.h>
#include <ituGL/scene/SceneLight.h>
#include <ituGL/scene/SceneModel.h>
#include <ituGL/scene/Transform.h>

RendererSceneVisitor::RendererSceneVisitor(Renderer& renderer) : m_renderer(renderer)
{
}

void RendererSceneVisitor::VisitCamera(SceneCamera& sceneCamera)
{
    assert(!m_renderer.HasCamera()); // Currently, only one camera per scene supported
    m_renderer.SetCurrentCamera(*sceneCamera.GetCamera());
}

void RendererSceneVisitor::VisitLight(SceneLight& sceneLight)
{
    m_renderer.AddLight(*sceneLight.GetLight());
}

void RendererSceneVisitor::VisitModel(SceneModel& sceneModel)
{
    assert(sceneModel.GetTransform());
    m_renderer.AddModel(*sceneModel.GetModel(), sceneModel.GetTransform()->GetTransformMatrix());
}
