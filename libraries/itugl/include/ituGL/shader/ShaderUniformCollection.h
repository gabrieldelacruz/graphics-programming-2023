#pragma once

#include <ituGL/shader/ShaderProgram.h>
#include <ituGL/texture/TextureObject.h>
#include <ituGL/core/Data.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

class ShaderUniformCollection
{
public:
    // Alias for a set of names
    using NameSet = std::unordered_set<std::string>;

public:
    ShaderUniformCollection();
    // Initialize with the shader program, will extract all the properties. Skip the names in filtered uniforms
    ShaderUniformCollection(std::shared_ptr<ShaderProgram> shaderProgram, const NameSet& filteredUniforms = NameSet());

    // Get the shader program
    std::shared_ptr<ShaderProgram> GetShaderProgram();
    std::shared_ptr<const ShaderProgram> GetShaderProgram() const;

    // Reset the material with a different shader
    void ChangeShader(std::shared_ptr<ShaderProgram> shaderProgram, const NameSet& filteredUniforms = NameSet());

    // Get the vertex attribute location by name
    ShaderProgram::Location GetAttributeLocation(const char* name) const;

    // Get the shader uniform location by name
    ShaderProgram::Location GetUniformLocation(const char* name) const;

    // Get uniform value for different types, using the name or the uniform location
    template<typename T>
    T GetUniformValue(const char* name) const;
    template<typename T>
    T GetUniformValue(ShaderProgram::Location location) const;
    template<typename T>
    void GetUniformValue(const char* name, T& value) const;
    template<typename T>
    void GetUniformValue(ShaderProgram::Location location, T& value) const;
    template<typename T>
    void GetUniformValue(ShaderProgram::Location location, std::shared_ptr<T>& value) const;
    template<typename T>
    void GetUniformValues(const char* name, std::span<T> value) const;
    template<typename T>
    void GetUniformValues(ShaderProgram::Location location, std::span<T> value) const;

    // Set uniform value for different types, using the name or the uniform location
    template<typename T>
    void SetUniformValue(const char* name, const T& value);
    template<typename T>
    void SetUniformValue(ShaderProgram::Location location, const T& value);
    template<typename T>
    void SetUniformValue(ShaderProgram::Location location, const std::shared_ptr<T>& value);
    template<typename T>
    void SetUniformValues(const char* name, std::span<const T> value);
    template<typename T>
    void SetUniformValues(ShaderProgram::Location location, std::span<const T> value);

    // Set all the properties to the shader. Requires the shader program to be in use
    void SetUniforms() const;

private:
    // Different dimensions of the properties
    enum class UniformDimension
    {
        Scalar,
        Vector2, Vector3, Vector4,
        VectorFirst = Vector2, VectorLast = Vector4,
        Matrix2x2, Matrix2x3, Matrix2x4,
        Matrix3x2, Matrix3x3, Matrix3x4,
        Matrix4x2, Matrix4x3, Matrix4x4,
        MatrixFirst = Matrix2x2, MatrixLast = Matrix4x4,
    };

    // Struct to store a data property
    struct DataUniform
    {
        // Uniform location
        ShaderProgram::Location location;
        // Data type
        Data::Type type;
        // Dimension of the data (scalar, vector, matrix)
        UniformDimension dimension;
        // Number of elements of the property
        unsigned int count;
        // Index in the data buffer
        int index;
    };

    // Struct to store a texture property
    struct TextureUniform
    {
        // Uniform location
        ShaderProgram::Location location;
        // Texture subtype
        TextureObject::Target target;
        // Shared pointer to the texture object
        std::shared_ptr<TextureObject> texture;
    };

private:
    // Get a data uniform
    DataUniform& GetDataUniform(ShaderProgram::Location location);
    const DataUniform& GetDataUniform(ShaderProgram::Location location) const;

    // Get a texture uniform
    TextureUniform& GetTextureUniform(ShaderProgram::Location location);
    const TextureUniform& GetTextureUniform(ShaderProgram::Location location) const;

    // Read all the uniforms in the shader and store them as properties
    // Can skip by name those in the filteredUniforms
    void ExtractUniforms(const NameSet& filteredUniforms = NameSet());

    // Check if an OpenGL type is a data type and, if so, return the data type and dimension
    static bool IsDataUniform(GLenum glType, Data::Type& type, UniformDimension& dimension);

    // Check if an OpenGL type is a texture and, if so, return the target type
    static bool IsTextureUniform(GLenum glType, TextureObject::Target& target);

    // Add uniform property
    void AddUniform(const DataUniform& uniform);
    template<typename T>
    void AddUniform(const DataUniform& uniform);
    void AddUniform(const TextureUniform& uniform);

    // Use uniform property
    void UseUniform(const DataUniform& uniform) const;
    template<typename T>
    void UseUniform(const DataUniform& uniform) const;
    void UseUniform(const TextureUniform& uniform) const;

