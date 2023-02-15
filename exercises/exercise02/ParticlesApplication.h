#pragma once

#include <ituGL/application/Application.h>
#include <ituGL/geometry/VertexBufferObject.h>
#include <ituGL/geometry/VertexArrayObject.h>
#include <ituGL/shader/ShaderProgram.h>

class ParticlesApplication : public Application
{
public:
    ParticlesApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;

private:
    // Initialize the VBO and VAO
    void InitializeGeometry();

    // Load, compile and link shaders
    void InitializeShaders();

    // Helper function to encapsulate loading and compiling a shader
    void LoadAndCompileShader(Shader& shader, const char* path);

    // Emit a new particle
    void EmitParticle(const glm::vec2& position, float size, float duration);

    // Helper methods for random values
    static float Random01();
    static float RandomRange(float from, float to);
    static glm::vec2 RandomDirection();
    static Color RandomColor();

private:
    // All particles stored in a single VBO with interleaved attributes
    VertexBufferObject m_vbo;

    // VAO that represents the particle system
    VertexArrayObject m_vao;

    // Particles shader program
    ShaderProgram m_shaderProgram;

    // Location of the "CurrentTime" uniform
    ShaderProgram::Location m_currentTimeUniform;

    // Mouse position during this frame
    glm::vec2 m_mousePosition;

    // Total number of particles created
    unsigned int m_particleCount;

    // Max number of particles that can exist at the same time
    const unsigned int m_particleCapacity;
};
