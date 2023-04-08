#include "FirefliesApplication.h"

#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/asset/ModelLoader.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/shader/Material.h>
#include <ituGL/renderer/ForwardRenderPass.h>
#include <ituGL/renderer/GBufferRenderPass.h>
#include <ituGL/renderer/DeferredRenderPass.h>
#include <glm/gtx/transform.hpp>
#include <imgui.h>

FirefliesApplication::FirefliesApplication()
    : Application(1024, 1024, "Fireflies demo")
    , m_renderMode(RenderMode::Deferred)
    , m_renderer(GetDevice())
    , m_mouseClicked(false)
    , m_ambientColor(0.0f)
    , m_lightColor(0.0f)
    , m_lightIntensity(0.0f)
    , m_useRandomColor(false)
{
}

void FirefliesApplication::Initialize()
{
    Application::Initialize();

    // Initialize DearImGUI
    m_imGui.Initialize(GetMainWindow());

    InitializeForwardMaterials();
    InitializeDeferredMaterials();
    InitializeModels();
    InitializeCamera();
    InitializeLights();
    InitializeRenderer();

    DeviceGL& device = GetDevice();
    device.EnableFeature(GL_DEPTH_TEST);
    device.SetVSyncEnabled(true);
}

void FirefliesApplication::Update()
{
    Application::Update();

    // Update fireflies and send them to renderer
    UpdateFireflies();

    // Set current camera
    m_renderer.SetCurrentCamera(m_camera);
}

void FirefliesApplication::Render()
{
    Application::Render();

    // Clear color and depth
    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    m_renderer.Render();

    // Render the debug user interface
    RenderGUI();
}

void FirefliesApplication::Cleanup()
{
    // Cleanup DearImGUI
    m_imGui.Cleanup();

    Application::Cleanup();
}

Renderer::UpdateLightsFunction FirefliesApplication::GetUpdateLightsFunction(std::shared_ptr<ShaderProgram> shaderProgramPtr)
{
    // Get lighting related uniform locations
    ShaderProgram::Location ambientColorLocation = shaderProgramPtr->GetUniformLocation("AmbientColor");
    ShaderProgram::Location lightColorLocation = shaderProgramPtr->GetUniformLocation("LightColor");
    ShaderProgram::Location lightPositionLocation = shaderProgramPtr->GetUniformLocation("LightPosition");
    ShaderProgram::Location lightDirectionLocation = shaderProgramPtr->GetUniformLocation("LightDirection");
    ShaderProgram::Location lightAttenuationLocation = shaderProgramPtr->GetUniformLocation("LightAttenuation");

    return [=](const ShaderProgram& shaderProgram, std::span<const Light* const> lights, unsigned int& lightIndex) -> bool
    {
        bool needsRender = false;

        if (lightIndex == 0)
        {
            shaderProgram.SetUniform(ambientColorLocation, m_ambientColor);
            needsRender = true;
        }
        else
        {
            shaderProgram.SetUniform(ambientColorLocation, glm::vec3(0));
        }

        if (lightIndex < lights.size())
        {
            const Light& light = *lights[lightIndex];
            shaderProgram.SetUniform(lightColorLocation, light.GetColor() * light.GetIntensity());
            shaderProgram.SetUniform(lightPositionLocation, light.GetPosition());
            shaderProgram.SetUniform(lightDirectionLocation, light.GetDirection());
            shaderProgram.SetUniform(lightAttenuationLocation, light.GetAttenuation());
            needsRender = true;
        }
        else
        {
            // Disable light
            shaderProgram.SetUniform(lightColorLocation, glm::vec3(0.0f));
        }

        lightIndex++;

        return needsRender;
    };
}

