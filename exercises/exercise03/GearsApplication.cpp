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

    // Mesh will have 2 sides for each cog
    unsigned int sides = 2 * cogCount;

    // Base delta angle for each side is 2*PI / sides
    float deltaAngle = 2 * static_cast<float>(std::numbers::pi) / sides;

    // Useful radius
    float outerRadius = pitchRadius - addendum * 0.5f;
    float teethRadius = pitchRadius + addendum * 0.5f;

    // Offsets to ensure the cogs fit
    float offsetOuterAngle = -deltaAngle * (0.5f - cogRatio / (1 + outerRadius / teethRadius));
    float offsetTeethAngle = deltaAngle * (0.5f - cogRatio / (1 + teethRadius / outerRadius));

    // Centered at 0. Half depth on each side
    float halfDepth = 0.5f * depth;

    // Loop over all the sides creating the vertices
    for (unsigned int i = 0; i < sides; ++i)
    {
        // Detect if it is a cog or a hole
        bool isCog = i % 2 == 1;
        int cogSign = isCog ? 1 : -1;

        // Add vertices
        {
            // Precompute normals
            glm::vec3 frontNormal = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec2 teethNormal2D(halfDepth * (1.0f - cogRatio), addendum);
            teethNormal2D = glm::normalize(teethNormal2D);
            glm::vec3 teethFrontNormal(0);
            glm::vec3 teethBackNormal(0);

            // Inner circle
            float innerAngle = i * deltaAngle;
            glm::vec2 innerDirection(std::sin(innerAngle), std::cos(innerAngle));
            glm::vec3 innerFrontPosition = glm::vec3(innerDirection * innerRadius, halfDepth);
            glm::vec3 innerBackPosition = glm::vec3(innerDirection * innerRadius, -halfDepth);
            vertices.emplace_back(innerFrontPosition, frontNormal); // 0
            vertices.emplace_back(innerBackPosition, -frontNormal); // 1
            // Break for center hole normals
            vertices.emplace_back(innerFrontPosition, glm::vec3(-innerDirection, 0.0f)); // 2
            vertices.emplace_back(innerBackPosition, glm::vec3(-innerDirection, 0.0f));  // 3

            // Outer circle
            float outerAngle = innerAngle + offsetOuterAngle * cogSign;
            glm::vec2 outerDirection(std::sin(outerAngle), std::cos(outerAngle));
            glm::vec3 outerFrontPosition = glm::vec3(outerDirection * outerRadius, halfDepth);
            glm::vec3 outerBackPosition = glm::vec3(outerDirection * outerRadius, -halfDepth);
            vertices.emplace_back(outerFrontPosition, frontNormal);  // 4
            vertices.emplace_back(outerBackPosition, -frontNormal);  // 5
            // Break for teeth front normals
            teethFrontNormal = glm::vec3(outerDirection * teethNormal2D.x, teethNormal2D.y);
            teethBackNormal = glm::vec3(outerDirection * teethNormal2D.x, -teethNormal2D.y);
            vertices.emplace_back(outerFrontPosition, teethFrontNormal); // 6
            vertices.emplace_back(outerBackPosition, teethBackNormal);   // 7
            // Break for teeth hole normals
            vertices.emplace_back(outerFrontPosition, glm::vec3(outerDirection, 0.0f)); // 8
            vertices.emplace_back(outerBackPosition, glm::vec3(outerDirection, 0.0f));  // 9
            // Break for teeth side normals
            float sideAngle = offsetTeethAngle - offsetOuterAngle;
            glm::vec3 teethSideNormal(std::sin(sideAngle) * innerDirection - cogSign * std::cos(sideAngle) * glm::vec2(innerDirection.y, -innerDirection.x), 0.0f);
            vertices.emplace_back(outerFrontPosition, teethSideNormal); // 10
            vertices.emplace_back(outerBackPosition, teethSideNormal);  // 11

            // Teeth circle
            float teethAngle = innerAngle + offsetTeethAngle * cogSign;
            glm::vec2 teethDirection(std::sin(teethAngle), std::cos(teethAngle));
            glm::vec3 teethFrontPosition = glm::vec3(teethDirection * teethRadius, halfDepth * cogRatio);
            glm::vec3 teethBackPosition = glm::vec3(teethDirection * teethRadius, -halfDepth * cogRatio);
            teethFrontNormal = glm::vec3(teethDirection * teethNormal2D.x, teethNormal2D.y);
            teethBackNormal = glm::vec3(teethDirection * teethNormal2D.x, -teethNormal2D.y);
            vertices.emplace_back(teethFrontPosition, teethFrontNormal); // 12
            vertices.emplace_back(teethBackPosition, teethBackNormal);   // 13
            // Break for teeth cap normals
            vertices.emplace_back(teethFrontPosition, glm::vec3(teethDirection, 0.0f)); // 14
            vertices.emplace_back(teethBackPosition, glm::vec3(teethDirection, 0.0f));  // 15
            // Break for teeth side normals
            vertices.emplace_back(teethFrontPosition, teethSideNormal); // 16
            vertices.emplace_back(teethBackPosition, teethSideNormal);  // 17
        }

        // Add triangles
        {
            unsigned vertexPerSide = 18;
            unsigned short index0 = vertexPerSide * i;
            unsigned short index1 = vertexPerSide * ((i + 1) % sides);

            // Body (Front)
            indices.push_back(index0 + 0); indices.push_back(index1 + 0); indices.push_back(index0 + 4);
            indices.push_back(index0 + 4); indices.push_back(index1 + 0); indices.push_back(index1 + 4);
            // Body (Back)
            indices.push_back(index0 + 1); indices.push_back(index1 + 1); indices.push_back(index0 + 5);
            indices.push_back(index0 + 5); indices.push_back(index1 + 1); indices.push_back(index1 + 5);
            // Center hole
            indices.push_back(index0 + 3); indices.push_back(index1 + 3); indices.push_back(index0 + 2);
            indices.push_back(index0 + 2); indices.push_back(index1 + 3); indices.push_back(index1 + 2);

            // Cog side
            indices.push_back(index0 + 10); indices.push_back(index0 + 16); indices.push_back(index0 + 11);
            indices.push_back(index0 + 11); indices.push_back(index0 + 16); indices.push_back(index0 + 17);
            if (isCog)
            {
                // Cog (Front)
                indices.push_back(index0 + 6); indices.push_back(index1 + 6); indices.push_back(index0 + 12);
                indices.push_back(index0 + 12); indices.push_back(index1 + 6); indices.push_back(index1 + 12);
                // Cog (Back)
                indices.push_back(index0 + 7); indices.push_back(index1 + 7); indices.push_back(index0 + 13);
                indices.push_back(index0 + 13); indices.push_back(index1 + 7); indices.push_back(index1 + 13);
                // Cog cap
                indices.push_back(index0 + 15); indices.push_back(index0 + 14); indices.push_back(index1 + 15);
                indices.push_back(index1 + 15); indices.push_back(index0 + 14); indices.push_back(index1 + 14);
            }
            else
            {
                //Cog hole
                indices.push_back(index0 + 9); indices.push_back(index0 + 8); indices.push_back(index1 + 9);
                indices.push_back(index1 + 9); indices.push_back(index0 + 8); indices.push_back(index1 + 8);
            }
        }
    }

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
