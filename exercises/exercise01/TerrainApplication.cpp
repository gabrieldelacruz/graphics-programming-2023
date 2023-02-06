#include "TerrainApplication.h"

// (todo) 01.1: Include the libraries you need

#include <cmath>


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

// (todo) 01.8: Declare an struct with the vertex format



TerrainApplication::TerrainApplication()
    : Application(1024, 1024, "Terrain demo"), m_gridX(16), m_gridY(16)
{
}

void TerrainApplication::Initialize()
{
    Application::Initialize();

    // (todo) 01.1: Create containers for the vertex position


    // (todo) 01.1: Fill in vertex data


    // (todo) 01.1: Initialize VAO, and VBO


    // (todo) 01.5: Initialize EBO


    // (todo) 01.1: Unbind VAO, and VBO


    // (todo) 01.5: Unbind EBO

}

void TerrainApplication::Update()
{
    Application::Update();
}

void TerrainApplication::Render()
{
    Application::Render();

    // (todo) 01.1: Draw the grid

}

void TerrainApplication::Cleanup()
{
    Application::Cleanup();
}
