#include <ituGL/lighting/DirectionalLight.h>

#include <glm/geometric.hpp>

DirectionalLight::DirectionalLight() : m_direction(0.0f, 1.0f, 0.0f)
{
}

Light::Type DirectionalLight::GetType() const
{
    return Light::Type::Directional;
}

glm::vec3 DirectionalLight::GetDirection(const glm::vec3& fallback) const
{
    return m_direction;
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
}
