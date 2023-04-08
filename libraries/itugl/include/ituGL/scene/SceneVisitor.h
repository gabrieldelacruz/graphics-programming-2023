#pragma once

class SceneCamera;
class SceneLight;
class SceneModel;
class Renderable;

class SceneVisitor
{
public:
    virtual void VisitCamera(SceneCamera& sceneCamera);
    virtual void VisitCamera(const SceneCamera& sceneCamera);

    virtual void VisitLight(SceneLight& sceneLight);
    virtual void VisitLight(const SceneLight& sceneLight);

    virtual void VisitModel(SceneModel& sceneModel);
    virtual void VisitModel(const SceneModel& sceneModel);

    virtual void VisitRenderable(Renderable& renderable);
    virtual void VisitRenderable(const Renderable& renderable);
};
