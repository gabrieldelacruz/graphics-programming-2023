#include <ituGL/scene/SceneNode.h>

#include <ituGL/scene/Scene.h>
#include <ituGL/scene/Transform.h>

SceneNode::SceneNode(const std::string& name) : SceneNode(name, std::make_shared<Transform>())
{
}

SceneNode::SceneNode(const std::string& name, std::shared_ptr<Transform> transform) : m_scene(nullptr), m_name(name), m_transform(transform)
{
}

SceneNode::~SceneNode()
{
}

const std::string& SceneNode::GetName() const
{
    return m_name;
}

void SceneNode::Rename(const std::string& name)
{
    std::shared_ptr<SceneNode> sceneNode;
    if (m_scene)
    {
        sceneNode = m_scene->GetSceneNode(m_name);
        assert(sceneNode.get() == this);
        m_scene->RemoveSceneNode(m_name);
    }
    m_name = name;
    if (m_scene)
    {
        m_scene->AddSceneNode(sceneNode);
    }
}

std::shared_ptr<Transform> SceneNode::GetTransform()
{
    return m_transform;
}

std::shared_ptr<const Transform> SceneNode::GetTransform() const
{
    return m_transform;
}

void SceneNode::SetTransform(std::shared_ptr<Transform> transform)
{
    m_transform = transform;
}

Scene* SceneNode::GetOwnerScene() const
{
    return m_scene;
}

void SceneNode::SetOwnerScene(Scene* scene)
{
    m_scene = scene;
}

SphereBounds SceneNode::GetSphereBounds() const
{
    return SphereBounds(glm::vec3(m_transform->GetTranslation()), 0.0f); // use world translation?
}

AabbBounds SceneNode::GetAabbBounds() const
{
    return AabbBounds(glm::vec3(m_transform->GetTranslation()), glm::vec3(0.0f)); // use world translation?
}

BoxBounds SceneNode::GetBoxBounds() const
{
    return BoxBounds(glm::vec3(m_transform->GetTranslation()), glm::mat3(1.0f), glm::vec3(0.0f)); // use world translation?
}

void SceneNode::AcceptVisitor(SceneVisitor& visitor)
{
}

void SceneNode::AcceptVisitor(SceneVisitor& visitor) const
{
}
