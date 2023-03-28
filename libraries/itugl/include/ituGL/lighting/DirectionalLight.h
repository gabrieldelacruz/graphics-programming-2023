#pragma once

#include <ituGL/lighting/Light.h>

class DirectionalLight : public Light
{
public:
    DirectionalLight();

    Type GetType() const override;

    using Light::GetDirection;
    glm::vec3 GetDirection(const glm::vec3& fallback) const override;
    void SetDirection(const glm::vec3& direction) override;

private:
    glm::vec3 m_direction;
};
