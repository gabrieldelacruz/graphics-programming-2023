#pragma once

#include <ituGL/scene/SceneNode.h>

class Camera;

class SceneCamera : public SceneNode
{
public:
    SceneCamera(const std::string& name, std::shared_ptr<Camera> camera);
    SceneCamera(const std::string& name, std::shared_ptr<Camera> camera, std::shared_ptr<Transform> transform);

    std::shared_ptr<Camera> GetCamera();
    std::shared_ptr<const Camera> GetCamera() const;
    void SetCamera(std::shared_ptr<Camera> camera);

    void AcceptVisitor(SceneVisitor& visitor) override;
    void AcceptVisitor(SceneVisitor& visitor) const override;

    void MatchCameraToTransform();
    void MatchTransformToCamera();

private:
    std::shared_ptr<Camera> m_camera;
};
