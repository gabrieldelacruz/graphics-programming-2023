#pragma once

#include <ituGL/shader/ShaderUniformCollection.h>

#include <ituGL/core/Color.h>
#include <functional>
#include <array>

// Class to group all the properties that may affect the look of a rendered geometry
class Material : public ShaderUniformCollection
{
public:
    // Flags to skip setting blending, depth or stencil properties and use current value
    enum OverrideFlags
    {
        NoOverride = 0,
        OverrideBlend = 1 << 0,
        OverrideDepthTest = 1 << 1,
        OverrideStencilTest = 1 << 2
    };

    // Different conditions for depth and stencil tests
    enum class TestFunction : GLenum;

    // Operation to perform when a stencil condition is met
    enum class StencilOperation : GLenum;

    // Different types of blending equations
    enum class BlendEquation : GLenum;

    // Parameters to be used to control each of the 4 components of the blending equation:
    // source color, destination color, source alpha, destination alpha
    enum class BlendParam : GLenum;

    // Function pointer to prepare the shader used by the material that is being rendered
    using ShaderSetupFunction = std::function<void(ShaderProgram&)>;

public:
    Material();
    // Initialize with the shader program, will extract all the properties. Skip the names in filtered uniforms
    Material(std::shared_ptr<ShaderProgram> shaderProgram, const NameSet& filteredUniforms = NameSet());


    // The function that will be executed for additional shader program setup
    void SetShaderSetupFunction(ShaderSetupFunction shaderSetupFunction);


    // The test function for the depth test, if depth test is enabled
    TestFunction GetDepthTestFunction() const;
    void SetDepthTestFunction(TestFunction function);

    // If the geometry should write to depth buffer, if depth test is enabled
    bool GetDepthWrite() const;
    void SetDepthWrite(bool depthWrite);


    // Set the test function to use for stencil, front and back
    // refValue: the value we compare against
    // mask: bit mask applied to both stencil buffer and refValue before comparing
    void SetStencilTestFunction(TestFunction function, int refValue, unsigned int mask);

    // Set the test function to use for stencil, only for front faces
    TestFunction GetStencilFrontTestFunction(int& refValue, unsigned int& mask) const;
    void SetStencilFrontTestFunction(TestFunction function, int refValue, unsigned int mask);

    // Set the test function to use for stencil, only for back faces
    TestFunction GetStencilBackTestFunction(int& refValue, unsigned int& mask) const;
    void SetStencilBackTestFunction(TestFunction function, int refValue, unsigned int mask);

    // Set the stencil operations, front and back
    void SetStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass);

    // Set the stencil operations, only for front faces
    void GetStencilFrontOperations(StencilOperation &stencilFail, StencilOperation &depthFail, StencilOperation &depthPass) const;
    void SetStencilFrontOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass);

    // Set the stencil operations, only for back faces
    void GetStencilBackOperations(StencilOperation& stencilFail, StencilOperation& depthFail, StencilOperation& depthPass) const;
    void SetStencilBackOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass);

    // Set the same blend equation for color and alpha. By default and most common: Add
    BlendEquation GetBlendEquationColor() const;
    BlendEquation GetBlendEquationAlpha() const;
    void SetBlendEquation(BlendEquation blendEquation);

    // Set separate blend equation for color and alpha. By default and most common: Add
    void SetBlendEquation(BlendEquation blendEquationColor, BlendEquation blendEquationAlpha);

    // Set the parameters for the blend equation, same for color and alpha
    void SetBlendParams(BlendParam source, BlendParam dest);

    // Set the parameters for the blend equation, same for color and alpha, plus the blend color to use with ConstantColor param
    void SetBlendParams(BlendParam source, BlendParam dest, Color blendColor);

    // Set the parameters for the blend equation, different for color and alpha
    void SetBlendParams(BlendParam sourceColor, BlendParam destColor, BlendParam sourceAlpha, BlendParam destAlpha);

    // Set the parameters for the blend equation, different for color and alpha, plus the blend color to use with ConstantColor param
    void SetBlendParams(BlendParam sourceColor, BlendParam destColor, BlendParam sourceAlpha, BlendParam destAlpha, Color blendColor);

    // Set the blend color to use with ConstantColor param
    void SetBlendColor(Color blendColor);


    // Use the shader program, set all uniforms, set depth properties, stencil properties, and blending
    // You can skip depth, stencil or blending using the override flags
    void Use(OverrideFlags overrideFlags = OverrideFlags::NoOverride) const;