    // Get the buffer where data values are stored for a certain type
    template<typename T>
    std::vector<T>& GetDataValues();
    template<typename T>
    const std::vector<T>& GetDataValues() const;

    // Get a span of values for a specific uniform
    template<typename T>
    std::span<T> GetDataValues(ShaderProgram::Location location);
    template<typename T>
    std::span<const T> GetDataValues(ShaderProgram::Location location) const;
    template<typename T>
    void GetDataValues(ShaderProgram::Location location, std::span<T>& values);
    template<typename T>
    void GetDataValues(ShaderProgram::Location location, std::span<const T>& values) const;
    template<typename T, int N>
    void GetDataValues(ShaderProgram::Location location, std::span<const glm::vec<N, T>>& values) const;
    template<typename T, int C, int R>
    void GetDataValues(ShaderProgram::Location location, std::span<const glm::mat<C, R, T>>& values) const;

    // Get the size of a data property
    int GetDataUniformSize(const DataUniform& uniform) const;

    // Delete all the properties and set the shader program to null
    void Reset();

#ifndef NDEBUG
    bool IsScalar(UniformDimension dimension) const;
    bool IsVector(UniformDimension dimension) const;
    bool IsMatrix(UniformDimension dimension) const;
    bool IsVectorSize(UniformDimension dimension, int size) const;
    bool IsMatrixSize(UniformDimension dimension, int columns, int rows) const;
#endif

protected:
    // The shader program
    std::shared_ptr<ShaderProgram> m_shaderProgram;

private:
    // The list of data properties
    std::vector<DataUniform> m_dataUniforms;
    // The list of texture properties
    std::vector<TextureUniform> m_textureUniforms;

    // Map to find data properties in the data list
    std::unordered_map<ShaderProgram::Location, int> m_locationDataIndex;
    // Map to find texture properties in the texture list
    std::unordered_map<ShaderProgram::Location, int> m_locationTextureIndex;

    // Buffers that store the values for data properties
    std::vector<int> m_intDataValues;
    std::vector<unsigned int> m_uintDataValues;
    std::vector<float> m_floatDataValues;
    std::vector<double> m_doubleDataValues;
};


template<typename T>
inline T ShaderUniformCollection::GetUniformValue(const char* name) const
{
    T value;
    GetUniformValue(name, value);
    return value;
}

template<typename T>
inline T ShaderUniformCollection::GetUniformValue(ShaderProgram::Location location) const
{
    T value;
    GetUniformValue(location, value);
    return value;
}

template<typename T>
inline void ShaderUniformCollection::GetUniformValue(const char* name, T& value) const
{
    ShaderProgram::Location location = GetUniformLocation(name);
    assert(location >= 0);
    return GetUniformValue(location, value);
}

template<typename T>
inline void ShaderUniformCollection::GetUniformValue(ShaderProgram::Location location, T& value) const
{
    GetUniformValue(location, std::span(&value, 1));
}

template<typename T>
void ShaderUniformCollection::GetUniformValue(ShaderProgram::Location location, std::shared_ptr<T>& value) const
{
    std::shared_ptr<TextureObject> textureValue;
    GetUniformValue(location, textureValue);
    value = textureValue;
}

template<>
void ShaderUniformCollection::GetUniformValue(ShaderProgram::Location location, std::shared_ptr<TextureObject>& value) const;

template<typename T>
inline void ShaderUniformCollection::GetUniformValues(const char* name, std::span<T> values) const
{
    ShaderProgram::Location location = GetUniformLocation(name);
    assert(location >= 0);
    return GetUniformValues(location, values);
}

template<typename T>
void ShaderUniformCollection::GetUniformValues(ShaderProgram::Location location, std::span<T> values) const
{
    std::span<const T> storedValues;
    GetDataValues(location, storedValues);
    assert(values.size() == storedValues.size());
    std::memcpy(values.data(), storedValues.data(), values.size_bytes());
}

template<typename T>
inline void ShaderUniformCollection::SetUniformValue(const char* name, const T& value)
{
    ShaderProgram::Location location = GetUniformLocation(name);
    //assert(location >= 0);
    if (location >= 0) // Replaced assert with silent skip
    {
        SetUniformValue(location, value);
    }
}

template<typename T>
inline void ShaderUniformCollection::SetUniformValue(ShaderProgram::Location location, const T& value)
{
    SetUniformValues(location, std::span(&value, 1));
}

template<typename T>
void ShaderUniformCollection::SetUniformValue(ShaderProgram::Location location, const std::shared_ptr<T>& value)
{
    std::shared_ptr<TextureObject> textureValue = value;
    SetUniformValue(location, textureValue);
}

template<>
void ShaderUniformCollection::SetUniformValue(ShaderProgram::Location location, const std::shared_ptr<TextureObject>& value);

template<typename T>
inline void ShaderUniformCollection::SetUniformValues(const char* name, std::span<const T> values)
{
    ShaderProgram::Location location = GetUniformLocation(name);
    assert(location >= 0);
    SetUniformValues(location, values);
}

