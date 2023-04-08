#include <ituGL/lighting/Light.h>

Light::Light() : m_color(1.0f), m_intensity(1.0f)
{
}

Light::~Light()
{
}

glm::vec3 Light::GetPosition() const
{
    return GetPosition(glm::vec3(0));
}

glm::vec3 Light::GetPosition(const glm::vec3& fallback) const
{
    return fallback;
}

void Light::SetPosition(const glm::vec3& position)
{
}

glm::vec3 Light::GetDirection() const
{
    return GetDirection(glm::vec3(0, 1, 0));
}

glm::vec3 Light::GetDirection(const glm::vec3& fallback) const
{
    return fallback;
}

void Light::SetDirection(const glm::vec3& direction)
{
}

glm::vec4 Light::GetAttenuation() const
{
    return glm::vec4(-1);
}

glm::vec3 Light::GetColor() const
{
    return m_color;
}

void Light::SetColor(const glm::vec3& color)
{
    m_color = color;
}

float Light::GetIntensity() const
{
    return m_intensity;
}

void Light::SetIntensity(float intensity)
{
    m_intensity = intensity;
}
