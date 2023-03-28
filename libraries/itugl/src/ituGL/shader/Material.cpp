#include <ituGL/shader/Material.h>
#include <ituGL/core/DeviceGL.h>
#include <cassert>

Material::Material() : Material(nullptr)
{
}

Material::Material(std::shared_ptr<ShaderProgram> shaderProgram, const NameSet& filteredUniforms)
    : ShaderUniformCollection(shaderProgram, filteredUniforms)
    , m_depthTestFunction(TestFunction::Less)
    , m_depthWrite(true)
    , m_stencilTestFunctions{ TestFunction::Never, TestFunction::Never }
    , m_stencilRefValues{ 0, 0 }
    , m_stencilMasks{ ~0u, ~0u }
    , m_stencilFail{ StencilOperation::Keep, StencilOperation::Keep }
    , m_stencilDepthFail{ StencilOperation::Keep, StencilOperation::Keep }
    , m_stencilDepthPass{ StencilOperation::Keep, StencilOperation::Keep }
    , m_blendEquations{ BlendEquation::None }
    , m_blendParams{ BlendParam::One, BlendParam::Zero, BlendParam::One, BlendParam::Zero }
{
}

void Material::SetShaderSetupFunction(ShaderSetupFunction shaderSetupFunction)
{
    m_shaderSetupFunction = shaderSetupFunction;
}

Material::TestFunction Material::GetDepthTestFunction() const
{
    return m_depthTestFunction;
}

void Material::SetDepthTestFunction(TestFunction function)
{
    m_depthTestFunction = function;
}

bool Material::GetDepthWrite() const
{
    return m_depthWrite;
}

void Material::SetDepthWrite(bool depthWrite)
{
    m_depthWrite = depthWrite;
}

void Material::SetStencilTestFunction(TestFunction function, int refValue, unsigned int mask)
{
    SetStencilFrontTestFunction(function, refValue, mask);
    SetStencilBackTestFunction(function, refValue, mask);
}

Material::TestFunction Material::GetStencilFrontTestFunction(int &refValue, unsigned int &mask) const
{
    refValue = m_stencilRefValues[0];
    mask = m_stencilMasks[0];
    return m_stencilTestFunctions[0];
}

void Material::SetStencilFrontTestFunction(TestFunction function, int refValue, unsigned int mask)
{
    m_stencilTestFunctions[0] = function;
    m_stencilRefValues[0] = refValue;
    m_stencilMasks[0] = mask;
}

Material::TestFunction Material::GetStencilBackTestFunction(int& refValue, unsigned int& mask) const
{
    refValue = m_stencilRefValues[1];
    mask = m_stencilMasks[1];
    return m_stencilTestFunctions[1];
}

void Material::SetStencilBackTestFunction(TestFunction function, int refValue, unsigned int mask)
{
    m_stencilTestFunctions[1] = function;
    m_stencilRefValues[1] = refValue;
    m_stencilMasks[1] = mask;
}

void Material::SetStencilOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass)
{
    SetStencilFrontOperations(stencilFail, depthFail, depthPass);
    SetStencilBackOperations(stencilFail, depthFail, depthPass);
}

void Material::GetStencilFrontOperations(StencilOperation& stencilFail, StencilOperation& depthFail, StencilOperation& depthPass) const
{
    stencilFail = m_stencilFail[0];
    depthFail = m_stencilDepthFail[0];
    depthPass = m_stencilDepthPass[0];
}

void Material::SetStencilFrontOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass)
{
    m_stencilFail[0] = stencilFail;
    m_stencilDepthFail[0] = depthFail;
    m_stencilDepthPass[0] = depthPass;
}

void Material::GetStencilBackOperations(StencilOperation& stencilFail, StencilOperation& depthFail, StencilOperation& depthPass) const
{
    stencilFail = m_stencilFail[1];
    depthFail = m_stencilDepthFail[1];
    depthPass = m_stencilDepthPass[1];
}

void Material::SetStencilBackOperations(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation depthPass)
{
    m_stencilFail[1] = stencilFail;
    m_stencilDepthFail[1] = depthFail;
    m_stencilDepthPass[1] = depthPass;
}