template<typename T>
void ShaderUniformCollection::SetUniformValues(ShaderProgram::Location location, std::span<const T> values)
{
    std::span<T> storedValues;
    GetDataValues(location, storedValues);
    assert(values.size() == storedValues.size());
    std::memcpy(storedValues.data(), values.data(), values.size_bytes());
}

template<typename T>
inline std::vector<T>& ShaderUniformCollection::GetDataValues()
{
    return const_cast<std::vector<T>&>(const_cast<const ShaderUniformCollection*>(this)->GetDataValues<T>());
}

template<> inline const std::vector<int>& ShaderUniformCollection::GetDataValues() const { return m_intDataValues; }
template<> inline const std::vector<unsigned int>& ShaderUniformCollection::GetDataValues() const { return m_uintDataValues; }
template<> inline const std::vector<float>& ShaderUniformCollection::GetDataValues() const { return m_floatDataValues; }
template<> inline const std::vector<double>& ShaderUniformCollection::GetDataValues() const { return m_doubleDataValues; }

template<typename T>
inline std::span<T> ShaderUniformCollection::GetDataValues(ShaderProgram::Location location)
{
    std::span<const T> values = const_cast<const ShaderUniformCollection*>(this)->GetDataValues<T>(location);
    return std::span<T>(const_cast<T*>(values.data()), values.size());
}

template<typename T>
std::span<const T> ShaderUniformCollection::GetDataValues(ShaderProgram::Location location) const
{
    std::span<const T> values;
    GetDataValues(location, values);
    return values;
}

template<typename T>
void ShaderUniformCollection::GetDataValues(ShaderProgram::Location location, std::span<T>& values)
{
    std::span<const T> v;
    const_cast<const ShaderUniformCollection*>(this)->GetDataValues(location, v);
    values = std::span<T>(const_cast<T*>(v.data()), v.size());
}

template<typename T>
void ShaderUniformCollection::GetDataValues(ShaderProgram::Location location, std::span<const T>& values) const
{
    const DataUniform& uniform = GetDataUniform(location);
    assert(uniform.type == Data::GetType<T>());
    assert(IsScalar(uniform.dimension));
    const std::vector<T>& allValues = GetDataValues<T>();
    auto dataPtr = &allValues[uniform.index];
    values = std::span(dataPtr, uniform.count);
}

template<typename T, int N>
void ShaderUniformCollection::GetDataValues(ShaderProgram::Location location, std::span<const glm::vec<N, T>>& values) const
{
    const DataUniform& uniform = GetDataUniform(location);
    assert(uniform.type == Data::GetType<T>());
    assert(IsVector(uniform.dimension));
    assert(IsVectorSize(uniform.dimension, N));
    const std::vector<T>& allValues = GetDataValues<T>();
    auto dataPtr = reinterpret_cast<const glm::vec<N, T>*>(&allValues[uniform.index]);
    values = std::span(dataPtr, uniform.count);
}

template<typename T, int C, int R>
void ShaderUniformCollection::GetDataValues(ShaderProgram::Location location, std::span<const glm::mat<C, R, T>>& values) const
{
    const DataUniform& uniform = GetDataUniform(location);
    assert(uniform.type == Data::GetType<T>());
    assert(IsMatrix(uniform.dimension));
    assert(IsMatrixSize(uniform.dimension, C, R));
    const std::vector<T>& allValues = GetDataValues<T>();
    auto dataPtr = reinterpret_cast<const glm::mat<C, R, T>*>(&allValues[uniform.index]);
    values = std::span(dataPtr, uniform.count);
}

template<typename T>
void ShaderUniformCollection::AddUniform(const DataUniform& uniform)
{
    m_locationDataIndex.insert(std::make_pair(uniform.location, static_cast<int>(m_dataUniforms.size())));
    m_dataUniforms.push_back(uniform);

    std::vector<T>& values = GetDataValues<T>();
    m_dataUniforms.back().index = static_cast<int>(values.size());
    int size = GetDataUniformSize(uniform);
    values.insert(values.end(), size, T());
}

template<>
void ShaderUniformCollection::UseUniform<float>(const DataUniform& uniform) const;

template<typename T>
void ShaderUniformCollection::UseUniform(const DataUniform& uniform) const
{
    ShaderProgram::Location location = uniform.location;
    switch (uniform.dimension)
    {
    case UniformDimension::Scalar:
        m_shaderProgram->SetUniforms<T>(location, GetDataValues<T>(location));
        break;
    case UniformDimension::Vector2:
        m_shaderProgram->SetUniforms<T, 2>(location, GetDataValues<glm::vec<2, T>>(location));
        break;
    case UniformDimension::Vector3:
        m_shaderProgram->SetUniforms<T, 3>(location, GetDataValues<glm::vec<3, T>>(location));
        break;
    case UniformDimension::Vector4:
        m_shaderProgram->SetUniforms<T, 4>(location, GetDataValues<glm::vec<4, T>>(location));
        break;
    default:
        assert(false);
    }
}
