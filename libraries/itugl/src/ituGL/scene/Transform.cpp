#include <ituGL/scene/Transform.h>

#include <glm/ext/matrix_transform.hpp>

Transform::Transform() : m_translation(0, 0, 0), m_rotation(0, 0, 0), m_scale(1, 1, 1), m_matrix(1.0f), m_dirty(false)
{
}

glm::mat4 Transform::GetTranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), m_translation);
}

glm::mat4 Transform::GetRotationMatrix() const
{
    glm::mat4 matrix = glm::identity<glm::mat4>();
    matrix = glm::rotate(matrix, m_rotation.y, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, m_rotation.x, glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, m_rotation.z, glm::vec3(0, 0, 1));
    return matrix;
}

glm::mat4 Transform::GetScaleMatrix() const
{
    return glm::scale(glm::identity<glm::mat4>(), m_scale);
}

glm::mat4 Transform::GetTransformMatrix() const
{
    if (IsDirty())
    {
        m_matrix = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
        if (m_parent)
        {
            m_matrix = m_parent->GetTransformMatrix() * m_matrix;
        }
        m_dirty = false;
    }
    return m_matrix;
}

bool Transform::IsDirty() const
{
    return m_dirty || (m_parent && m_parent->IsDirty());
}
