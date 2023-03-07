#pragma once

#include <ituGL/asset/AssetLoader.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/geometry/Mesh.h>
#include <vector>

struct aiMesh;
struct aiMaterial;
class VertexFormat;

// Asset loader for Models. Contains a pointer to a reference material for loaded submeshes
class ModelLoader : public AssetLoader<Model>
{
public:
    ModelLoader(std::shared_ptr<Material> referenceMaterial = nullptr);

    std::shared_ptr<Material> GetReferenceMaterial() const;
    void SetReferenceMaterial(std::shared_ptr<Material> referenceMaterial);

    bool GetCreateMaterials() const;
    void SetCreateMaterials(bool createMaterials);

    // Load the model from the path
    Model Load(const char* path) override;

    // Maps a semantic to an attribute in the shader program used by the material
    bool SetMaterialAttribute(VertexAttribute::Semantic semantic, const char* attributeName);

private:
    // Generate a submesh from the loaded mesh data
    void GenerateSubmesh(Mesh& mesh, const aiMesh& meshData);

    // Generate a material from the loaded material data
    std::shared_ptr<Material> GenerateMaterial(const aiMaterial& materialData);

    // Build the vertex data from the mesh data
    static std::vector<GLubyte> CollectVertexData(const aiMesh& meshData, VertexFormat& vertexFormat, bool interleaved);

    // Build the element data from the mesh data
    static std::vector<GLubyte> CollectElementData(const aiMesh& meshData, Data::Type& elementType,
        std::vector<Drawcall::Primitive>& primitives, std::vector<int>& elementCounts);

    // Get the correct vertex data pointer for a specific semantic
    static const void* GetVertexDataPointer(const aiMesh& meshData, VertexAttribute::Semantic semantic, int& stride);

    // Copy one buffer to another preserving the stride
    static void CopyBuffer(void* dstBuffer, size_t dstStride, const void* srcBuffer, size_t srcStride, size_t count, size_t size);

    // Get the type of primitive depending on the number of elements
    static Drawcall::Primitive GetPrimitiveType(int elementCount);

private:
    // Pointer to the reference material
    std::shared_ptr<Material> m_referenceMaterial;

    // Maps the semantic to attributes in the reference material
    Mesh::SemanticMap m_materialAttributeMap;

    // Should create new materials for each submesh or use the reference material
    bool m_createMaterials;
};
