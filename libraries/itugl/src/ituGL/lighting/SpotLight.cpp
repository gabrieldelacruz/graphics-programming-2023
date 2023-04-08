#include <ituGL/lighting/SpotLight.h>

#include <glm/geometric.hpp>

SpotLight::SpotLight() : m_position(0.0f), m_direction(0.0f, 1.0f, 0.0f), m_attenuation(0.0f)
{
}

Light::Type SpotLight::GetType() const
{
    return Light::Type::Spot;
}

glm::vec3 SpotLight::GetPosition(const glm::vec3& fallback) const
{
    return m_position;
}

void SpotLight::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

glm::vec3 SpotLight::GetDirection(const glm::vec3& fallback) const
{
    return m_direction;
}

void SpotLight::SetDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
}

glm::vec4 SpotLight::GetAttenuation() const
{
    return m_attenuation;
}

float SpotLight::GetAngle() const
{
    return m_attenuation.w;
}

void SpotLight::SetAngle(float angle)
{
    m_attenuation.w = angle;
}

glm::vec2 SpotLight::GetDistanceAttenuation() const
{
    return glm::vec2(m_attenuation.x, m_attenuation.y);
}

void SpotLight::SetDistanceAttenuation(glm::vec2 attenuation)
{
    m_attenuation.x = attenuation.x;
    m_attenuation.y = attenuation.y;
}

glm::vec2 SpotLight::GetAngleAttenuation() const
{
    return glm::vec2(m_attenuation.z, m_attenuation.w);
}

void SpotLight::SetAngleAttenuation(glm::vec2 attenuation)
{
    m_attenuation.z = attenuation.x;
    m_attenuation.w = attenuation.y;
}