private:
    // Set all the properties relative to depth
    void UseDepthTest() const;

    // Set all the properties relative to stencil
    void UseStencilTest() const;

    // Set all the properties relative to blending
    void UseBlend() const;

private:
    // Function pointer to prepare the shader used by the material
    ShaderSetupFunction m_shaderSetupFunction;

    // Test function for depth. Default: Less
    TestFunction m_depthTestFunction;

    // If it should write to depth or not. Default: True
    bool m_depthWrite;

    // Test functions for front and back stencil. Default: Never
    std::array<TestFunction, 2> m_stencilTestFunctions;

    // Ref values for front and back stencil. Default: 0
    std::array<int, 2> m_stencilRefValues;

    // Mask for front and back stencil. Default: ~0
    std::array<unsigned int, 2> m_stencilMasks;

    // Stencil operation to perform if stencil test fails, front and back. Default: Keep
    std::array<StencilOperation, 2> m_stencilFail;

    // Stencil operation to perform if depth test fails, front and back. Default: Keep
    std::array<StencilOperation, 2> m_stencilDepthFail;

    // Stencil operation to perform if depth test passes, front and back. Default: Keep
    std::array<StencilOperation, 2>m_stencilDepthPass;

    // Blend equation for color and alpha. Default: None
    std::array<BlendEquation, 2> m_blendEquations;

    // Blend parameters for source color, destination color, source alpha and destination alpha
    // Default: One, Zero, One, Zero
    std::array<BlendParam, 4> m_blendParams;

    // Blend color to use with ConstantColor or ConstantAlpha parameters. Default: white
    Color m_blendColor;
};

// Different conditions for depth and stencil tests
enum class Material::TestFunction : GLenum
{
    Always = GL_ALWAYS,
    Never = GL_NEVER,
    Equal = GL_EQUAL,
    NotEqual = GL_NOTEQUAL,
    Less = GL_LESS,
    LessEqual = GL_LEQUAL,
    Greater = GL_GREATER,
    GreaterEqual = GL_GEQUAL
};

// Operation to perform when a stencil condition is met
enum class Material::StencilOperation : GLenum
{
    // Keep the same value (do nothing)
    Keep = GL_KEEP,
    // Set the value to zero (only the bits affected by the mask)
    Zero = GL_ZERO,
    // Replace the value with the reference value (only the bits affected by the mask)
    Replace = GL_REPLACE,
    // Increase the value. If the maximum is reached, do nothing
    Increase = GL_INCR,
    // Increase the value. If the maximum is reached, back to 0
    IncreaseWrap = GL_INCR_WRAP,
    // Decrease the value. If 0 is reached, do nothing
    Decrease = GL_DECR,
    // Decrease the value. If 0 is reached, set to maximum value
    DecreaseWrap = GL_DECR_WRAP,
    // Invert the bits affected by the mask
    Invert = GL_INVERT
};

// Different types of blending equations
enum class Material::BlendEquation : GLenum
{
    None = GL_NONE,
    Add = GL_FUNC_ADD,
    Substract = GL_FUNC_SUBTRACT,
    ReverseSubstract = GL_FUNC_REVERSE_SUBTRACT,
    Min = GL_MIN,
    Max = GL_MAX
};

// Parameters to be used to control each of the 4 components of the blending equation:
// source color, destination color, source alpha, destination alpha
enum class Material::BlendParam : GLenum
{
    Zero = GL_ZERO,
    One = GL_ONE,
    SourceColor = GL_SRC_COLOR,
    OneMinusSourceColor = GL_ONE_MINUS_SRC_COLOR,
    SourceAlpha = GL_SRC_ALPHA,
    OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
    DestColor = GL_DST_COLOR,
    OneMinusDestColor = GL_ONE_MINUS_DST_COLOR,
    DestAlpha = GL_DST_ALPHA,
    OneMinusDestAlpha = GL_ONE_MINUS_DST_ALPHA,
    ConstantColor = GL_CONSTANT_COLOR,
    OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
    ConstantAlpha = GL_CONSTANT_ALPHA,
    OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA
};
