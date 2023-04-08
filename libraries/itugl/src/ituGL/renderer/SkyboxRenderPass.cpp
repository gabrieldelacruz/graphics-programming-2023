#include <ituGL/renderer/SkyboxRenderPass.h>

#include <ituGL/renderer/Renderer.h>
#include <ituGL/geometry/Mesh.h>
#include <ituGL/camera/Camera.h>
#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/texture/TextureCubemapObject.h>

SkyboxRenderPass::SkyboxRenderPass(std::shared_ptr<TextureCubemapObject> texture)
    : m_texture(texture)
    , m_cameraPositionLocation(-1)
    , m_invViewProjMatrixLocation(-1)
    , m_skyboxTextureLocation(-1)
{
    // Load shaders and build shader program
    Shader vertexShader = ShaderLoader(Shader::VertexShader).Load("shaders/renderer/skybox.vert");
    Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load("shaders/renderer/skybox.frag");
    m_shaderProgram.Build(vertexShader, fragmentShader);

    // Get uniform locations
    m_cameraPositionLocation = m_shaderProgram.GetUniformLocation("CameraPosition");
    m_invViewProjMatrixLocation = m_shaderProgram.GetUniformLocation("InvViewProjMatrix");
    m_skyboxTextureLocation = m_shaderProgram.GetUniformLocation("SkyboxTexture");
}

std::shared_ptr<TextureCubemapObject> SkyboxRenderPass::GetTexture() const
{
    return m_texture;
}

void SkyboxRenderPass::SetTexture(std::shared_ptr<TextureCubemapObject> texture)
{
    m_texture = texture;
}

void SkyboxRenderPass::Render()
{
    Renderer& renderer = GetRenderer();

    m_shaderProgram.Use();

    const Camera& camera = renderer.GetCurrentCamera();
    m_shaderProgram.SetUniform(m_cameraPositionLocation, camera.ExtractTranslation());
    m_shaderProgram.SetUniform(m_invViewProjMatrixLocation, glm::inverse(camera.GetViewProjectionMatrix()));
    m_shaderProgram.SetTexture(m_skyboxTextureLocation, 0, *m_texture);

    // Only write to depth == 1
    glDepthFunc(GL_EQUAL);

    const Mesh& fullscreenMesh = renderer.GetFullscreenMesh();
    fullscreenMesh.DrawSubmesh(0);
    
    // Restore default value
    glDepthFunc(GL_LESS);
}
