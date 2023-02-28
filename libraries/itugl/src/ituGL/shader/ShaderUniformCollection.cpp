#include <ituGL/shader/ShaderUniformCollection.h>
#include <cassert>
#include <array>

ShaderUniformCollection::ShaderUniformCollection() : m_shaderProgram(nullptr)
{
}

ShaderUniformCollection::ShaderUniformCollection(std::shared_ptr<ShaderProgram> shaderProgram, const NameSet& filteredUniforms) : m_shaderProgram(shaderProgram)
{
    ExtractUniforms(filteredUniforms);
}

std::shared_ptr<ShaderProgram> ShaderUniformCollection::GetShaderProgram()
{
    return m_shaderProgram;
}

std::shared_ptr<const ShaderProgram> ShaderUniformCollection::GetShaderProgram() const
{
    return m_shaderProgram;
}

void ShaderUniformCollection::ChangeShader(std::shared_ptr<ShaderProgram> shaderProgram, const NameSet& filteredUniforms)
{
    Reset();
    m_shaderProgram = shaderProgram;
    ExtractUniforms(filteredUniforms);
}

ShaderProgram::Location ShaderUniformCollection::GetAttributeLocation(const char* name) const
{
    return m_shaderProgram->GetAttributeLocation(name);
}

ShaderProgram::Location ShaderUniformCollection::GetUniformLocation(const char* name) const
{
    return m_shaderProgram->GetUniformLocation(name);
}

ShaderUniformCollection::DataUniform& ShaderUniformCollection::GetDataUniform(ShaderProgram::Location location)
{
    return const_cast<DataUniform&>(const_cast<const ShaderUniformCollection*>(this)->GetDataUniform(location));
}

const ShaderUniformCollection::DataUniform& ShaderUniformCollection::GetDataUniform(ShaderProgram::Location location) const
{
    int uniformIndex = m_locationDataIndex.at(location);
    const DataUniform& uniform = m_dataUniforms[uniformIndex];
    assert(uniform.location == location);
    return uniform;
}

ShaderUniformCollection::TextureUniform& ShaderUniformCollection::GetTextureUniform(ShaderProgram::Location location)
{
    return const_cast<TextureUniform&>(const_cast<const ShaderUniformCollection*>(this)->GetTextureUniform(location));
}

const ShaderUniformCollection::TextureUniform& ShaderUniformCollection::GetTextureUniform(ShaderProgram::Location location) const
{
    int uniformIndex = m_locationTextureIndex.at(location);
    const TextureUniform& uniform = m_textureUniforms[uniformIndex];
    assert(uniform.location == location);
    return uniform;
}

void ShaderUniformCollection::ExtractUniforms(const NameSet& filteredUniforms)
{
    assert(m_shaderProgram);

    ShaderProgram& shaderProgram = *m_shaderProgram;

    unsigned int uniformCount = shaderProgram.GetUniformCount();

    // Loop over all the uniforms
    for (unsigned int i = 0; i < uniformCount; ++i)
    {
        // Get the information of uniform in position i
        int size;
        GLenum glType;
        char uniformName[256];
        shaderProgram.GetUniformInfo(i, size, glType, std::span(uniformName, sizeof(uniformName)));

        // If the named is in the filtered list, skip
        if (filteredUniforms.contains(uniformName))
            continue;

        // Get the uniform location
        ShaderProgram::Location location = GetUniformLocation(uniformName);
        assert(location >= 0);

        Data::Type type;
        UniformDimension dimension;
        TextureObject::Target target;
        if (IsDataUniform(glType, type, dimension))
        {
            // If it is a data property, store as data
            DataUniform uniform;
            uniform.location = location;
            uniform.type = type;
            uniform.dimension = dimension;
            uniform.count = size;
            AddUniform(uniform);
        }
        else if (IsTextureUniform(glType, target))
        {
            // If it is a texture property, store as property
            TextureUniform uniform;
            uniform.location = location;
            uniform.target = target;
            AddUniform(uniform);
        }
        else
        {
            // Unsupported uniform type
            assert(false);
        }
    }
}

