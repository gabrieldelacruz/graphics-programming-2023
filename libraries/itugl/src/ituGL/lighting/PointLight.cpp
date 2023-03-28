#include <ituGL/lighting/PointLight.h>

PointLight::PointLight() : m_position(0.0f), m_attenuation(0.0f)
{
}

Light::Type PointLight::GetType() const
{
    return Light::Type::Point;
}

glm::vec3 PointLight::GetPosition(const glm::vec3& fallback) const
{
    return m_position;
}

void PointLight::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

glm::vec4 PointLight::GetAttenuation() const
{
    return glm::vec4(m_attenuation, 0.0f, 0.0f);
}

glm::vec2 PointLight::GetDistanceAttenuation() const
{
    return m_attenuation;
}

void PointLight::SetDistanceAttenuation(glm::vec2 attenuation)
{
    m_attenuation = attenuation;
}