Material::BlendEquation Material::GetBlendEquationColor() const
{
    return m_blendEquations[0];
}

Material::BlendEquation Material::GetBlendEquationAlpha() const
{
    return m_blendEquations[1];
}

void Material::SetBlendEquation(BlendEquation blendEquation)
{
    SetBlendEquation(blendEquation, blendEquation);
}

void Material::SetBlendEquation(BlendEquation blendEquationColor, BlendEquation blendEquationAlpha)
{
    m_blendEquations[0] = blendEquationColor;
    m_blendEquations[1] = blendEquationAlpha;
}

void Material::SetBlendParams(BlendParam source, BlendParam dest)
{
    SetBlendParams(source, dest, source, dest);
}

void Material::SetBlendParams(BlendParam source, BlendParam dest, Color blendColor)
{
    SetBlendParams(source, dest, source, dest, blendColor);
}

void Material::SetBlendParams(BlendParam sourceColor, BlendParam destColor, BlendParam sourceAlpha, BlendParam destAlpha)
{
    m_blendParams[0] = sourceColor;
    m_blendParams[1] = destColor;
    m_blendParams[2] = sourceAlpha;
    m_blendParams[3] = destAlpha;
}

void Material::SetBlendParams(BlendParam sourceColor, BlendParam destColor, BlendParam sourceAlpha, BlendParam destAlpha, Color blendColor)
{
    SetBlendParams(sourceColor, destColor, sourceAlpha, destAlpha);
    SetBlendColor(blendColor);
}

void Material::SetBlendColor(Color blendColor)
{
    // Check that at least one of the parameters is ConstantColor or ConstantAlpha
    assert(m_blendParams[0] == BlendParam::ConstantColor || m_blendParams[0] == BlendParam::ConstantAlpha
        || m_blendParams[1] == BlendParam::ConstantColor || m_blendParams[1] == BlendParam::ConstantAlpha
        || m_blendParams[2] == BlendParam::ConstantColor || m_blendParams[2] == BlendParam::ConstantAlpha
        || m_blendParams[3] == BlendParam::ConstantColor || m_blendParams[3] == BlendParam::ConstantAlpha);

    m_blendColor = blendColor;
}

void Material::Use(OverrideFlags overrideFlags) const
{
    assert(m_shaderProgram);

    // Set the shader program as the one currently in use
    m_shaderProgram->Use();

    // Set the value of all the uniforms stored as properties
    SetUniforms();

    if (m_shaderSetupFunction)
    {
        // if needed, do extra set up for the shader
        m_shaderSetupFunction(*m_shaderProgram);
    }

    // If not skipped, set the depth settings
    if ((overrideFlags & OverrideFlags::OverrideDepthTest) == 0)
    {
        UseDepthTest();
    }

    // If not skipped, set the stencil settings
    if ((overrideFlags & OverrideFlags::OverrideStencilTest) == 0)
    {
        UseStencilTest();
    }

    // If not skipped, set the blend settings
    if ((overrideFlags & OverrideFlags::OverrideBlend) == 0)
    {
        UseBlend();
    }
}

void Material::UseDepthTest() const
{
    // Depth function
    glDepthFunc(static_cast<GLenum>(m_depthTestFunction));

    // Depth write
    glDepthMask(m_depthWrite ? GL_TRUE : GL_FALSE);
}

