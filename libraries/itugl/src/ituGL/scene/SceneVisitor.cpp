#include <ituGL/scene/SceneVisitor.h>

void SceneVisitor::VisitCamera(SceneCamera& sceneCamera)
{
    VisitCamera(const_cast<const SceneCamera&>(sceneCamera));
}

void SceneVisitor::VisitCamera(const SceneCamera& sceneCamera)
{
}

void SceneVisitor::VisitLight(SceneLight& sceneLight)
{
    VisitLight(const_cast<const SceneLight&>(sceneLight));
}

void SceneVisitor::VisitLight(const SceneLight& sceneLight)
{
}

void SceneVisitor::VisitModel(SceneModel& sceneModel)
{
    VisitModel(const_cast<const SceneModel&>(sceneModel));
}

void SceneVisitor::VisitModel(const SceneModel& sceneModel)
{
}

void SceneVisitor::VisitRenderable(Renderable& renderable)
{
    VisitRenderable(const_cast<const Renderable&>(renderable));
}

void SceneVisitor::VisitRenderable(const Renderable& renderable)
{
}
