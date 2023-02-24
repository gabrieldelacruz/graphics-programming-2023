#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/shader/ShaderProgram.h>
#include <ituGL/geometry/Mesh.h>
#include <ituGL/camera/Camera.h>

class GearsApplication : public Application
{
public:
    GearsApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;

private:
    // Initialize the gear meshes
    void InitializeGeometry();

    // Load, compile and link shaders
    void InitializeShaders();

    // Create a new gear mesh:
    // cogCount: Number of cogs (teeth)
    // innerRadius: Radius of the small circle inside the gear
    // pitchRadius: Radius of the gear, at the distance where it makes contact, in the middle of the cog length
    // addendum: Height of the cogs
    // depth: Size of the cog in Z-coordinate
    void CreateGearMesh(Mesh& mesh, unsigned int cogCount, float innerRadius, float pitchRadius, float addendum, float cogRatio, float depth);

    // Draw a gear mesh with a specific world matrix and color
    void DrawGear(const Mesh& mesh, const glm::mat4& worldMatrix, const Color& color);

    // Helper function to encapsulate loading and compiling a shader
    void LoadAndCompileShader(Shader& shader, const char* path);

private:

    // Gear meshes
    Mesh m_largeGear;
    Mesh m_mediumGear;
    Mesh m_smallGear;

    // Shader program
    ShaderProgram m_shaderProgram;

    // Shader uniform locations
    ShaderProgram::Location m_colorUniform;
    ShaderProgram::Location m_worldMatrixUniform;
    ShaderProgram::Location m_viewProjUniform;

    Camera m_camera;
};