void FirefliesApplication::InitializeForwardMaterials()
{
    // Load and build shader
    std::vector<const char*> vertexShaderPaths;
    vertexShaderPaths.push_back("shaders/version330.glsl");
    vertexShaderPaths.push_back("shaders/lit.vert");
    Shader vertexShader = ShaderLoader(Shader::VertexShader).Load(vertexShaderPaths);

    std::vector<const char*> fragmentShaderPaths;
    fragmentShaderPaths.push_back("shaders/version330.glsl");
    fragmentShaderPaths.push_back("shaders/utils.glsl");
    fragmentShaderPaths.push_back("shaders/blinn-phong.glsl");
    fragmentShaderPaths.push_back("shaders/lighting.glsl");
    fragmentShaderPaths.push_back("shaders/lit.frag");
    Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load(fragmentShaderPaths);

    std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram>();
    shaderProgramPtr->Build(vertexShader, fragmentShader);

    // Get transform related uniform locations
    ShaderProgram::Location cameraPositionLocation = shaderProgramPtr->GetUniformLocation("CameraPosition");
    ShaderProgram::Location worldMatrixLocation = shaderProgramPtr->GetUniformLocation("WorldMatrix");
    ShaderProgram::Location viewProjMatrixLocation = shaderProgramPtr->GetUniformLocation("ViewProjMatrix");

    // Register shader with renderer
    m_renderer.RegisterShaderProgram(shaderProgramPtr,
        [=](const ShaderProgram& shaderProgram, const glm::mat4& worldMatrix, const Camera& camera, bool cameraChanged)
        {
            if (cameraChanged)
            {
                shaderProgram.SetUniform(cameraPositionLocation, camera.ExtractTranslation());
                shaderProgram.SetUniform(viewProjMatrixLocation, camera.GetViewProjectionMatrix());
            }
            shaderProgram.SetUniform(worldMatrixLocation, worldMatrix);
        },
        GetUpdateLightsFunction(shaderProgramPtr)
        );

    // Filter out uniforms that are not material properties
    ShaderUniformCollection::NameSet filteredUniforms;
    filteredUniforms.insert("WorldMatrix");
    filteredUniforms.insert("ViewProjMatrix");
    filteredUniforms.insert("AmbientColor");
    filteredUniforms.insert("LightColor");
    filteredUniforms.insert("LightPosition");
    filteredUniforms.insert("LightDirection");
    filteredUniforms.insert("LightAttenuation");

    // Create reference material
    m_forwardMaterial = std::make_shared<Material>(shaderProgramPtr, filteredUniforms);
}

