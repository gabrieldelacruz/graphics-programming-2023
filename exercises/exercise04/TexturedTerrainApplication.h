#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/geometry/Mesh.h>
#include <ituGL/camera/Camera.h>
#include <ituGL/shader/Material.h>
#include <glm/mat4x4.hpp>
#include <vector>

class Texture2DObject;

class TexturedTerrainApplication : public Application
{
public:
    TexturedTerrainApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;

private:
    void InitializeTextures();
    void InitializeMaterials();
    void InitializeMeshes();

    void DrawObject(const Mesh& mesh, Material& material, const glm::mat4& worldMatrix);

    std::shared_ptr<Texture2DObject> CreateDefaultTexture();
    std::shared_ptr<Texture2DObject> CreateHeightMap(unsigned int width, unsigned int height, glm::ivec2 coords);
    std::shared_ptr<Texture2DObject> LoadTexture(const char* path);

    void CreateTerrainMesh(Mesh& mesh, unsigned int gridX, unsigned int gridY);

private:
    unsigned int m_gridX, m_gridY;

    Camera m_camera;

    ShaderLoader m_vertexShaderLoader;
    ShaderLoader m_fragmentShaderLoader;

    Mesh m_terrainPatch;

    std::shared_ptr<Material> m_defaultMaterial;
    std::shared_ptr<Material> m_terrainMaterial00;
    std::shared_ptr<Material> m_terrainMaterial10;
    std::shared_ptr<Material> m_terrainMaterial01;
    std::shared_ptr<Material> m_terrainMaterial11;
    std::shared_ptr<Material> m_waterMaterial;

    std::shared_ptr<Texture2DObject> m_defaultTexture;
    std::shared_ptr<Texture2DObject> m_heightmapTexture00;
    std::shared_ptr<Texture2DObject> m_heightmapTexture10;
    std::shared_ptr<Texture2DObject> m_heightmapTexture01;
    std::shared_ptr<Texture2DObject> m_heightmapTexture11;
    std::shared_ptr<Texture2DObject> m_dirtTexture;
    std::shared_ptr<Texture2DObject> m_grassTexture;
    std::shared_ptr<Texture2DObject> m_rockTexture;
    std::shared_ptr<Texture2DObject> m_snowTexture;
    std::shared_ptr<Texture2DObject> m_waterTexture;
};
