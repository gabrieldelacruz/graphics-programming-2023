#pragma once

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

class Bounds
{
public:
    enum class Type
    {
        Sphere,
        AABB,
        Box,
        Frustum
    };

public:
    Bounds(const glm::vec3& center) : m_center(center) {}

    virtual Type GetType() const = 0;

    inline const glm::vec3& GetCenter() const { return m_center; }
    inline void SetCenter(const glm::vec3& center) { m_center = center; }

    template<typename T>
    bool Intersects(const T& other) const;

    template<typename TA, typename TB>
    static bool Intersects(const TA& boundsA, const TB& boundsB);
    template<typename TA>
    static bool Intersects(const TA& boundsA, const Bounds& boundsB);
    template<typename TB>
    static bool Intersects(const Bounds& boundsA, const TB& boundsB);
    static bool Intersects(const Bounds& boundsA, const Bounds& boundsB);

protected:
    glm::vec3 m_center;
};

class SphereBounds : public Bounds
{
public:
    SphereBounds(const glm::vec3& center, float radius) : Bounds(center), m_radius(radius) {}
    SphereBounds(const Bounds& bounds);

    inline Type GetType() const override { return Type::Sphere; }

    inline float GetRadius() const { return m_radius; }
    inline void SetRadius(float radius) { m_radius = radius; }

private:
    float m_radius;
};

class AabbBounds : public Bounds
{
public:
    AabbBounds(const glm::vec3& center, const glm::vec3& size) : Bounds(center), m_size(size) {}
    AabbBounds(const Bounds& bounds);

    inline Type GetType() const override { return Type::AABB; }

    inline const glm::vec3& GetSize() const { return m_size; }
    inline void SetSize(const glm::vec3& size) { m_size = size; }

    glm::vec3 GetMin() const { return m_center - m_size; }
    glm::vec3 GetMax() const { return m_center + m_size; }

private:
    glm::vec3 m_size;
};

class RotatedBounds : public Bounds
{
public:
    RotatedBounds(const glm::vec3& center, const glm::mat3& rotationMatrix) : Bounds(center), m_rotationMatrix(rotationMatrix) {}

    inline const glm::mat3& GetRotationMatrix() const { return m_rotationMatrix; }
    inline void SetRotationMatrix(const glm::mat3& rotationMatrix) { m_rotationMatrix = rotationMatrix; }

    inline const glm::vec3& GetXVector() const { return m_rotationMatrix[0]; }
    inline const glm::vec3& GetYVector() const { return m_rotationMatrix[1]; }
    inline const glm::vec3& GetZVector() const { return m_rotationMatrix[2]; }

protected:
    glm::mat3 m_rotationMatrix;
};

class BoxBounds : public RotatedBounds
{
public:
    BoxBounds(const glm::vec3& center, const glm::mat3& rotationMatrix, const glm::vec3& size) : RotatedBounds(center, rotationMatrix), m_size(size) {}
    BoxBounds(const Bounds& bounds);

    inline Type GetType() const override { return Type::Box; }

    inline const glm::vec3& GetSize() const { return m_size; }
    inline void SetSize(const glm::vec3& size) { m_size = size; }

    glm::mat3 GetScaledMatrix() const;

private:
    glm::vec3 m_size;
};

class FrustumBounds : public RotatedBounds
{
public:
    FrustumBounds(const glm::vec3& center, const glm::vec3& size);

    inline Type GetType() const override { return Type::Frustum; }

private:
};


template<typename T>
bool Bounds::Intersects(const T& other) const
{
    Bounds::Intersects(this, other);
}

template<typename TA, typename TB>
bool Bounds::Intersects(const TA& boundsA, const TB& boundsB)
{
    assert(boundsA.GetType() < boundsB.GetType());
    return Bounds::Intersects(boundsB, boundsA);
}

template<typename TA>
bool Bounds::Intersects(const TA& boundsA, const Bounds& boundsB)
{
    return Bounds::Intersects(boundsB, boundsA);
}

template<typename TB>
bool Bounds::Intersects(const Bounds& boundsA, const TB& boundsB)
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

template<>
bool Bounds::Intersects(const SphereBounds& boundsA, const SphereBounds& boundsB);
template<>
bool Bounds::Intersects(const AabbBounds& boundsA, const SphereBounds& boundsB);
template<>
bool Bounds::Intersects(const AabbBounds& boundsA, const AabbBounds& boundsB);
template<>
bool Bounds::Intersects(const BoxBounds& boundsA, const SphereBounds& boundsB);
template<>
bool Bounds::Intersects(const BoxBounds& boundsA, const AabbBounds& boundsB);
template<>
bool Bounds::Intersects(const BoxBounds& boundsA, const BoxBounds& boundsB);
template<>
bool Bounds::Intersects(const FrustumBounds& boundsA, const SphereBounds& boundsB);
template<>
bool Bounds::Intersects(const FrustumBounds& boundsA, const AabbBounds& boundsB);
template<>
bool Bounds::Intersects(const FrustumBounds& boundsA, const BoxBounds& boundsB);
//template<>
//bool Bounds::Intersects(const FrustumBounds& boundsA, const FrustumBounds& boundsB);