void Material::UseStencilTest() const
{
    // Stencil operations
    if (m_stencilFail[0] == m_stencilFail[1] && m_stencilDepthFail[0] == m_stencilDepthFail[1] && m_stencilDepthPass[0] == m_stencilDepthPass[1])
    {
        // Same for front and back
        glStencilOp(static_cast<GLenum>(m_stencilFail[0]), static_cast<GLenum>(m_stencilDepthFail[0]), static_cast<GLenum>(m_stencilDepthPass[0]));
    }
    else
    {
        // Separate functions for front and back
        glStencilOpSeparate(GL_FRONT, static_cast<GLenum>(m_stencilFail[0]), static_cast<GLenum>(m_stencilDepthFail[0]), static_cast<GLenum>(m_stencilDepthPass[0]));
        glStencilOpSeparate(GL_BACK, static_cast<GLenum>(m_stencilFail[1]), static_cast<GLenum>(m_stencilDepthFail[1]), static_cast<GLenum>(m_stencilDepthPass[1]));
    }

    // Stencil functions
    if (m_stencilTestFunctions[0] == m_stencilTestFunctions[1] && m_stencilRefValues[0] == m_stencilRefValues[1] && m_stencilMasks[0] == m_stencilMasks[1])
    {
        // Same for front and back
        glStencilFunc(static_cast<GLenum>(m_stencilTestFunctions[0]), m_stencilRefValues[0], m_stencilMasks[0]);
    }
    else
    {
        // Separate functions for front and back
        glStencilFuncSeparate(GL_FRONT, static_cast<GLenum>(m_stencilTestFunctions[0]), m_stencilRefValues[0], m_stencilMasks[0]);
        glStencilFuncSeparate(GL_BACK, static_cast<GLenum>(m_stencilTestFunctions[1]), m_stencilRefValues[1], m_stencilMasks[1]);
    }
}

void Material::UseBlend() const
{
    // If the blend equation is None for color and alpha, do nothing
    bool blending = m_blendEquations[0] != BlendEquation::None || m_blendEquations[1] != BlendEquation::None;
    DeviceGL::GetInstance().SetFeatureEnabled(GL_BLEND, blending);
    if (blending)
    {
        std::array<BlendParam, 4> blendParams = m_blendParams;

        // Set blend equation
        if (m_blendEquations[0] == m_blendEquations[1])
        {
            // Set the same blend equation for color and alpha
            glBlendEquation(static_cast<GLenum>(m_blendEquations[0]));
        }
        else
        {
            GLenum blendEquationColor = static_cast<GLenum>(m_blendEquations[0]);
            GLenum blendEquationAlpha = static_cast<GLenum>(m_blendEquations[1]);

            // Because there is no "None" equation, we replace it with (Source * 1 + Dest * 0)
            if (m_blendEquations[0] == BlendEquation::None)
            {
                blendEquationColor = GL_FUNC_ADD;
                blendParams[0] = BlendParam::One;
                blendParams[1] = BlendParam::Zero;
            }
            if (m_blendEquations[1] == BlendEquation::None)
            {
                blendEquationAlpha = GL_FUNC_ADD;
                blendParams[2] = BlendParam::One;
                blendParams[3] = BlendParam::Zero;
            }

            // Set separate blend equation for color and alpha
            glBlendEquationSeparate(blendEquationColor, blendEquationAlpha);
        }

        // Set blend params
        if (blendParams[0] == blendParams[2] && blendParams[1] == blendParams[3])
        {
            // Set the same blend params for color and alpha
            glBlendFunc(static_cast<GLenum>(blendParams[0]), static_cast<GLenum>(blendParams[1]));
        }
        else
        {
            // Set separate blend params for color and alpha
            glBlendFuncSeparate(
                static_cast<GLenum>(blendParams[0]), static_cast<GLenum>(blendParams[1]),
                static_cast<GLenum>(blendParams[2]), static_cast<GLenum>(blendParams[3]));
        }

        // Set blend color only if one param is using constant color or constant alpha
        if (blendParams[0] == BlendParam::ConstantColor || blendParams[0] == BlendParam::ConstantAlpha ||
            blendParams[1] == BlendParam::ConstantColor || blendParams[1] == BlendParam::ConstantAlpha ||
            blendParams[2] == BlendParam::ConstantColor || blendParams[2] == BlendParam::ConstantAlpha ||
            blendParams[3] == BlendParam::ConstantColor || blendParams[3] == BlendParam::ConstantAlpha)
        {
            glBlendColor(m_blendColor.GetRed(), m_blendColor.GetGreen(), m_blendColor.GetBlue(), m_blendColor.GetAlpha());
        }
    }
}
