#pragma once

#include <ituGL/lighting/Light.h>
#include <glm/vec2.hpp>

class SpotLight : public Light
{
public:
    SpotLight();

    Type GetType() const override;

    using Light::GetPosition;
    glm::vec3 GetPosition(const glm::vec3& fallback) const override;
    void SetPosition(const glm::vec3& position) override;

    using Light::GetDirection;
    glm::vec3 GetDirection(const glm::vec3& fallback) const override;
    void SetDirection(const glm::vec3& direction) override;

    glm::vec4 GetAttenuation() const override;

    float GetAngle() const;
    void SetAngle(float angle);

    glm::vec2 GetDistanceAttenuation() const;
    void SetDistanceAttenuation(glm::vec2 attenuation);

    glm::vec2 GetAngleAttenuation() const;
    void SetAngleAttenuation(glm::vec2 attenuation);

private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec4 m_attenuation;
};
