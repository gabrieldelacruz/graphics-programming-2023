#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

class TextureObject;

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

    std::shared_ptr<const TextureObject> GetShadowMap() const;
    void SetShadowMap(std::shared_ptr<const TextureObject> shadowMap);
    virtual bool CreateShadowMap(glm::ivec2 resolution);

    glm::mat4 GetShadowMatrix() const;
    void SetShadowMatrix(const glm::mat4& matrix);

    float GetShadowBias() const;
    void SetShadowBias(float bias);

protected:
    glm::vec3 m_color;
    float m_intensity;
    std::shared_ptr<const TextureObject> m_shadowMap;
    glm::mat4 m_shadowMatrix;
    float m_shadowBias;
};
