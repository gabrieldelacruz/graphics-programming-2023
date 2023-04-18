#include <ituGL/lighting/Light.h>

#include <ituGL/texture/Texture2DObject.h>

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

std::shared_ptr<const TextureObject> Light::GetShadowMap() const
{
    return m_shadowMap;
}

void Light::SetShadowMap(std::shared_ptr<const TextureObject> shadowMap)
{
    m_shadowMap = shadowMap;
}

bool Light::CreateShadowMap(glm::ivec2 resolution)
{
    assert(!m_shadowMap);
    std::shared_ptr<Texture2DObject> shadowMap = std::make_shared<Texture2DObject>();
    shadowMap->Bind();
    shadowMap->SetImage(0, resolution.x, resolution.y, TextureObject::FormatDepth, TextureObject::InternalFormatDepth32);
    shadowMap->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR);
    shadowMap->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_LINEAR);
    shadowMap->SetParameter(TextureObject::ParameterEnum::WrapS, GL_CLAMP_TO_BORDER);
    shadowMap->SetParameter(TextureObject::ParameterEnum::WrapT, GL_CLAMP_TO_BORDER);
    glm::vec4 borderColor(1.0f);
    shadowMap->SetParameter(TextureObject::ParameterColor::BorderColor, std::span<float, 4>(&borderColor[0], &borderColor[0] + 4));
    m_shadowMap = shadowMap;
    return true;
}

glm::mat4 Light::GetShadowMatrix() const
{
    return m_shadowMatrix;
}

void Light::SetShadowMatrix(const glm::mat4& matrix)
{
    m_shadowMatrix = matrix;
}

float Light::GetShadowBias() const
{
    return m_shadowBias;
}

void Light::SetShadowBias(float bias)
{
    assert(bias >= 0.0f);
    m_shadowBias = bias;
}
