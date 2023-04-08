#include <ituGL/scene/Bounds.h>

SphereBounds::SphereBounds(const Bounds& bounds) : Bounds(bounds.GetCenter()), m_radius(0.0f)
{
    switch (bounds.GetType())
    {
    case Type::Sphere:
        m_radius = static_cast<const SphereBounds&>(bounds).GetRadius();
        break;
    case Type::AABB:
        m_radius = static_cast<const AabbBounds&>(bounds).GetSize().length();
        break;
    case Type::Box:
        m_radius = static_cast<const BoxBounds&>(bounds).GetSize().length();
        break;
    default:
        assert(false);
    }
}

AabbBounds::AabbBounds(const Bounds& bounds) : Bounds(bounds.GetCenter()), m_size(0.0f)
{
    switch (bounds.GetType())
    {
    case Type::Sphere:
        m_size = glm::vec3(static_cast<const SphereBounds&>(bounds).GetRadius());
        break;
    case Type::AABB:
        m_size = static_cast<const AabbBounds&>(bounds).GetSize();
        break;
    case Type::Box:
        {
            glm::mat3 scaledMatrix = static_cast<const BoxBounds&>(bounds).GetScaledMatrix();
            m_size = glm::abs(scaledMatrix[0]);
            m_size = glm::max(m_size, glm::abs(scaledMatrix[1]));
            m_size = glm::max(m_size, glm::abs(scaledMatrix[2]));
        }
        break;
    default:
        assert(false);
        break;
    }
}

BoxBounds::BoxBounds(const Bounds& bounds) : RotatedBounds(bounds.GetCenter(), glm::mat3(1.0f)), m_size(0.0f)
{
    switch (bounds.GetType())
    {
    case Type::Sphere:
        m_size = glm::vec3(static_cast<const SphereBounds&>(bounds).GetRadius());
        break;
    case Type::AABB:
        m_size = static_cast<const AabbBounds&>(bounds).GetSize();
        break;
    case Type::Box:
        m_rotationMatrix = static_cast<const BoxBounds&>(bounds).GetRotationMatrix();
        m_size = static_cast<const BoxBounds&>(bounds).GetSize();
        break;
    default:
        assert(false);
    }
}

template<>
bool Bounds::Intersects(const SphereBounds& boundsA, const SphereBounds& boundsB)
{
    return glm::distance(boundsA.GetCenter(), boundsB.GetCenter()) <= boundsA.GetRadius() + boundsB.GetRadius();
}

template<>
bool Bounds::Intersects(const AabbBounds& boundsA, const SphereBounds& boundsB)
{
    glm::vec3 closestPoint = boundsB.GetCenter();
    closestPoint = glm::max(boundsA.GetMin(), closestPoint);
    closestPoint = glm::min(boundsA.GetMax(), closestPoint);
    return glm::distance(closestPoint, boundsB.GetCenter()) <= boundsB.GetRadius();
}

template<>
bool Bounds::Intersects(const AabbBounds& boundsA, const AabbBounds& boundsB)
{
    glm::vec3 maxMin = glm::max(boundsA.GetMin(), boundsB.GetMin());
    glm::vec3 minMax = glm::min(boundsA.GetMax(), boundsB.GetMax());
    return maxMin.x <= minMax.x && maxMin.y <= minMax.y && maxMin.z <= minMax.z;
}

template<>
bool Bounds::Intersects(const BoxBounds& boundsA, const SphereBounds& boundsB)
{
    glm::mat3 inverseRotationMatrix = glm::transpose(boundsA.GetRotationMatrix());
    glm::vec3 localCenter = inverseRotationMatrix * (boundsB.GetCenter() - boundsA.GetCenter());
    SphereBounds localSphere(localCenter, boundsB.GetRadius());
    AabbBounds localBox(glm::vec3(0), boundsA.GetSize());
    return Bounds::Intersects(localBox, localSphere);
}

template<>
bool Bounds::Intersects(const BoxBounds& boundsA, const AabbBounds& boundsB)
{
    return Bounds::Intersects(boundsA, BoxBounds(boundsB.GetCenter(), glm::mat3(1.0f), boundsB.GetSize()));
}

bool TestSeparationAxis(const glm::vec3& axis, const glm::vec3& distance, const glm::mat3& mA, const glm::mat3& mB)
{
    float projDistance = std::abs(glm::dot(distance, axis));
    float projSize = 0.0f;
    for (int i = 0; i < 3; ++i)
    {
        projSize += std::abs(glm::dot(mA[i], axis));
        projSize += std::abs(glm::dot(mB[i], axis));
    }
    return projSize <= projDistance;
}

template<>
bool Bounds::Intersects(const BoxBounds& boundsA, const BoxBounds& boundsB)
{
    glm::vec3 distance = boundsB.GetCenter() - boundsA.GetCenter();
    glm::mat3 mA = boundsA.GetScaledMatrix();
    glm::mat3 mB = boundsA.GetScaledMatrix();
    return TestSeparationAxis(boundsA.GetXVector(), distance, mA, mB)
        && TestSeparationAxis(boundsA.GetYVector(), distance, mA, mB)
        && TestSeparationAxis(boundsA.GetZVector(), distance, mA, mB)
        && TestSeparationAxis(boundsB.GetXVector(), distance, mA, mB)
        && TestSeparationAxis(boundsB.GetYVector(), distance, mA, mB)
        && TestSeparationAxis(boundsB.GetZVector(), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetXVector(), boundsB.GetXVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetXVector(), boundsB.GetYVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetXVector(), boundsB.GetZVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetYVector(), boundsB.GetXVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetYVector(), boundsB.GetYVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetYVector(), boundsB.GetZVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetZVector(), boundsB.GetXVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetZVector(), boundsB.GetYVector()), distance, mA, mB)
        && TestSeparationAxis(glm::cross(boundsA.GetZVector(), boundsB.GetZVector()), distance, mA, mB);
}

template<>
bool Bounds::Intersects(const FrustumBounds& boundsA, const SphereBounds& boundsB)
{
    return true;
}

template<>
bool Bounds::Intersects(const FrustumBounds& boundsA, const AabbBounds& boundsB)
{
    return true;
}

template<>
bool Bounds::Intersects(const FrustumBounds& boundsA, const BoxBounds& boundsB)
{
    return true;
}

bool Bounds::Intersects(const Bounds& boundsA, const Bounds& boundsB)
{
    switch (boundsA.GetType())
    {
    case Type::Sphere:
        return Bounds::Intersects(static_cast<const SphereBounds&>(boundsA), boundsB);
    case Type::AABB:
        return Bounds::Intersects(static_cast<const AabbBounds&>(boundsA), boundsB);
    case Type::Box:
        return Bounds::Intersects(static_cast<const BoxBounds&>(boundsA), boundsB);
    default:
        assert(false);
        return false;
    }
}


glm::mat3 BoxBounds::GetScaledMatrix() const
{
    return glm::mat3(
        m_rotationMatrix[0] * m_size[0],
        m_rotationMatrix[1] * m_size[1],
        m_rotationMatrix[2] * m_size[2]
    );
}
