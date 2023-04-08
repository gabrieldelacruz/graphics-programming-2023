#include <ituGL/scene/SceneLight.h>

#include <ituGL/lighting/DirectionalLight.h>
#include <ituGL/lighting/PointLight.h>
#include <ituGL/lighting/SpotLight.h>
#include <ituGL/scene/Transform.h>
#include <ituGL/scene/SceneVisitor.h>

SceneLight::SceneLight(const std::string& name, std::shared_ptr<Light> light) : SceneNode(name), m_light(light)
{
    MatchTransformToLight();
}

SceneLight::SceneLight(const std::string& name, std::shared_ptr<Light> light, std::shared_ptr<Transform> transform) : SceneNode(name, transform), m_light(light)
{
    MatchLightToTransform();
}

std::shared_ptr<Light> SceneLight::GetLight() const
{
    return m_light;
}

void SceneLight::SetLight(std::shared_ptr<Light> light)
{
    m_light = light;
}

void SceneLight::AcceptVisitor(SceneVisitor& visitor)
{
    visitor.VisitLight(*this);
}

void SceneLight::AcceptVisitor(SceneVisitor& visitor) const
{
    visitor.VisitLight(*this);
}

void SceneLight::MatchLightToTransform()
{
    if (m_light && m_transform)
    {
        assert(!m_transform->GetParent());
        glm::vec3 position = m_transform->GetTranslation();
        m_light->SetPosition(position);
        glm::vec3 direction = glm::mat3(m_transform->GetRotationMatrix()) * glm::vec3(0.0f, -1.0f, 0.0f);
        m_light->SetDirection(direction);
    }
}

void SceneLight::MatchTransformToLight()
{
    if (m_light && m_transform)
    {
        assert(!m_transform->GetParent());
        glm::vec3 position = m_light->GetPosition(m_transform->GetTranslation());
        glm::vec3 transformDirection = glm::mat3(m_transform->GetRotationMatrix()) * glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 direction = m_light->GetDirection(transformDirection);
        glm::vec3 rotation = GetRotationFromDirection(direction);
        m_transform->SetTranslation(position);
        m_transform->SetRotation(rotation);
        m_transform->SetScale(glm::vec3(1.0f));
    }
}

glm::vec3 SceneLight::GetRotationFromDirection(const glm::vec3& direction) const
{
    glm::vec3 rotation(0);
    rotation.x = std::acos(-direction.y);
    rotation.y = std::atan2(-direction.x, -direction.z);
    return rotation;
}
