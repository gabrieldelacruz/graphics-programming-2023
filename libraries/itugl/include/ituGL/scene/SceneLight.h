#pragma once

#include <ituGL/scene/SceneNode.h>

class Light;

class SceneLight : public SceneNode
{
public:
    SceneLight(const std::string& name, std::shared_ptr<Light> light);
    SceneLight(const std::string& name, std::shared_ptr<Light> light, std::shared_ptr<Transform> transform);

    std::shared_ptr<Light> GetLight() const;
    void SetLight(std::shared_ptr<Light> light);

    void AcceptVisitor(SceneVisitor& visitor) override;
    void AcceptVisitor(SceneVisitor& visitor) const override;

    void MatchLightToTransform();
    void MatchTransformToLight();

private:
    glm::vec3 GetRotationFromDirection(const glm::vec3& direction) const;

private:
    std::shared_ptr<Light> m_light;
};
