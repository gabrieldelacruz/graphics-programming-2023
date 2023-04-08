#include <ituGL/scene/SceneModel.h>

#include <ituGL/geometry/Model.h>
#include <ituGL/geometry/Mesh.h>
#include <ituGL/scene/Transform.h>
#include <ituGL/scene/SceneVisitor.h>
#include <cassert>

SceneModel::SceneModel(const std::string& name, std::shared_ptr<Model> model) : SceneNode(name), m_model(model)
{
}

SceneModel::SceneModel(const std::string& name, std::shared_ptr<Model> model, std::shared_ptr<Transform> transform) : SceneNode(name, transform), m_model(model)
{
}

std::shared_ptr<Model> SceneModel::GetModel() const
{
    return m_model;
}

void SceneModel::SetModel(std::shared_ptr<Model> model)
{
    m_model = model;
}

/*glm::mat4 SceneModel::GetWorldMatrix() const
{
    return m_transform ? m_transform->GetTransformMatrix() : glm::mat4(1.0f);
}

int SceneModel::GetDrawcallCount() const
{
    return m_model ? m_model->GetMesh().GetSubmeshCount() : 0;
}

const Drawcall& SceneModel::GetDrawcall(int index, const VertexArrayObject*& vao, const Material* &material) const
{
    assert(m_model);
    material = &m_model->GetMaterial(index);
    const Mesh& mesh = m_model->GetMesh();
    vao = &mesh.GetSubmeshVertexArray(index);
    return mesh.GetSubmeshDrawcall(index);
}*/

SphereBounds SceneModel::GetSphereBounds() const
{
    return SphereBounds(GetBoxBounds());
}

AabbBounds SceneModel::GetAabbBounds() const
{
    return AabbBounds(GetBoxBounds());
}

BoxBounds SceneModel::GetBoxBounds() const
{
    assert(m_transform);
    assert(m_model);
    return BoxBounds(m_transform->GetTranslation(), m_transform->GetRotationMatrix(), m_transform->GetScale() /* * m_model->GetSize()*/);
}

void SceneModel::AcceptVisitor(SceneVisitor& visitor)
{
    visitor.VisitModel(*this);
    //visitor.VisitRenderable(*this);
}

void SceneModel::AcceptVisitor(SceneVisitor& visitor) const
{
    visitor.VisitModel(*this);
    //visitor.VisitRenderable(*this);
}
