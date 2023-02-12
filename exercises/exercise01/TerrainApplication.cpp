#include "TerrainApplication.h"

#include <ituGL/geometry/VertexAttribute.h>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

#include <vector>
#include <cmath>
#include <iostream>

// Helper structures. Declared here only for this exercise
struct Vector2
{
    Vector2() : Vector2(0.f, 0.f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    float x, y;
};

struct Vector3
{
    Vector3() : Vector3(0.f,0.f,0.f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float x, y, z;

    Vector3 Normalize() const
    {
        float length = std::sqrt(1 + x * x + y * y);
        return Vector3(x / length, y / length, z / length);
    }
};

// Helper struct with the vertex format
struct Vertex
{
    Vector3 position;
    Vector2 texCoord;
    Vector3 color;
    Vector3 normal;
};


// Forward declare helper function
Vector3 GetColorFromHeight(float height);


TerrainApplication::TerrainApplication()
    : Application(1024, 1024, "Terrain demo"), m_gridX(256), m_gridY(256), m_shaderProgram(0)
{
}

void TerrainApplication::Initialize()
{
    Application::Initialize();

    // Build shaders and store in m_shaderProgram
    BuildShaders();

    // Create containers for the vertex data
    std::vector<Vertex> vertices;

    // Create container for the element data
    std::vector<unsigned int> indices;

    // Grid scale to convert the entire grid to size 1x1
    Vector2 scale(1.0f / m_gridX, 1.0f / m_gridY);

    // Number of columns and rows
    unsigned int columnCount = m_gridX + 1;
    unsigned int rowCount = m_gridY + 1;

    // Iterate over each VERTEX
    for (int j = 0; j < rowCount; ++j)
    {
        for (int i = 0; i < columnCount; ++i)
        {
            // Vertex data for this vertex only
            Vertex& vertex = vertices.emplace_back();
            float x = i * scale.x - 0.5f;
            float y = j * scale.y - 0.5f;
            float z = stb_perlin_fbm_noise3(x * 2, y * 2, 0.0f, 1.9f, 0.5f, 8) * 0.5f;
            vertex.position = Vector3(x, y, z);
            vertex.texCoord = Vector2(i, j);
            vertex.color = GetColorFromHeight(z);
            vertex.normal = Vector3(0.0f, 0.0f, 1.0f); // Actual value computed after all vertices are created

            // Index data for quad formed by previous vertices and current
            if (i > 0 && j > 0)
            {
                unsigned int top_right = j * columnCount + i; // Current vertex
                unsigned int top_left = top_right - 1;
                unsigned int bottom_right = top_right - columnCount;
                unsigned int bottom_left = bottom_right - 1;

                //Triangle 1
                indices.push_back(bottom_left);
                indices.push_back(bottom_right);
                indices.push_back(top_left);

                //Triangle 2
                indices.push_back(bottom_right);
                indices.push_back(top_left);
                indices.push_back(top_right);
            }
        }
    }

    // Compute normals when we have the positions of all the vertices
    // Iterate AGAIN over each vertex
    for (int j = 0; j < rowCount; ++j)
    {
        for (int i = 0; i < columnCount; ++i)
        {
            // Get the vertex at (i, j)
            int index = j * columnCount + i;
            Vertex& vertex = vertices[index];

            // Compute the delta in X
            int prevX = i > 0 ? index - 1 : index;
            int nextX = i < m_gridX ? index + 1 : index;
            float deltaHeightX = vertices[nextX].position.z - vertices[prevX].position.z;
            float deltaX = vertices[nextX].position.x - vertices[prevX].position.x;
            float x = deltaHeightX / deltaX;

            // Compute the delta in Y
            int prevY = j > 0 ? index - columnCount : index;
            int nextY = j < m_gridY ? index + columnCount : index;
            float deltaHeightY = vertices[nextY].position.z - vertices[prevY].position.z;
            float deltaY = vertices[nextY].position.y - vertices[prevY].position.y;
            float y = deltaHeightY / deltaY;

            // Compute the normal
            vertex.normal = Vector3(x, y, 1.0f).Normalize();
        }
    }

    // Declare attributes
    VertexAttribute positionAttribute(Data::Type::Float, 3);
    VertexAttribute texCoordAttribute(Data::Type::Float, 2);
    VertexAttribute colorAttribute(Data::Type::Float, 3);
    VertexAttribute normalAttribute(Data::Type::Float, 3);

    // Compute offsets inside the VERTEX STRUCT
    size_t positionOffset = 0u;
    size_t texCoordOffset = positionOffset + positionAttribute.GetSize();
    size_t colorOffset = texCoordOffset + texCoordAttribute.GetSize();
    size_t normalOffset = colorOffset + colorAttribute.GetSize();

    // Allocate uninitialized data for the total size in the VBO
    m_vbo.Bind();
    m_vbo.AllocateData(std::span(vertices));

    // The stride is not automatic now. Each attribute element is "sizeof(Vertex)" bytes apart from next
    GLsizei stride = sizeof(Vertex);

    // Set the pointer to the data in the VAO (notice that this offsets are for a single element)
    m_vao.Bind();
    m_vao.SetAttribute(0, positionAttribute, positionOffset, stride);
    m_vao.SetAttribute(1, texCoordAttribute, texCoordOffset, stride);
    m_vao.SetAttribute(2, colorAttribute, colorOffset, stride);
    m_vao.SetAttribute(3, normalAttribute, normalOffset, stride);

    // With VAO bound, bind EBO to register it (and allocate element buffer at the same time)
    m_ebo.Bind();
    m_ebo.AllocateData(std::span(indices));

    // Unbind VAO, and VBO
    VertexBufferObject::Unbind();
    VertexArrayObject::Unbind();

    // Unbind EBO (when VAO is no longer bound)
    ElementBufferObject::Unbind();

    // Enable wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
}

void TerrainApplication::Update()
{
    Application::Update();

    UpdateOutputMode();
}

void TerrainApplication::Render()
{
    Application::Render();

    // Clear color and depth
    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    // Set shader to be used
    glUseProgram(m_shaderProgram);

    // Bind the grid VAO
    m_vao.Bind();

    // Draw the grid (m_gridX * m_gridY quads, 6 vertices per quad)
    glDrawElements(GL_TRIANGLES, m_gridX * m_gridY * 6, GL_UNSIGNED_INT, nullptr);

    // No need to unbind every time
    //VertexArrayObject::Unbind();
}

void TerrainApplication::Cleanup()
{
    Application::Cleanup();
}

Vector3 GetColorFromHeight(float height)
{
    if (height > 0.3f)
    {
        return Vector3(1.0f, 1.0f, 1.0f); // Snow
    }
    else if (height > 0.1f)
    {
        return Vector3(0.3, 0.3f, 0.35f); // Rock
    }
    else if (height > -0.05f)
    {
        return Vector3(0.1, 0.4f, 0.15f); // Grass
    }
    else if (height > -0.1f)
    {
        return Vector3(0.6, 0.5f, 0.4f); // Sand
    }
    else
    {
        return Vector3(0.1f, 0.1f, 0.3f); // Water
    }
}

void TerrainApplication::BuildShaders()
{
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "layout (location = 2) in vec3 aColor;\n"
        "layout (location = 3) in vec3 aNormal;\n"
        "uniform mat4 Matrix = mat4(1);\n"
        "out vec2 texCoord;\n"
        "out vec3 color;\n"
        "out vec3 normal;\n"
        "void main()\n"
        "{\n"
        "   texCoord = aTexCoord;\n"
        "   color = aColor;\n"
        "   normal = aNormal;\n"
        "   gl_Position = Matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "uniform uint Mode = 0u;\n"
        "in vec2 texCoord;\n"
        "in vec3 color;\n"
        "in vec3 normal;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   switch (Mode)\n"
        "   {\n"
        "   default:\n"
        "   case 0:\n"
        "       FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "       break;\n"
        "   case 1:\n"
        "       FragColor = vec4(fract(texCoord), 0.0f, 1.0f);\n"
        "       break;\n"
        "   case 2:\n"
        "       FragColor = vec4(color, 1.0f);\n"
        "       break;\n"
        "   case 3:\n"
        "       FragColor = vec4(normalize(normal), 1.0f);\n"
        "       break;\n"
        "   case 4:\n"
        "       FragColor = vec4(color * max(dot(normalize(normal), normalize(vec3(1,0,1))), 0.2f), 1.0f);\n"
        "       break;\n"
        "   }\n"
        "}\n\0";

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    m_shaderProgram = shaderProgram;
}

void TerrainApplication::UpdateOutputMode()
{
    for (int i = 0; i <= 4; ++i)
    {
        if (GetMainWindow().IsKeyPressed(GLFW_KEY_0 + i))
        {
            int modeLocation = glGetUniformLocation(m_shaderProgram, "Mode");
            glUseProgram(m_shaderProgram);
            glUniform1ui(modeLocation, i);
            break;
        }
    }
    if (GetMainWindow().IsKeyPressed(GLFW_KEY_TAB))
    {
        const float projMatrix[16] = { 0, -1.294f, -0.721f, -0.707f, 1.83f, 0, 0, 0, 0, 1.294f, -0.721f, -0.707f, 0, 0, 1.24f, 1.414f };
        int matrixLocation = glGetUniformLocation(m_shaderProgram, "Matrix");
        glUseProgram(m_shaderProgram);
        glUniformMatrix4fv(matrixLocation, 1, false, projMatrix);
    }
}
