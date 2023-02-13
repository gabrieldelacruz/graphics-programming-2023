#include "ParticlesApplication.h"

#include <ituGL/shader/Shader.h>
#include <ituGL/geometry/VertexAttribute.h>
#include <cassert>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

// Structure defining that Particle data
struct Particle
{
    glm::vec2 position;
    // (todo) 02.X: Add more vertex attributes
 
};

// List of attributes of the particle. Must match the structure above
const std::array<VertexAttribute, 1> s_vertexAttributes =
{
    VertexAttribute(Data::Type::Float, 2), // position
    // (todo) 02.X: Add more vertex attributes

};


ParticlesApplication::ParticlesApplication()
    : Application(1024, 1024, "Particles demo")
    , m_particleCount(0)
    , m_particleCapacity(2048)  // You can change the capacity here to have more particles
{
}

void ParticlesApplication::Initialize()
{
    InitializeGeometry();

    InitializeShaders();

    // Initialize the mouse position with the current position of the mouse
    m_mousePosition = GetMainWindow().GetMousePosition(true);

    // (todo) 02.2: Enable the GL_PROGRAM_POINT_SIZE feature on the device


    // (todo) 02.3: Enable the GL_BLEND feature on the device


    // We need to enable V-sync, otherwise the framerate would be too high and spawn multiple particles in one click
    GetDevice().SetVSyncEnabled(true);
}

void ParticlesApplication::Update()
{
    Application::Update();

    const Window& window = GetMainWindow();

    // Get the mouse position this frame
    glm::vec2 mousePosition = window.GetMousePosition(true);

    // Emit particles while the left button is pressed
    if (window.IsMouseButtonPressed(Window::MouseButton::Left))
    {
        // (todo) 02.X: Compute new particle attributes here


        EmitParticle(mousePosition);
    }

    // save the mouse position (to compare next frame and obtain velocity)
    m_mousePosition = mousePosition;
}

void ParticlesApplication::Render()
{
    // Clear background
    GetDevice().Clear(Color(0.0f, 0.0f, 0.0f));

    // Set our particles shader program
    m_shaderProgram.Use();

    // (todo) 02.4: Set CurrentTime uniform


    // (todo) 02.6: Set Gravity uniform


    // Bind the particle system VAO
    m_vao.Bind();

    // Draw points. The amount of points can't exceed the capacity
    glDrawArrays(GL_POINTS, 0, std::min(m_particleCount, m_particleCapacity));

    Application::Render();
}

// Nothing to do in this method for this exercise.
// Change s_vertexAttributes and the Particle struct to add new vertex attributes
void ParticlesApplication::InitializeGeometry()
{
    m_vbo.Bind();

    // Allocate enough data for all the particles
    // Notice the DynamicDraw usage, because we will update the buffer every time we emit a particle
    m_vbo.AllocateData(m_particleCapacity * sizeof(Particle), BufferObject::Usage::DynamicDraw);

    m_vao.Bind();

    // Automatically iterate through the vertex attributes, and set the pointer
    // We use interleaved attributes, so the offset is local to the particle, and the stride is the size of the particle
    GLsizei stride = sizeof(Particle);
    GLint offset = 0;
    GLuint location = 0;
    for (const VertexAttribute& attribute : s_vertexAttributes)
    {
        m_vao.SetAttribute(location++, attribute, offset, stride);
        offset += attribute.GetSize();
    }

    // Unbind VAO and VBO
    VertexArrayObject::Unbind();
    VertexBufferObject::Unbind();
}

// Load, compile and Build shaders
void ParticlesApplication::InitializeShaders()
{
    // Load and compile vertex shader
    Shader vertexShader(Shader::VertexShader);
    LoadAndCompileShader(vertexShader, "shaders/particles.vert");

    // Load and compile fragment shader
    Shader fragmentShader(Shader::FragmentShader);
    LoadAndCompileShader(fragmentShader, "shaders/particles.frag");

    // Attach shaders and link
    if (!m_shaderProgram.Build(vertexShader, fragmentShader))
    {
        std::cout << "Error linking shaders" << std::endl;
    }
}

void ParticlesApplication::EmitParticle(const glm::vec2& position)
{
    // Initialize the particle
    Particle particle;
    particle.position = position;
    // (todo) 02.X: Set the value for other attributes of the particle


    // Get the index in the circular buffer
    unsigned int particleIndex = m_particleCount % m_particleCapacity;

    // Bind the VBO before updating data
    m_vbo.Bind();

    // Update the particle data in the VBO
    int offset = particleIndex * sizeof(Particle);
    m_vbo.UpdateData(std::span(&particle, 1), offset);

    // Unbind the VBO
    VertexBufferObject::Unbind();

    // Increment the particle count
    m_particleCount++;
}

void ParticlesApplication::LoadAndCompileShader(Shader& shader, const char* path)
{
    // Open the file for reading
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "Can't find file: " << path << std::endl;
        std::cout << "Is your working directory properly set?" << std::endl;
        return;
    }

    // Dump the contents into a string
    std::stringstream stringStream;
    stringStream << file.rdbuf() << '\0';

    // Set the source code from the string
    shader.SetSource(stringStream.str().c_str());

    // Try to compile
    if (!shader.Compile())
    {
        // Get errors in case of failure
        std::array<char, 256> errors;
        shader.GetCompilationErrors(errors);
        std::cout << "Error compiling shader: " << path << std::endl;
        std::cout << errors.data() << std::endl;
    }
}

float ParticlesApplication::Random01()
{
    return static_cast<float>(rand()) / RAND_MAX;
}

float ParticlesApplication::RandomRange(float from, float to)
{
    return Random01() * (to - from) + from;
}

glm::vec2 ParticlesApplication::RandomDirection()
{
    return glm::normalize(glm::vec2(Random01() - 0.5f, Random01() - 0.5f));
}

Color ParticlesApplication::RandomColor()
{
    return Color(Random01(), Random01(), Random01());
}
