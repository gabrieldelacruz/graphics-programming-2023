#pragma once

#include <ituGL/lighting/Light.h>
#include <glm/vec2.hpp>

class PointLight : public Light
{
public:
    PointLight();

    Type GetType() const override;

    using Light::GetPosition;
    glm::vec3 GetPosition(const glm::vec3& fallback) const override;
    void SetPosition(const glm::vec3& position) override;

    glm::vec4 GetAttenuation() const override;

    glm::vec2 GetDistanceAttenuation() const;
    void SetDistanceAttenuation(glm::vec2 attenuation);

private:
    glm::vec3 m_position;
    glm::vec2 m_attenuation;
};
