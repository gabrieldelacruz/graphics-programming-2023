#include <ituGL/geometry/Model.h>

#include <ituGL/geometry/Mesh.h>
#include <ituGL/shader/Material.h>

Model::Model(std::shared_ptr<Mesh> mesh) : m_mesh(mesh)
{
}

Mesh& Model::GetMesh()
{
    return *m_mesh;
}

const Mesh& Model::GetMesh() const
{
    return *m_mesh;
}

void Model::SetMesh(std::shared_ptr<Mesh> mesh)
{
    // Clear the material list before changing the mesh
    assert(m_materials.empty());
    m_mesh = mesh;
}

unsigned int Model::GetMaterialCount()
{
    return static_cast<unsigned int>(m_materials.size());
}

Material& Model::GetMaterial(unsigned int index)
{
    return *m_materials[index];
}

const Material& Model::GetMaterial(unsigned int index) const
{
    return *m_materials[index];
}

void Model::SetMaterial(unsigned int index, std::shared_ptr<Material> material)
{
    m_materials[index] = material;
}

unsigned int Model::AddMaterial(std::shared_ptr<Material> material)
{
    unsigned int index = static_cast<unsigned int>(m_materials.size());
    m_materials.push_back(material);
    return index;
}

void Model::ClearMaterials()
{
    m_materials.clear();
}

void Model::Draw()
{
    if (m_mesh)
    {
        // Ensure that there is a material for each submesh
        assert(m_mesh->GetSubmeshCount() == m_materials.size());

        for (unsigned int submeshIndex = 0; submeshIndex < m_mesh->GetSubmeshCount(); ++submeshIndex)
        {
            Material& material = GetMaterial(submeshIndex);

            // Set up the material before rendering
            material.Use();

            // Draw the submesh
            m_mesh->DrawSubmesh(submeshIndex);
        }
    }
}