bool ShaderUniformCollection::IsDataUniform(GLenum glType, Data::Type& type, UniformDimension& dimension)
{
    // Type
    switch (glType)
    {
    case GL_INT:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
        type = Data::Type::Int;
        break;
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_VEC2:
    case GL_UNSIGNED_INT_VEC3:
    case GL_UNSIGNED_INT_VEC4:
        type = Data::Type::UInt;
        break;
    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT2x3:
    case GL_FLOAT_MAT2x4:
    case GL_FLOAT_MAT3x2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT3x4:
    case GL_FLOAT_MAT4x2:
    case GL_FLOAT_MAT4x3:
    case GL_FLOAT_MAT4:
        type = Data::Type::Float;
        break;
    case GL_DOUBLE:
    case GL_DOUBLE_VEC2:
    case GL_DOUBLE_VEC3:
    case GL_DOUBLE_VEC4:
        type = Data::Type::Int;
        break;
    default:
        return false;
    }

    // UniformDimension
    switch (glType)
    {
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
    case GL_DOUBLE:
        dimension = UniformDimension::Scalar;
        break;
    case GL_INT_VEC2:
    case GL_UNSIGNED_INT_VEC2:
    case GL_FLOAT_VEC2:
    case GL_DOUBLE_VEC2:
        dimension = UniformDimension::Vector2;
        break;
    case GL_INT_VEC3:
    case GL_UNSIGNED_INT_VEC3:
    case GL_FLOAT_VEC3:
    case GL_DOUBLE_VEC3:
        dimension = UniformDimension::Vector3;
        break;
    case GL_INT_VEC4:
    case GL_UNSIGNED_INT_VEC4:
    case GL_FLOAT_VEC4:
    case GL_DOUBLE_VEC4:
        dimension = UniformDimension::Vector4;
        break;
    case GL_FLOAT_MAT2:
        dimension = UniformDimension::Matrix2x2;
        break;
    case GL_FLOAT_MAT2x3:
        dimension = UniformDimension::Matrix2x3;
        break;
    case GL_FLOAT_MAT2x4:
        dimension = UniformDimension::Matrix2x4;
        break;
    case GL_FLOAT_MAT3x2:
        dimension = UniformDimension::Matrix3x2;
        break;
    case GL_FLOAT_MAT3:
        dimension = UniformDimension::Matrix3x3;
        break;
    case GL_FLOAT_MAT3x4:
        dimension = UniformDimension::Matrix3x4;
        break;
    case GL_FLOAT_MAT4x2:
        dimension = UniformDimension::Matrix4x2;
        break;
    case GL_FLOAT_MAT4x3:
        dimension = UniformDimension::Matrix4x3;
        break;
    case GL_FLOAT_MAT4:
        dimension = UniformDimension::Matrix4x4;
        break;
    default:
        return false;
    }
    return true;
}

bool ShaderUniformCollection::IsTextureUniform(GLenum glType, TextureObject::Target& target)
{
    switch (glType)
    {
    case GL_SAMPLER_1D:
        target = TextureObject::Target::Texture1D;
        break;
    case GL_SAMPLER_1D_ARRAY:
        target = TextureObject::Target::Texture1DArray;
        break;
    case GL_SAMPLER_2D:
        target = TextureObject::Target::Texture2D;
        break;
    case GL_SAMPLER_2D_ARRAY:
        target = TextureObject::Target::Texture2DArray;
        break;
    case GL_SAMPLER_2D_MULTISAMPLE:
        target = TextureObject::Target::Texture2DMultisample;
        break;
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
        target = TextureObject::Target::Texture2DMultisampleArray;
        break;
    case GL_SAMPLER_3D:
        target = TextureObject::Target::Texture3D;
        break;
    case GL_SAMPLER_CUBE:
        target = TextureObject::Target::TextureCubemap;
        break;
    case GL_SAMPLER_CUBE_MAP_ARRAY:
        target = TextureObject::Target::TextureCubemapArray;
        break;
    default:
        return false;
    }
    return true;
}

