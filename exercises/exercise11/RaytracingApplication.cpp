#include "RaytracingApplication.h"

#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/camera/Camera.h>
#include <ituGL/scene/SceneCamera.h>
#include <ituGL/scene/Transform.h>
#include <ituGL/lighting/DirectionalLight.h>
#include <ituGL/shader/Material.h>
#include <ituGL/texture/Texture2DObject.h>
#include <ituGL/texture/FramebufferObject.h>
#include <ituGL/renderer/PostFXRenderPass.h>
#include <ituGL/scene/RendererSceneVisitor.h>
#include <imgui.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

RaytracingApplication::RaytracingApplication()
    : Application(1024, 1024, "Ray-tracing demo")
    , m_renderer(GetDevice())
    , m_frameCount(0)
    , m_sphereCenter(-3, 0, 0)
    , m_boxMatrix(glm::translate(glm::vec3(3, 0, 0)))
{
}

void RaytracingApplication::Initialize()
{
    Application::Initialize();

    // Initialize DearImGUI
    m_imGui.Initialize(GetMainWindow());

    InitializeCamera();
    InitializeMaterial();
    InitializeFramebuffer();
    InitializeRenderer();
}

void RaytracingApplication::Update()
{
    Application::Update();

    // Update camera controller
    m_cameraController.Update(GetMainWindow(), GetDeltaTime());

    // Invalidate accumulation when moving the camera
    if (m_cameraController.IsEnabled())
    {
        InvalidateScene();
    }

    // Set renderer camera
    const Camera& camera = *m_cameraController.GetCamera()->GetCamera();
    m_renderer.SetCurrentCamera(camera);

    // Update the material properties
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    m_material->SetUniformValue("ViewMatrix", viewMatrix);
    m_material->SetUniformValue("ProjMatrix", camera.GetProjectionMatrix());
    m_material->SetUniformValue("InvProjMatrix", glm::inverse(camera.GetProjectionMatrix()));
    m_material->SetUniformValue("SphereCenter", glm::vec3(viewMatrix * glm::vec4(m_sphereCenter, 1.0f)));
    m_material->SetUniformValue("BoxMatrix", viewMatrix * m_boxMatrix);
    m_material->SetUniformValue("FrameCount", ++m_frameCount);
}

void RaytracingApplication::Render()
{
    Application::Render();

    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    // Render the scene
    m_renderer.Render();

    // Render the debug user interface
    RenderGUI();
}

void RaytracingApplication::Cleanup()
{
    // Cleanup DearImGUI
    m_imGui.Cleanup();

    Application::Cleanup();
}

void RaytracingApplication::InvalidateScene()
{
    m_frameCount = 0;
}

void RaytracingApplication::InitializeCamera()
{
    // Create the main camera
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->SetViewMatrix(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
    float fov = 1.57f;
    camera->SetPerspectiveProjectionMatrix(fov, GetMainWindow().GetAspectRatio(), 0.1f, 100.0f);

    // Create a scene node for the camera
    std::shared_ptr<SceneCamera> sceneCamera = std::make_shared<SceneCamera>("camera", camera);

    // Set the camera scene node to be controlled by the camera controller
    m_cameraController.SetCamera(sceneCamera);
}

void RaytracingApplication::InitializeMaterial()
{
    m_material = CreateRaytracingMaterial("shaders/exercise11.glsl");

    // Initialize material uniforms
    m_material->SetUniformValue("SphereCenter", m_sphereCenter);
    m_material->SetUniformValue("SphereRadius", 1.25f);
    m_material->SetUniformValue("SphereColor", glm::vec3(0, 0, 1));
    m_material->SetUniformValue("BoxMatrix", m_boxMatrix);
    m_material->SetUniformValue("BoxSize", glm::vec3(1, 1, 1));
    m_material->SetUniformValue("BoxColor", glm::vec3(1, 0, 0));
    m_material->SetUniformValue("LightColor", glm::vec3(1.0f));
    m_material->SetUniformValue("LightIntensity", 4.0f);
    m_material->SetUniformValue("LightSize", glm::vec2(3.0f));

    // (todo) 11.3: Enable blending and set the blending parameters to alpha blending
    m_material->SetBlendEquation(Material::BlendEquation::None);
}

void RaytracingApplication::InitializeFramebuffer()
{
    int width, height;
    GetMainWindow().GetDimensions(width, height);

    // Scene Texture
    m_sceneTexture = std::make_shared<Texture2DObject>();
    m_sceneTexture->Bind();
    m_sceneTexture->SetImage(0, width, height, TextureObject::FormatRGBA, TextureObject::InternalFormat::InternalFormatRGBA16F);
    m_sceneTexture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_LINEAR);
    m_sceneTexture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_LINEAR);
    Texture2DObject::Unbind();

    // Scene framebuffer
    m_sceneFramebuffer = std::make_shared<FramebufferObject>();
    m_sceneFramebuffer->Bind();
    m_sceneFramebuffer->SetTexture(FramebufferObject::Target::Draw, FramebufferObject::Attachment::Color0, *m_sceneTexture);
    m_sceneFramebuffer->SetDrawBuffers(std::array<FramebufferObject::Attachment, 1>({ FramebufferObject::Attachment::Color0 }));
    FramebufferObject::Unbind();
}

