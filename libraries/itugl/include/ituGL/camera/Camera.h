#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// Class that represents a camera in a 3D scene
class Camera
{
public:
    Camera();

    inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    inline glm::mat4& GetViewMatrix() { return m_viewMatrix; }

    inline void SetViewMatrix(const glm::mat4& viewMatrix) { m_viewMatrix = viewMatrix; }
    void SetViewMatrix(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    inline const glm::mat4& GetProjectionMatrix() const { return m_projMatrix; }
    inline glm::mat4& GetProjectionMatrix() { return m_projMatrix; }

    inline void SetProjectionMatrix(const glm::mat4& projMatrix) { m_projMatrix = projMatrix; }
    void SetPerspectiveProjectionMatrix(float fov, float aspect, float near, float far);
    void SetOrthographicProjectionMatrix(const glm::vec3& min, const glm::vec3& max);

    // Get the combined view-projection matrix. Matrix transformations can be combined by multiplying them
    inline glm::mat4 GetViewProjectionMatrix() const { return m_projMatrix * m_viewMatrix; }

    // Extract the translation from the view matrix
    glm::vec3 ExtractTranslation() const;

    // Extract the rotation from the view matrix
    glm::vec3 ExtractRotation() const;

    // Extract the scale from the view matrix
    glm::vec3 ExtractScale() const;

    // Extract the basis vectors from the view matrix
    void ExtractVectors(glm::vec3& right, glm::vec3& up, glm::vec3& forward) const;


private:
    // The view matrix (from world space to view space)
    glm::mat4 m_viewMatrix;

    // The projection matrix (from view space to clip space)
    glm::mat4 m_projMatrix;
};
