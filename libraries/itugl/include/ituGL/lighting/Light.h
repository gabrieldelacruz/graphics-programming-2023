#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Light
{
public:
    enum class Type
    {
        Directional,
        Point,
        Spot,
    };

public:
    Light();
    virtual ~Light();

    virtual Type GetType() const = 0;

    glm::vec3 GetPosition() const;
    virtual glm::vec3 GetPosition(const glm::vec3& fallback) const;
    virtual void SetPosition(const glm::vec3& position);

    glm::vec3 GetDirection() const;
    virtual glm::vec3 GetDirection(const glm::vec3& fallback) const;
    virtual void SetDirection(const glm::vec3& direction);

    virtual glm::vec4 GetAttenuation() const;

    glm::vec3 GetColor() const;
    void SetColor(const glm::vec3& color);

    float GetIntensity() const;
    void SetIntensity(float intensity);

private:
    glm::vec3 m_color;
    float m_intensity;
};
