#include <ituGL/asset/ModelLoader.h>

#include <ituGL/geometry/VertexFormat.h>
#include <ituGL/shader/Material.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

ModelLoader::ModelLoader(std::shared_ptr<Material> referenceMaterial)
    : m_referenceMaterial(referenceMaterial)
    , m_createMaterials(false)
{
    m_textureLoader.SetGenerateMipmap(true);
}

std::shared_ptr<Material> ModelLoader::GetReferenceMaterial() const
{
    return m_referenceMaterial;
}

void ModelLoader::SetReferenceMaterial(std::shared_ptr<Material> referenceMaterial)
{
    // Clear the previous attribute map
    m_materialAttributeMap.clear();

    m_referenceMaterial = referenceMaterial;
}

bool ModelLoader::GetCreateMaterials() const
{
    return m_createMaterials;
}

void ModelLoader::SetCreateMaterials(bool createMaterials)
{
    m_createMaterials = createMaterials;
}

Texture2DLoader& ModelLoader::GetTexture2DLoader()
{
    return m_textureLoader;
}

const Texture2DLoader& ModelLoader::GetTexture2DLoader() const
{
    return m_textureLoader;
}

bool ModelLoader::SetMaterialAttribute(VertexAttribute::Semantic semantic, const char* attributeName)
{
    bool found = false;
    ShaderProgram::Location location = m_referenceMaterial->GetAttributeLocation(attributeName);
    if (location != -1)
    {
        m_materialAttributeMap.insert(std::make_pair(semantic, location));
        found = true;
    }
    return found;
}

bool ModelLoader::SetMaterialProperty(MaterialProperty materialProperty, const char* uniformName)
{
    bool found = false;
    ShaderProgram::Location location = m_referenceMaterial->GetUniformLocation(uniformName);
    if (location != -1)
    {
        m_materialPropertyMap.insert(std::make_pair(materialProperty, location));
        found = true;
    }
    return found;
}

Model ModelLoader::Load(const char* path)
{
    Model model;

    // Read the file using Assimp importer
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    m_baseFolder = path;
    m_baseFolder.resize(m_baseFolder.rfind('/') + 1);

    // If the file was loaded, load all the meshes as submeshes
    if (scene)
    {
        model.SetMesh(std::make_shared<Mesh>());
        Mesh& mesh = model.GetMesh();
        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            aiMesh& meshData = *scene->mMeshes[meshIndex];
            GenerateSubmesh(mesh, meshData);

            std::shared_ptr<Material> material = m_referenceMaterial;
            if (m_createMaterials)
            {
                // Create a new material with the material data
                material = GenerateMaterial(*scene->mMaterials[meshData.mMaterialIndex]);
            }
            model.AddMaterial(material);
        }
    }

    return model;
}

void ModelLoader::GenerateSubmesh(Mesh& mesh, const aiMesh& meshData)
{
    // Collect vertex data
    VertexFormat vertexFormat;
    bool interleaved = true;
    std::vector<GLubyte> vertexData = CollectVertexData(meshData, vertexFormat, interleaved);
    int vboIndex = mesh.AddVertexData<GLubyte>(vertexData);

    // Collect element data
    Data::Type elementType;
    std::vector<Drawcall::Primitive> primitives;
    std::vector<int> elementCounts;
    std::vector<GLubyte> elementData = CollectElementData(meshData, elementType, primitives, elementCounts);
    int eboIndex = mesh.AddElementData<GLubyte>(elementData);

    // Add submeshes
    int start = 0;
    assert(primitives.size() == elementCounts.size());
    for (int i = 0; i < primitives.size(); ++i)
    {
        Drawcall::Primitive primitive = primitives[i];
        int end = elementCounts[i];
        mesh.AddSubmesh(primitive, start, end - start, elementType, eboIndex, vboIndex, vertexFormat.LayoutBegin(static_cast<int>(vertexData.size()), interleaved), vertexFormat.LayoutEnd(), m_materialAttributeMap);
        start = end;
    }
}