void ShaderUniformCollection::AddUniform(const DataUniform& uniform)
{
    switch (uniform.type)
    {
    case Data::Type::Int:
        AddUniform<int>(uniform);
        break;
    case Data::Type::UInt:
        AddUniform<unsigned int>(uniform);
        break;
    case Data::Type::Float:
        AddUniform<float>(uniform);
        break;
    case Data::Type::Double:
        AddUniform<double>(uniform);
        break;
    default:
        assert(false);
    }
}

void ShaderUniformCollection::AddUniform(const TextureUniform& uniform)
{
    m_locationTextureIndex.insert(std::make_pair(uniform.location, static_cast<int>(m_textureUniforms.size())));
    m_textureUniforms.push_back(uniform);
}

void ShaderUniformCollection::SetUniforms() const
{
    for (const DataUniform& uniform : m_dataUniforms)
    {
        UseUniform(uniform);
    }
    for (const TextureUniform& uniform : m_textureUniforms)
    {
        UseUniform(uniform);
    }
}

void ShaderUniformCollection::UseUniform(const DataUniform& uniform) const
{
    switch (uniform.type)
    {
    case Data::Type::Int:
        UseUniform<int>(uniform);
        break;
    case Data::Type::UInt:
        UseUniform<unsigned int>(uniform);
        break;
    case Data::Type::Float:
        UseUniform<float>(uniform);
        break;
    case Data::Type::Double:
        UseUniform<double>(uniform);
        break;
    default:
        assert(false);
    }
}

void ShaderUniformCollection::UseUniform(const TextureUniform& uniform) const
{
    //TODO: default texture
    if (uniform.texture)
    {
        size_t textureIndex = &uniform - m_textureUniforms.data();
        m_shaderProgram->SetTexture(uniform.location, static_cast<int>(textureIndex), *uniform.texture);
    }
}

template<>
void ShaderUniformCollection::UseUniform<float>(const DataUniform& uniform) const
{
    ShaderProgram::Location location = uniform.location;
    switch (uniform.dimension)
    {
    case UniformDimension::Scalar:
        m_shaderProgram->SetUniforms<float>(location, GetDataValues<float>(location));
        break;
    case UniformDimension::Vector2:
        m_shaderProgram->SetUniforms<float, 2>(location, GetDataValues<glm::vec<2,float>>(location));
        break;
    case UniformDimension::Vector3:
        m_shaderProgram->SetUniforms<float, 3>(location, GetDataValues<glm::vec<3, float>>(location));
        break;
    case UniformDimension::Vector4:
        m_shaderProgram->SetUniforms<float, 4>(location, GetDataValues<glm::vec<4, float>>(location));
        break;
    case UniformDimension::Matrix2x2:
        m_shaderProgram->SetUniforms<float, 2, 2>(location, GetDataValues<glm::mat<2, 2, float>>(location));
        break;
    case UniformDimension::Matrix2x3:
        m_shaderProgram->SetUniforms<float, 2, 3>(location, GetDataValues<glm::mat<2, 3, float>>(location));
        break;
    case UniformDimension::Matrix2x4:
        m_shaderProgram->SetUniforms<float, 2, 4>(location, GetDataValues<glm::mat<2, 4, float>>(location));
        break;
    case UniformDimension::Matrix3x2:
        m_shaderProgram->SetUniforms<float, 3, 2>(location, GetDataValues<glm::mat<3, 2, float>>(location));
        break;
    case UniformDimension::Matrix3x3:
        m_shaderProgram->SetUniforms<float, 3, 3>(location, GetDataValues<glm::mat<3, 3, float>>(location));
        break;
    case UniformDimension::Matrix3x4:
        m_shaderProgram->SetUniforms<float, 3, 4>(location, GetDataValues<glm::mat<3, 4, float>>(location));
        break;
    case UniformDimension::Matrix4x2:
        m_shaderProgram->SetUniforms<float, 4, 2>(location, GetDataValues<glm::mat<4, 2, float>>(location));
        break;
    case UniformDimension::Matrix4x3:
        m_shaderProgram->SetUniforms<float, 4, 3>(location, GetDataValues<glm::mat<4, 3, float>>(location));
        break;
    case UniformDimension::Matrix4x4:
        m_shaderProgram->SetUniforms<float, 4, 4>(location, GetDataValues<glm::mat<4, 4, float>>(location));
        break;
    default:
        assert(false);
    }
}

