#pragma once

#include <ituGL/application/Application.h>

// (todo) 01.1: Include the libraries you need


class TerrainApplication : public Application
{
public:
    TerrainApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private:
    void BuildShaders();
    void UpdateOutputMode();

private:
    unsigned int m_gridX, m_gridY;
    unsigned int m_shaderProgram;

    // (todo) 01.1: Declare an VBO, VAO


    // (todo) 01.5: Declare an EBO

};