std::shared_ptr<Material> ModelLoader::GenerateMaterial(const aiMaterial& materialData)
{
    std::shared_ptr<Material> material = std::make_shared<Material>(*m_referenceMaterial);
    float value;
    for (auto& materialPropertyPair : m_materialPropertyMap)
    {
        aiColor3D color;
        MaterialProperty materialProperty = materialPropertyPair.first;
        ShaderProgram::Location location = materialPropertyPair.second;
        switch (materialProperty)
        {
        case MaterialProperty::AmbientColor:
            if (materialData.Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
            {
                material->SetUniformValue(location, glm::vec3(color.r, color.g, color.b));
            }
            break;
        case MaterialProperty::DiffuseColor:
            if (materialData.Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
            {
                material->SetUniformValue(location, glm::vec3(color.r, color.g, color.b));
            }
            break;
        case MaterialProperty::SpecularColor:
            if (materialData.Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
            {
                material->SetUniformValue(location, glm::vec3(color.r, color.g, color.b));
            }
            break;
        case MaterialProperty::SpecularExponent:
            if (materialData.Get(AI_MATKEY_SHININESS, value) == aiReturn_SUCCESS)
            {
                material->SetUniformValue(location, value);
            }
            break;
        case MaterialProperty::DiffuseTexture:
            LoadTexture(materialData, aiTextureType_DIFFUSE, *material, location, TextureObject::FormatRGB, TextureObject::InternalFormatSRGB8);
            break;
        case MaterialProperty::NormalTexture:
            LoadTexture(materialData, aiTextureType_NORMALS, *material, location, TextureObject::FormatRGB, TextureObject::InternalFormatRGB8);
            break;
        case MaterialProperty::SpecularTexture:
            LoadTexture(materialData, aiTextureType_SHININESS, *material, location, TextureObject::FormatRGB, TextureObject::InternalFormatSRGB8);
            break;
        }
    }
    return material;
}

void ModelLoader::LoadTexture(const aiMaterial& materialData, int textureTypeValue, Material& material, ShaderProgram::Location location,
    TextureObject::Format format, TextureObject::InternalFormat internalFormat) const
{
    aiTextureType textureType = static_cast<aiTextureType>(textureTypeValue);
    if (materialData.GetTextureCount(textureType) > 0)
    {
        assert(materialData.GetTextureCount(textureType) == 1);
        aiString texturePath;
        if (materialData.GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
        {
            texturePath = m_baseFolder + texturePath.C_Str();
            m_textureLoader.SetFormat(format);
            m_textureLoader.SetInternalFormat(internalFormat);
            std::shared_ptr<Texture2DObject> texture = m_textureLoader.LoadShared(texturePath.C_Str());
            material.SetUniformValue(location, texture);
        }
    }
}

std::vector<GLubyte> ModelLoader::CollectVertexData(const aiMesh& meshData, VertexFormat& vertexFormat, bool interleaved)
{
    vertexFormat.Clear();

    // Buid the vertex format with the available vertex data

    assert(meshData.HasPositions());
    {
        vertexFormat.AddVertexAttribute<float>(3, VertexAttribute::Semantic::Position);
    }
    if (meshData.HasNormals())
    {
        vertexFormat.AddVertexAttribute<float>(3, VertexAttribute::Semantic::Normal);
    }
    if (meshData.HasTangentsAndBitangents())
    {
        vertexFormat.AddVertexAttribute<float>(3, VertexAttribute::Semantic::Tangent);
        vertexFormat.AddVertexAttribute<float>(3, VertexAttribute::Semantic::Bitangent);
    }
    unsigned int colorSemantic = static_cast<unsigned int>(VertexAttribute::Semantic::Color0);
    for (unsigned int colorChannel = 0; colorChannel < meshData.GetNumColorChannels(); ++colorChannel)
    {
        vertexFormat.AddVertexAttribute<GLubyte>(4, true, static_cast<VertexAttribute::Semantic>(colorSemantic + colorChannel));
    }
    unsigned int uvSemantic = static_cast<unsigned int>(VertexAttribute::Semantic::TexCoord0);
    for (unsigned int uvChannel = 0; uvChannel < meshData.GetNumUVChannels(); ++uvChannel)
    {
        vertexFormat.AddVertexAttribute<float>(meshData.mNumUVComponents[uvChannel], static_cast<VertexAttribute::Semantic>(uvSemantic + uvChannel));
    }

    std::vector<GLubyte> vertexData;
    vertexData.resize(vertexFormat.GetSize() * meshData.mNumVertices);

    // Pack the vertex data all together
    auto it = vertexFormat.LayoutBegin(meshData.mNumVertices, interleaved);
    auto itEnd = vertexFormat.LayoutEnd();
    for (; it != itEnd; it++)
    {
        const VertexAttribute& attribute = it->GetAttribute();
        int dstStride = it->GetStride();
        void* dstBuffer = &vertexData[it->GetOffset()];
        int srcStride = 0;
        const void* srcBuffer = GetVertexDataPointer(meshData, attribute.GetSemantic(), srcStride);
        assert(srcBuffer);
        CopyBuffer(dstBuffer, dstStride, srcBuffer, srcStride, meshData.mNumVertices, attribute.GetSize());
    }

    return vertexData;
}

std::vector<GLubyte> ModelLoader::CollectElementData(const aiMesh& meshData, Data::Type& elementType,
    std::vector<Drawcall::Primitive>& primitives, std::vector<int>& elementCounts)
{
    std::vector<GLubyte> elementData;

    elementType = ElementBufferObject::GetSmallestType(meshData.mNumVertices);
    int elementSize = Data::GetTypeSize(elementType);

    //Reserve max possible size
    elementData.reserve(meshData.mNumFaces * elementSize * 3);

    int numIndices = 0;
    for (unsigned int faceIndex = 0; faceIndex < meshData.mNumFaces; ++faceIndex)
    {
        aiFace& face = meshData.mFaces[faceIndex];

        int currentSize = static_cast<int>(elementData.size());
        elementData.resize(currentSize + face.mNumIndices * elementSize);

        void* dstBuffer = &elementData[currentSize];
        unsigned int dstStride = elementSize;
        const void* srcBuffer = face.mIndices;
        unsigned int srcStride = sizeof(*face.mIndices);

        if (std::endian::native == std::endian::big)
        {
            intptr_t srcAddress = reinterpret_cast<intptr_t>(srcBuffer);
            srcAddress += srcStride - elementSize;
            srcBuffer = reinterpret_cast<const void*>(srcAddress);
        }
        CopyBuffer(dstBuffer, dstStride, srcBuffer, srcStride, face.mNumIndices, elementSize);

        if (numIndices != face.mNumIndices)
        {
            numIndices = face.mNumIndices;
            primitives.push_back(GetPrimitiveType(face.mNumIndices));
            if (currentSize > 0)
            {
                elementCounts.push_back(currentSize);
            }
        }
    }
    elementCounts.push_back(static_cast<int>(elementData.size()));

    return elementData;
}

const void* ModelLoader::GetVertexDataPointer(const aiMesh& meshData, VertexAttribute::Semantic semantic, int& stride)
{
    const void* data = nullptr;
    switch (semantic)
    {
    case VertexAttribute::Semantic::Position:
        data = meshData.mVertices;
        stride = sizeof(*meshData.mVertices);
        break;
    case VertexAttribute::Semantic::Normal:
        data = meshData.mNormals;
        stride = sizeof(*meshData.mNormals);
        break;
    case VertexAttribute::Semantic::Tangent:
        data = meshData.mTangents;
        stride = sizeof(*meshData.mTangents);
        break;
    case VertexAttribute::Semantic::Bitangent:
        data = meshData.mBitangents;
        stride = sizeof(*meshData.mBitangents);
        break;
    case VertexAttribute::Semantic::TexCoord0:
    case VertexAttribute::Semantic::TexCoord1:
    case VertexAttribute::Semantic::TexCoord2:
    case VertexAttribute::Semantic::TexCoord3:
    case VertexAttribute::Semantic::TexCoord4:
    case VertexAttribute::Semantic::TexCoord5:
    case VertexAttribute::Semantic::TexCoord6:
    case VertexAttribute::Semantic::TexCoord7:
        {
            unsigned int texCoord0 = static_cast<unsigned int>(VertexAttribute::Semantic::TexCoord0);
            unsigned int texCoords = static_cast<unsigned int>(semantic);
            data = meshData.mTextureCoords[texCoords - texCoord0];
            stride = sizeof(*meshData.mTextureCoords[0]);
        }
        break;
    case VertexAttribute::Semantic::Color0:
    case VertexAttribute::Semantic::Color1:
    case VertexAttribute::Semantic::Color2:
    case VertexAttribute::Semantic::Color3:
    case VertexAttribute::Semantic::Color4:
    case VertexAttribute::Semantic::Color5:
    case VertexAttribute::Semantic::Color6:
    case VertexAttribute::Semantic::Color7:
        {
            unsigned int color0 = static_cast<unsigned int>(VertexAttribute::Semantic::Color0);
            unsigned int colors = static_cast<unsigned int>(semantic);
            data = meshData.mColors[colors - color0];
            stride = sizeof(*meshData.mColors[0]);
        }
        break;
    case VertexAttribute::Semantic::Unknown:
        // Do nothing
        break;
    }
    return data;
}

void ModelLoader::CopyBuffer(void* dstBuffer, size_t dstStride, const void* srcBuffer, size_t srcStride, size_t count, size_t size)
{
    if (srcStride == size && (dstStride == 0 || dstStride == srcStride))
    {
        memcpy(dstBuffer, srcBuffer, size * count);
    }
    else
    {
        const unsigned char* srcBytes = static_cast<const unsigned char*>(srcBuffer);
        unsigned char* dstBytes = static_cast<unsigned char*>(dstBuffer);
        for (int i = 0; i < count; ++i, srcBytes += srcStride, dstBytes += dstStride)
        {
            memcpy(dstBytes, srcBytes, size);
        }
    }
}

Drawcall::Primitive ModelLoader::GetPrimitiveType(int elementCount)
{
    Drawcall::Primitive primitive = Drawcall::Primitive::Invalid;
    switch (elementCount)
    {
    case 1:
        primitive = Drawcall::Primitive::Points;
    case 2:
        primitive = Drawcall::Primitive::Lines;
    case 3:
        primitive = Drawcall::Primitive::Triangles;
    }
    return primitive;
}
