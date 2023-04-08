#include <ituGL/scene/SceneCamera.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/scene/SceneVisitor.h>
#include <ituGL/scene/Transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

SceneCamera::SceneCamera(const std::string& name, std::shared_ptr<Camera> camera) : SceneNode(name), m_camera(camera)
{
    MatchTransformToCamera();
}

SceneCamera::SceneCamera(const std::string& name, std::shared_ptr<Camera> camera, std::shared_ptr<Transform> transform) : SceneNode(name, transform), m_camera(camera)
{
    MatchCameraToTransform();
}

std::shared_ptr<Camera> SceneCamera::GetCamera()
{
    return m_camera;
}

std::shared_ptr<const Camera> SceneCamera::GetCamera() const
{
    return m_camera;
}

void SceneCamera::SetCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void SceneCamera::AcceptVisitor(SceneVisitor& visitor)
{
    visitor.VisitCamera(*this);
}

void SceneCamera::AcceptVisitor(SceneVisitor& visitor) const
{
    visitor.VisitCamera(*this);
}

void SceneCamera::MatchCameraToTransform()
{
    if (m_camera && m_transform)
    {
        assert(m_transform->GetScale() == glm::vec3(1.0f));
        assert(!m_transform->GetParent());
        glm::mat4 matrix = glm::transpose(m_transform->GetRotationMatrix());
        matrix = glm::translate(matrix, -m_transform->GetTranslation());
        m_camera->SetViewMatrix(matrix);
    }
}

void SceneCamera::MatchTransformToCamera()
{
    if (m_camera && m_transform)
    {
        assert(!m_transform->GetParent());
        m_transform->SetTranslation(m_camera->ExtractTranslation());
        m_transform->SetRotation(m_camera->ExtractRotation());
        m_transform->SetScale(glm::vec3(1.0f));
    }
}