void FirefliesApplication::InitializeDeferredMaterials()
{
    // G-buffer material
    {
        // Load and build shader
        std::vector<const char*> vertexShaderPaths;
        vertexShaderPaths.push_back("shaders/version330.glsl");
        vertexShaderPaths.push_back("shaders/gbuffer.vert");
        Shader vertexShader = ShaderLoader(Shader::VertexShader).Load(vertexShaderPaths);

        std::vector<const char*> fragmentShaderPaths;
        fragmentShaderPaths.push_back("shaders/version330.glsl");
        fragmentShaderPaths.push_back("shaders/gbuffer.frag");
        Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load(fragmentShaderPaths);

        std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram>();
        shaderProgramPtr->Build(vertexShader, fragmentShader);

        // Get transform related uniform locations
        ShaderProgram::Location worldViewMatrixLocation = shaderProgramPtr->GetUniformLocation("WorldViewMatrix");
        ShaderProgram::Location worldViewProjMatrixLocation = shaderProgramPtr->GetUniformLocation("WorldViewProjMatrix");

        // Register shader with renderer
        m_renderer.RegisterShaderProgram(shaderProgramPtr,
            [=](const ShaderProgram& shaderProgram, const glm::mat4& worldMatrix, const Camera& camera, bool cameraChanged)
            {
                shaderProgram.SetUniform(worldViewMatrixLocation, camera.GetViewMatrix() * worldMatrix);
                shaderProgram.SetUniform(worldViewProjMatrixLocation, camera.GetViewProjectionMatrix() * worldMatrix);
            },
            nullptr
        );

        // Filter out uniforms that are not material properties
        ShaderUniformCollection::NameSet filteredUniforms;
        filteredUniforms.insert("WorldViewMatrix");
        filteredUniforms.insert("WorldViewProjMatrix");

        // Create material
        m_gbufferMaterial = std::make_shared<Material>(shaderProgramPtr, filteredUniforms);
    }

    // Deferred material
    {
        std::vector<const char*> vertexShaderPaths;
        vertexShaderPaths.push_back("shaders/version330.glsl");
        vertexShaderPaths.push_back("shaders/deferred.vert");
        Shader vertexShader = ShaderLoader(Shader::VertexShader).Load(vertexShaderPaths);

        std::vector<const char*> fragmentShaderPaths;
        fragmentShaderPaths.push_back("shaders/version330.glsl");
        fragmentShaderPaths.push_back("shaders/utils.glsl");
        fragmentShaderPaths.push_back("shaders/blinn-phong.glsl");
        fragmentShaderPaths.push_back("shaders/lighting.glsl");
        fragmentShaderPaths.push_back("shaders/deferred.frag");
        Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load(fragmentShaderPaths);

        std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram>();
        shaderProgramPtr->Build(vertexShader, fragmentShader);

        // Filter out uniforms that are not material properties
        ShaderUniformCollection::NameSet filteredUniforms;
        filteredUniforms.insert("InvProjMatrix");
        filteredUniforms.insert("WorldViewProjMatrix");

        // Get transform related uniform locations
        ShaderProgram::Location invViewMatrixLocation = shaderProgramPtr->GetUniformLocation("InvViewMatrix");
        ShaderProgram::Location invProjMatrixLocation = shaderProgramPtr->GetUniformLocation("InvProjMatrix");
        ShaderProgram::Location worldViewProjMatrixLocation = shaderProgramPtr->GetUniformLocation("WorldViewProjMatrix");

        // Register shader with renderer
        m_renderer.RegisterShaderProgram(shaderProgramPtr,
            [=](const ShaderProgram& shaderProgram, const glm::mat4& worldMatrix, const Camera& camera, bool cameraChanged)
            {
                if (cameraChanged)
                {
                    shaderProgram.SetUniform(invViewMatrixLocation, glm::inverse(camera.GetViewMatrix()));
                    shaderProgram.SetUniform(invProjMatrixLocation, glm::inverse(camera.GetProjectionMatrix()));
                }
                shaderProgram.SetUniform(worldViewProjMatrixLocation, camera.GetViewProjectionMatrix() * worldMatrix);
            },
            GetUpdateLightsFunction(shaderProgramPtr)
        );

        // Create material
        m_deferredMaterial = std::make_shared<Material>(shaderProgramPtr, filteredUniforms);
    }
}

void FirefliesApplication::InitializeModels()
{
    std::shared_ptr<Material> material = m_renderMode == RenderMode::Forward ? m_forwardMaterial : m_gbufferMaterial;

    material->SetUniformValue("Color", glm::vec3(1.0f));
    material->SetUniformValue("AmbientReflectance", 1.0f);
    material->SetUniformValue("DiffuseReflectance", 1.0f);
    material->SetUniformValue("SpecularReflectance", 1.0f);
    material->SetUniformValue("SpecularExponent", 100.0f);

    // Configure loader
    ModelLoader loader(material);
    loader.SetCreateMaterials(true);
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Position, "VertexPosition");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Normal, "VertexNormal");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::TexCoord0, "VertexTexCoord");
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::DiffuseColor, "Color");
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::DiffuseTexture, "ColorTexture");
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::SpecularExponent, "SpecularExponent");

    // Load models
    m_fireflyModel = loader.Load("models/firefly/firefly.obj");
    m_floorModel = loader.Load("models/floor/floor.obj");
}

void FirefliesApplication::InitializeCamera()
{
    // Set view matrix, from the camera position looking to the origin
    m_camera.SetViewMatrix(glm::vec3(0, 8, 0), glm::vec3(0.0f), glm::vec3(0, 0, -1));

    // Set perspective matrix
    float aspectRatio = GetMainWindow().GetAspectRatio();
    m_camera.SetPerspectiveProjectionMatrix(1.0f, aspectRatio, 0.1f, 1000.0f);
}

void FirefliesApplication::InitializeLights()
{
    // Initialize light variables
    m_ambientColor = glm::vec3(0.25f);
    m_lightColor = glm::vec3(1.0f, 0.8f, 0.3f);
    m_lightIntensity = 1.0f;
}