template<>
void ShaderUniformCollection::GetUniformValue(ShaderProgram::Location location, std::shared_ptr<TextureObject>& value) const
{
    const TextureUniform& uniform = GetTextureUniform(location);
    value = uniform.texture;
}

template<>
void ShaderUniformCollection::SetUniformValue(ShaderProgram::Location location, const std::shared_ptr<TextureObject>& value)
{
    TextureUniform& uniform = GetTextureUniform(location);
    assert(!value || uniform.target == value->GetTarget());
    uniform.texture = value;
}

int ShaderUniformCollection::GetDataUniformSize(const DataUniform& uniform) const
{
    int size = 0;
    switch (uniform.dimension)
    {
    case UniformDimension::Scalar:
        size = 1;
        break;
    case UniformDimension::Vector2:
        size = 2;
        break;
    case UniformDimension::Vector3:
        size = 3;
        break;
    case UniformDimension::Vector4:
    case UniformDimension::Matrix2x2:
        size = 4;
        break;
    case UniformDimension::Matrix2x3:
    case UniformDimension::Matrix3x2:
        size = 6;
        break;
    case UniformDimension::Matrix2x4:
    case UniformDimension::Matrix4x2:
        size = 8;
        break;
    case UniformDimension::Matrix3x3:
        size = 9;
        break;
    case UniformDimension::Matrix3x4:
    case UniformDimension::Matrix4x3:
        size = 12;
        break;
    case UniformDimension::Matrix4x4:
        size = 16;
        break;
    default:
        assert(false);
    }
    return size * uniform.count;
}

void ShaderUniformCollection::Reset()
{
    m_shaderProgram = nullptr;
    m_dataUniforms.clear();
    m_textureUniforms.clear();
    m_locationDataIndex.clear();
    m_locationTextureIndex.clear();
    m_intDataValues.clear();
    m_uintDataValues.clear();
    m_floatDataValues.clear();
    m_doubleDataValues.clear();
}

#ifndef NDEBUG
bool ShaderUniformCollection::IsScalar(UniformDimension dimension) const
{
    return dimension == UniformDimension::Scalar;
}

bool ShaderUniformCollection::IsVector(UniformDimension dimension) const
{
    return dimension >= UniformDimension::VectorFirst && dimension <= UniformDimension::VectorLast;
}

bool ShaderUniformCollection::IsMatrix(UniformDimension dimension) const
{
    return dimension >= UniformDimension::MatrixFirst && dimension <= UniformDimension::MatrixLast;
}

bool ShaderUniformCollection::IsVectorSize(UniformDimension dimension, int size) const
{
    assert(size >= 2 && size <= 4);
    return IsVector(dimension) && static_cast<int>(dimension) == static_cast<int>(UniformDimension::VectorFirst) + size - 2;
}

bool ShaderUniformCollection::IsMatrixSize(UniformDimension dimension, int columns, int rows) const
{
    assert(columns >= 2 && columns <= 4);
    assert(rows >= 2 && rows <= 4);
    int offset = (columns - 2) * 3 + (rows - 2);
    return IsMatrix(dimension) && static_cast<int>(dimension) == static_cast<int>(UniformDimension::MatrixFirst) + offset;
}
#endif
