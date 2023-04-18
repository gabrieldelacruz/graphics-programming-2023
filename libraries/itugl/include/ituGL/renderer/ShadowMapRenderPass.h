#pragma once

#include <ituGL/renderer/RenderPass.h>

#include <glm/vec3.hpp>

class Light;
class Camera;
class Material;

class ShadowMapRenderPass : public RenderPass
{
public:
    ShadowMapRenderPass(std::shared_ptr<Light> light, std::shared_ptr<const Material> material, int drawcallCollectionIndex = 0);

    void SetVolume(glm::vec3 volumeCenter, glm::vec3 volumeSize);

    void Render() override;

private:
    void InitFramebuffer();
    void InitLightCamera(Camera& lightCamera) const;

private:
    std::shared_ptr<Light> m_light;

    std::shared_ptr<const Material> m_material;

    int m_drawcallCollectionIndex;

    glm::vec3 m_volumeCenter;
    glm::vec3 m_volumeSize;
};
