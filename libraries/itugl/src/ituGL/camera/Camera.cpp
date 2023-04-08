#include <ituGL/camera/Camera.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <numbers>

Camera::Camera() : m_viewMatrix(1.0f), m_projMatrix(1.0f)
{
}

void Camera::SetViewMatrix(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up)
{
    m_viewMatrix = glm::lookAt(position, lookAt, up);
}

void Camera::SetPerspectiveProjectionMatrix(float fov, float aspect, float near, float far)
{
    m_projMatrix = glm::perspective(fov, aspect, near, far);
}

void Camera::SetOrthographicProjectionMatrix(const glm::vec3& min, const glm::vec3& max)
{
    m_projMatrix = glm::ortho(min.x, max.x, min.y, max.y, min.z, max.z);
}

glm::vec3 Camera::ExtractTranslation() const
{
    // Keep only 3x3 rotation part of the matrix
    glm::mat3 transposed = glm::transpose(m_viewMatrix);

    glm::vec3 inverseTranslation = m_viewMatrix[3];

    return transposed * -inverseTranslation;
}

glm::vec3 Camera::ExtractRotation() const
{
    // Columns should be divided by scale first, but scale is (1, 1, 1) 
    glm::vec3 rotation(0.0f);
    float f = m_viewMatrix[1][2];
    if (std::abs(std::abs(f) - 1.0f) < 0.00001f)
    {
        rotation.x = -f * static_cast<float>(std::numbers::pi) * 0.5f;
        rotation.y = std::atan2(-f * m_viewMatrix[0][1], -f * m_viewMatrix[0][0]);
        rotation.z = 0.0f;
    }
    else
    {
        rotation.x = -std::asin(f);
        float cosX = std::cos(rotation.x);
        rotation.y = std::atan2(m_viewMatrix[0][2] / cosX, m_viewMatrix[2][2] / cosX);
        rotation.z = std::atan2(m_viewMatrix[1][0] / cosX, m_viewMatrix[1][1] / cosX);
    }
    return rotation;
}

glm::vec3 Camera::ExtractScale() const
{
    // Should return (1, 1, 1)
    return glm::vec3(m_viewMatrix[0].length(), m_viewMatrix[1].length(), m_viewMatrix[2].length());
}

void Camera::ExtractVectors(glm::vec3& right, glm::vec3& up, glm::vec3& forward) const
{
    // Keep only 3x3 rotation part of the matrix
    glm::mat3 transposed = glm::transpose(m_viewMatrix);

    right = transposed[0];
    up = transposed[1];
    forward = transposed[2];
}
