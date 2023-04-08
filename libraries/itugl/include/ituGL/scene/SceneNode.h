#pragma once

#include <ituGL/scene/Bounds.h>
#include <string>
#include <memory>

class Scene;
class SceneVisitor;
class Transform;

class SceneNode
{
public:
    SceneNode(const std::string& name);
    SceneNode(const std::string& name, std::shared_ptr<Transform> transform);

    virtual ~SceneNode();

    const std::string& GetName() const;
    void Rename(const std::string& name);

    std::shared_ptr<Transform> GetTransform();
    std::shared_ptr<const Transform> GetTransform() const;
    void SetTransform(std::shared_ptr<Transform> transform);

    virtual SphereBounds GetSphereBounds() const;
    virtual AabbBounds GetAabbBounds() const;
    virtual BoxBounds GetBoxBounds() const;

    virtual void AcceptVisitor(SceneVisitor& visitor);
    virtual void AcceptVisitor(SceneVisitor& visitor) const;

private:
    friend class Scene;

    Scene* GetOwnerScene() const;
    void SetOwnerScene(Scene* scene);

    Scene* m_scene;

protected:
    std::string m_name;
    std::shared_ptr<Transform> m_transform;
};