void RaytracingApplication::InitializeRenderer()
{
    m_renderer.AddRenderPass(std::make_unique<PostFXRenderPass>(m_material, m_sceneFramebuffer));

    std::shared_ptr<Material> copyMaterial = CreateCopyMaterial();
    copyMaterial->SetUniformValue("SourceTexture", m_sceneTexture);
    m_renderer.AddRenderPass(std::make_unique<PostFXRenderPass>(copyMaterial, m_renderer.GetDefaultFramebuffer()));
}

std::shared_ptr<Material> RaytracingApplication::CreateRaytracingMaterial(const char* fragmentShaderPath)
{
    // We could keep this vertex shader and reuse it, but it looks simpler this way
    std::vector<const char*> vertexShaderPaths;
    vertexShaderPaths.push_back("shaders/version330.glsl");
    vertexShaderPaths.push_back("shaders/renderer/fullscreen.vert");
    Shader vertexShader = ShaderLoader(Shader::VertexShader).Load(vertexShaderPaths);

    std::vector<const char*> fragmentShaderPaths;
    fragmentShaderPaths.push_back("shaders/version330.glsl");
    fragmentShaderPaths.push_back("shaders/utils.glsl");
    fragmentShaderPaths.push_back("shaders/transform.glsl");
    fragmentShaderPaths.push_back("shaders/raytracer.glsl");
    fragmentShaderPaths.push_back("shaders/raylibrary.glsl");
    fragmentShaderPaths.push_back(fragmentShaderPath);
    fragmentShaderPaths.push_back("shaders/raytracing.frag");
    Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load(fragmentShaderPaths);

    std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram>();
    shaderProgramPtr->Build(vertexShader, fragmentShader);

    // Create material
    std::shared_ptr<Material> material = std::make_shared<Material>(shaderProgramPtr);
    
    return material;
}

std::shared_ptr<Material> RaytracingApplication::CreateCopyMaterial()
{
    std::vector<const char*> vertexShaderPaths;
    vertexShaderPaths.push_back("shaders/version330.glsl");
    vertexShaderPaths.push_back("shaders/renderer/fullscreen.vert");
    Shader vertexShader = ShaderLoader(Shader::VertexShader).Load(vertexShaderPaths);

    std::vector<const char*> fragmentShaderPaths;
    fragmentShaderPaths.push_back("shaders/version330.glsl");
    fragmentShaderPaths.push_back("shaders/utils.glsl");
    fragmentShaderPaths.push_back("shaders/renderer/copy.frag");
    Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load(fragmentShaderPaths);

    std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram>();
    shaderProgramPtr->Build(vertexShader, fragmentShader);

    // Create material
    std::shared_ptr<Material> material = std::make_shared<Material>(shaderProgramPtr);

    return material;
}

void RaytracingApplication::RenderGUI()
{
    m_imGui.BeginFrame();

    bool changed = false;

    if (auto window = m_imGui.UseWindow("Scene parameters"))
    {
        if (ImGui::TreeNodeEx("Sphere", ImGuiTreeNodeFlags_DefaultOpen))
        {
            changed |= ImGui::DragFloat3("Center", &m_sphereCenter[0], 0.1f);
            changed |= ImGui::DragFloat("Radius", m_material->GetDataUniformPointer<float>("SphereRadius"), 0.1f);
            changed |= ImGui::ColorEdit3("Color", m_material->GetDataUniformPointer<float>("SphereColor"));
            
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Box", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static glm::vec3 translation(3, 0, 0);
            static glm::vec3 rotation(0.0f);

            changed |= ImGui::DragFloat3("Translation", &translation[0], 0.1f);
            changed |= ImGui::DragFloat3("Rotation", &rotation[0], 0.1f);
            changed |= ImGui::DragFloat3("Size", m_material->GetDataUniformPointer<float>("BoxSize"), 0.1f);
            changed |= ImGui::ColorEdit3("Color", m_material->GetDataUniformPointer<float>("BoxColor"));
            m_boxMatrix = glm::translate(translation) * glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            changed |= ImGui::DragFloat2("Size", m_material->GetDataUniformPointer<float>("LightSize"), 0.1f);
            changed |= ImGui::DragFloat("Intensity", m_material->GetDataUniformPointer<float>("LightIntensity"), 0.1f);
            changed |= ImGui::ColorEdit3("Color", m_material->GetDataUniformPointer<float>("LightColor"));

            ImGui::TreePop();
        }
    }

    if (changed)
    {
        InvalidateScene();
    }

    m_imGui.EndFrame();
}
