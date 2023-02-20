#include "GearsApplication.h"

#include <ituGL/shader/Shader.h>
#include <ituGL/shader/ShaderProgram.h>
#include <ituGL/geometry/VertexFormat.h>
#include <cassert>  // for asserts
#include <array>    // to get shader error messages
#include <fstream>  // shader loading
#include <sstream>  // shader loading
#include <iostream> // to print messages to the console
#include <vector>   // to store vertices and indices
#include <numbers>  // for PI constant
#include <glm/gtx/transform.hpp>  // for matrix transformations

GearsApplication::GearsApplication()
    : Application(1024, 1024, "Gears demo")
    , m_colorUniform(-1)
{
}

void GearsApplication::Initialize()
{
    Application::Initialize();

    InitializeGeometry();

    InitializeShaders();

    GetDevice().EnableFeature(GL_DEPTH_TEST);
}

void GearsApplication::Update()
{
    Application::Update();

    const Window& window = GetMainWindow();

    // (todo) 03.5: Update the camera matrices


}

void GearsApplication::Render()
{
    // Clear background
    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f), true, 1.0f);

    // Set our shader program
    m_shaderProgram.Use();

    // (todo) 03.5: Set the view projection matrix from the camera. Once set, we will use it for all the objects


    // (todo) 03.1: Draw large gear at the center
    glm::mat4 centerGearMatrix(1.0f);
    DrawGear(m_largeGear, centerGearMatrix, Color(1.0f, 1.0f, 1.0f));

    // (todo) 03.2: Draw medium gear to the right


    // (todo) 03.3: Draw small gear at the top-left corner


    // (todo) 03.4: Draw small gear linked to the center gear


    Application::Render();
}

// Create the meshes that we will use during the exercise
void GearsApplication::InitializeGeometry()
{
    CreateGearMesh(m_largeGear, 16, 0.2f, 0.5f, 0.1f, 0.5f, 0.1f);
    CreateGearMesh(m_mediumGear, 8, 0.1f, 0.25f, 0.1f, 0.5f, 0.1f);
    CreateGearMesh(m_smallGear, 30, 0.05f, 0.121f, 0.0125f, 0.5f, 0.015f);
}

// Load, compile and Build shaders
void GearsApplication::InitializeShaders()
{
    // Load and compile vertex shader
    Shader vertexShader(Shader::VertexShader);
    LoadAndCompileShader(vertexShader, "shaders/basic.vert");

    // Load and compile fragment shader
    Shader fragmentShader(Shader::FragmentShader);
    LoadAndCompileShader(fragmentShader, "shaders/basic.frag");

    // Attach shaders and link
    if (!m_shaderProgram.Build(vertexShader, fragmentShader))
    {
        std::cout << "Error linking shaders" << std::endl;
        return;
    }

    m_colorUniform = m_shaderProgram.GetUniformLocation("Color");

    // (todo) 03.1: Find the WorldMatrix uniform location


    // (todo) 03.5: Find the ViewProjMatrix uniform location


}

// Draw a gear mesh with a specific world matrix and color
void GearsApplication::DrawGear(const Mesh& mesh, const glm::mat4& worldMatrix, const Color& color)
{
    m_shaderProgram.SetUniform(m_colorUniform, static_cast<glm::vec3>(color));

    // (todo) 03.1: Set the value of the WorldMatrix uniform


    mesh.DrawSubmesh(0);
}

void GearsApplication::CreateGearMesh(Mesh& mesh, unsigned int cogCount, float innerRadius, float pitchRadius, float addendum, float cogRatio, float depth)
{
    // Define the vertex structure
    struct Vertex
    {
        Vertex() = default;
        Vertex(const glm::vec3& position, const glm::vec3& normal) : position(position), normal(normal) {}
        glm::vec3 position;
        glm::vec3 normal;
    };

    // Define the vertex format (should match the vertex structure)
    VertexFormat vertexFormat;
    vertexFormat.AddVertexAttribute<float>(3);
    vertexFormat.AddVertexAttribute<float>(3);

    // List of vertices (VBO)
    std::vector<Vertex> vertices;

    // List of indices (EBO)
    std::vector<unsigned short> indices;

    // Vertices of a triangle
    vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    vertices.emplace_back(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    vertices.emplace_back(glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // Indices
    indices.push_back(0); indices.push_back(1); indices.push_back(2);

    // Finally create the new submesh with all the data
    mesh.AddSubmesh<Vertex, unsigned short, VertexFormat::LayoutIterator>(Drawcall::Primitive::Triangles, vertices, indices,
        vertexFormat.LayoutBegin(static_cast<int>(vertices.size()), true /* interleaved */), vertexFormat.LayoutEnd());
}

void GearsApplication::LoadAndCompileShader(Shader& shader, const char* path)
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
