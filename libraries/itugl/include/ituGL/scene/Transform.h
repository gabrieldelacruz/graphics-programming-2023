#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

class Transform
{
public:
    Transform();

    inline glm::vec3 GetTranslation() const { return m_translation; }
    inline void SetTranslation(const glm::vec3& translation) { m_translation = translation; m_dirty = true; }

    inline glm::vec3 GetRotation() const { return m_rotation; }
    inline void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; m_dirty = true; }

    inline glm::vec3 GetScale() const { return m_scale; }
    inline void SetScale(const glm::vec3& scale) { m_scale = scale; m_dirty = true; }

    inline std::shared_ptr<Transform> GetParent() const { return m_parent; }
    inline void SetParent(std::shared_ptr<Transform> parent) { m_parent = parent; m_dirty = true; }

    glm::mat4 GetTranslationMatrix() const;
    glm::mat4 GetRotationMatrix() const;
    glm::mat4 GetScaleMatrix() const;

    glm::mat4 GetTransformMatrix() const;

    bool IsDirty() const;

private:
    glm::vec3 m_translation;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    std::shared_ptr<Transform> m_parent;

    // Cached matrix
    mutable glm::mat4 m_matrix;
    mutable bool m_dirty;
};
