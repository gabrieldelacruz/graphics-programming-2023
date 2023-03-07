#pragma once

#include <memory>
#include <vector>

class Mesh;
class Material;

class ShaderProgram;

// Contains a pointer to a Mesh and a list of pointers to materials, one for each submesh
class Model
{
public:
    Model(std::shared_ptr<Mesh> mesh = nullptr);

    Mesh& GetMesh();
    const Mesh& GetMesh() const;

    void SetMesh(std::shared_ptr<Mesh> mesh);

    unsigned int GetMaterialCount();

    Material& GetMaterial(unsigned int index);
    const Material& GetMaterial(unsigned int index) const;

    void SetMaterial(unsigned int index, std::shared_ptr<Material> material);

    // Add a new material pointer to the list
    // Returns the index of the new material
    unsigned int AddMaterial(std::shared_ptr<Material> material);

    // Clear the list of materials
    void ClearMaterials();

    // Draw all the submeshes of the mesh, each one with a material on the list
    void Draw();

private:
    // Pointer to the model Mesh
    std::shared_ptr<Mesh> m_mesh;

    // List of material pointers, one for each submesh
    std::vector<std::shared_ptr<Material>> m_materials;
};