void FirefliesApplication::InitializeRenderer()
{
    switch (m_renderMode)
    {
    case RenderMode::Forward:
        m_renderer.AddRenderPass(std::make_unique<ForwardRenderPass>());
        break;
    case RenderMode::Deferred:
        {
            // Set up deferred passes
            int width, height;
            GetMainWindow().GetDimensions(width, height);
            std::unique_ptr<GBufferRenderPass> gbufferRenderPass(std::make_unique<GBufferRenderPass>(width, height));

            // Set the g-buffer textures as properties of the deferred material
            m_deferredMaterial->SetUniformValue("DepthTexture", gbufferRenderPass->GetDepthTexture());
            m_deferredMaterial->SetUniformValue("AlbedoTexture", gbufferRenderPass->GetAlbedoTexture());
            m_deferredMaterial->SetUniformValue("NormalTexture", gbufferRenderPass->GetNormalTexture());
            m_deferredMaterial->SetUniformValue("OthersTexture", gbufferRenderPass->GetOthersTexture());

            // Add the render passes
            m_renderer.AddRenderPass(std::move(gbufferRenderPass));
            m_renderer.AddRenderPass(std::make_unique<DeferredRenderPass>(m_deferredMaterial));
            break;
        }
    }
}

void FirefliesApplication::RenderGUI()
{
    m_imGui.BeginFrame();

    // Add debug controls for light properties
    ImGui::ColorEdit3("Ambient color", &m_ambientColor[0]);
    ImGui::Separator();
    ImGui::ColorEdit3("Light color", &m_lightColor[0]);
    ImGui::DragFloat("Light intensity", &m_lightIntensity, 0.05f, 0.0f, 100.0f);
    ImGui::Checkbox("Use random color", &m_useRandomColor);

    m_imGui.EndFrame();
}

void FirefliesApplication::UpdateFireflies()
{
    Window& window = GetMainWindow();
    if (!m_mouseClicked && window.IsMouseButtonPressed(Window::MouseButton::Right))
    {
        glm::vec2 mousePosition = GetMainWindow().GetMousePosition(true);
        AddFirefly(mousePosition);
        m_mouseClicked = true;
    }
    else if (m_mouseClicked && window.IsMouseButtonReleased(Window::MouseButton::Right))
    {
        m_mouseClicked = false;
    }

    m_renderer.AddModel(m_floorModel, glm::mat4(1.0f));
    for (Firefly& firefly : m_fireflies)
    {
        float deltaTime = GetDeltaTime();
        glm::mat4& worldMatrix = firefly.worldMatrix;

        // Rotate a random angle
        firefly.rotationSpeed += RandomRange(-0.25f, 0.25f);
        firefly.rotationSpeed = glm::clamp(firefly.rotationSpeed, -1.57f, 1.57f);
        worldMatrix = glm::rotate(worldMatrix, firefly.rotationSpeed * deltaTime, glm::vec3(0, 1, 0));

        // Advance in forward direction
        worldMatrix[3] += -worldMatrix[2] * 2.0f * deltaTime;

        // Copy the position to the light
        firefly.pointLight.SetPosition(worldMatrix[3]);

        m_renderer.AddModel(m_fireflyModel, firefly.worldMatrix);
        m_renderer.AddLight(firefly.pointLight);
    }
}

void FirefliesApplication::AddFirefly(glm::vec2 position2D)
{
    Firefly& firefly = m_fireflies.emplace_back();

    float scale = 4.0;
    glm::vec3 position3D(position2D.x * scale, RandomRange(0.8f, 1.0f), position2D.y * -scale);

    PointLight& pointLight = firefly.pointLight;
    pointLight.SetPosition(position3D);
    pointLight.SetColor(m_useRandomColor ? glm::vec3(RandomColor()) : m_lightColor);
    pointLight.SetIntensity(m_lightIntensity);
    pointLight.SetDistanceAttenuation(glm::vec2(1.0f, 2.0f));

    firefly.worldMatrix = glm::translate(position3D) * glm::rotate(RandomRange(-3.1416f, 3.1416f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.25f));

    firefly.rotationSpeed = 0.0f;
}

float FirefliesApplication::Random01()
{
    return static_cast<float>(rand()) / RAND_MAX;
}

float FirefliesApplication::RandomRange(float from, float to)
{
    return Random01() * (to - from) + from;
}

Color FirefliesApplication::RandomColor()
{
    return Color(Random01(), Random01(), Random01());
}
