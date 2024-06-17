#pragma once

#include "Components/Placement2D.h"
#include "Components/CollisionShape2D.h"



namespace Project001
{
    struct CollisionData2D
    {
        CollisionData2D()
            : myShapeTag((unsigned int)-1)
            , otherEntityId((unsigned int)-1)
            , otherShapeTag((unsigned int)-1)
            , point(NAN, NAN)
            , normal(NAN, NAN)
            , depth(NAN)
        {}

        unsigned int myShapeTag;

        unsigned int otherEntityId;
        unsigned int otherShapeTag;

        glm::vec2 point;
        glm::vec2 normal;
        float depth;
    };

    class CollisionBody2D : public Placement2D
    {
    public:
        enum class PhysicsType
        {
            PHYSICS_TYPE_OVERLAP_ONLY,
            PHYSICS_TYPE_REGULAR_PHYSICS
        };

        // Note that Points, Lines, Rays, LineSegments, and Polygons
        // (not ConvexPolygons) do not have regular phusics interactions. They
        // only have overlap detection.

        CollisionBody2D();

        // For the collision system to count a collision, at least one group in the
        // collisionGroupMask of each body needs to be in the other body's 
        // allowedCollisionFilterMask

        const uint32_t& GetCollisionGroupMask() const;
        void SetCollisionGroupMask(uint32_t collisionGroupMask);

        const uint32_t& GetAllowedCollisionFilterMask() const;
        void SetAllowedCollisionFilterMask(uint32_t allowedCollisionFilterMask);

        const bool& GetTangible() const;
        void SetTangible(bool tangible);

        // Note: Use std::swap to swap vectors of CollisionShape2D

        std::vector<CollisionPoint2D>& GetCollisionPoints();
        const std::vector<CollisionPoint2D>& GetCollisionPoints() const;
        const std::vector<CollisionPoint2D>& GetTransformedCollisionPoints();

        std::vector<CollisionLine2D>& GetCollisionLines();
        const std::vector<CollisionLine2D>& GetCollisionLines() const;
        const std::vector<CollisionLine2D>& GetTransformedCollisionLines();

        std::vector<CollisionRay2D>& GetCollisionRays();
        const std::vector<CollisionRay2D>& GetCollisionRays() const;
        const std::vector<CollisionRay2D>& GetTransformedCollisionRays();

        std::vector<CollisionLineSegment2D>& GetCollisionLineSegments();
        const std::vector<CollisionLineSegment2D>& GetCollisionLineSegments() const;
        const std::vector<CollisionLineSegment2D>& GetTransformedCollisionLineSegments();

        std::vector<CollisionRectangle2D>& GetCollisionRectangles();
        const std::vector<CollisionRectangle2D>& GetCollisionRectangles() const;
        const std::vector<CollisionRectangle2D>& GetTransformedCollisionRectangles();

        std::vector<CollisionOrientedRectangle2D>& GetCollisionOrientedRectangles();
        const std::vector<CollisionOrientedRectangle2D>& GetCollisionOrientedRectangles() const;
        const std::vector<CollisionOrientedRectangle2D>& GetTransformedCollisionOrientedRectangles();

        std::vector<CollisionCircle2D>& GetCollisionCircles();
        const std::vector<CollisionCircle2D>& GetCollisionCircles() const;
        const std::vector<CollisionCircle2D>& GetTransformedCollisionCircles();

        std::vector<CollisionCapsule2D>& GetCollisionCapsules();
        const std::vector<CollisionCapsule2D>& GetCollisionCapsules() const;
        const std::vector<CollisionCapsule2D>& GetTransformedCollisionCapsules();

        std::vector<CollisionTriangle2D>& GetCollisionTriangles();
        const std::vector<CollisionTriangle2D>& GetCollisionTriangles() const;
        const std::vector<CollisionTriangle2D>& GetTransformedCollisionTriangles();

        std::vector<CollisionPolygon2D>& GetCollisionPolygons();
        const std::vector<CollisionPolygon2D>& GetCollisionPolygons() const;
        const std::vector<CollisionPolygon2D>& GetTransformedCollisionPolygons();

        std::vector<CollisionConvexPolygon2D>& GetCollisionConvexPolygons();
        const std::vector<CollisionConvexPolygon2D>& GetCollisionConvexPolygons() const;
        const std::vector<CollisionConvexPolygon2D>& GetTransformedCollisionConvexPolygons();

        const float& GetBoundingRadius();

        const float& GetArea();

        const float& GetMomentOfInertia();

        // Only tangible_ shapes will be included in the bounding radius
        // calucation and in the transformed collision shape vectors

        void AddCollision(const CollisionData2D& collision);
        void ClearCollisions();
        const std::vector<CollisionData2D>& GetCollisions() const;

        float GetDistanceSquaredToPoint(const glm::vec2& point_position) const;

        float GetDistanceSquaredToPointAndClosestPointToPoint(
            const glm::vec2& point_position,
            glm::vec2& closestPoint_position) const;

        const PhysicsType& GetPhysicsType() const;
        void SetPhysicsType(PhysicsType physicsType);

        // Mass cannot be set to 0.0f or less. It will at minimum be set to
        // s_minimumMass_

        const float& GetMass() const;
        void SetMass(float mass);

        const float& GetRestitution() const;
        void SetRestitution(float restitution);

        const glm::vec2& GetVelocity() const;
        void SetVelocity(const glm::vec2& velocity);

        const float& GetAngularVelocity() const;
        void SetAngularVelocity(float angularVelocity);

        const glm::vec2& GetAcceleration() const;
        void SetAcceleration(const glm::vec2& acceleration);

        const float& GetAngularAcceleration() const;
        void SetAngularAcceleration(float angularAcceleration);

        // Overwriten:

        void SetPosition(const glm::vec2& position);
        void SetPosition(float x, float y);
        void SetPositionX(float x);
        void SetPositionY(float y);

        void AddTranslation(const glm::vec2& translation);
        void AddTranslation(float x, float y);
        void AddTranslationX(float x);
        void AddTranslationY(float y);

        void RevolveAround(const glm::vec2& focalPoint, float angleInRadians);

        void TranslateUp(float d);
        void TranslateRight(float d);

        void AddRotation(float angleInRadians);
        void SetRotation(float angleInRadians);

    protected:
        void CalculateBoundingRadius();
        void CalculateArea();
        void CalculateMomentOfInertia();
        void CalculateTransformedCollisionShapes();

        // Inherited:
        // glm::vec2 position_;
        // float rotation_;

        std::vector<CollisionPoint2D> collisionPoints_;
        std::vector<CollisionLine2D> collisionLines_;
        std::vector<CollisionRay2D> collisionRays_;
        std::vector<CollisionLineSegment2D> collisionLineSegments_;
        std::vector<CollisionRectangle2D> collisionRectangles_;
        std::vector<CollisionOrientedRectangle2D> collisionOrientedRectangles_;
        std::vector<CollisionCircle2D> collisionCircles_;
        std::vector<CollisionCapsule2D> collisionCapsules_;
        std::vector<CollisionTriangle2D> collisionTriangles_;
        std::vector<CollisionPolygon2D> collisionPolygons_;
        std::vector<CollisionConvexPolygon2D> collisionConvexPolygons_;

        // When being transformed a shape can become a different shape.
        // With a rotation, an axis aligned rectangle becomes an
        // oriented rectangle.
        std::vector<CollisionPoint2D> transformedCollisionPoints_;
        std::vector<CollisionLine2D> transformedCollisionLines_;
        std::vector<CollisionRay2D> transformedCollisionRays_;
        std::vector<CollisionLineSegment2D> transformedCollisionLineSegments_;
        std::vector<CollisionRectangle2D> transformedCollisionRectangles_;
        std::vector<CollisionOrientedRectangle2D> transformedCollisionOrientedRectangles_;
        std::vector<CollisionCircle2D> transformedCollisionCircles_;
        std::vector<CollisionCapsule2D> transformedCollisionCapsules_;
        std::vector<CollisionTriangle2D> transformedCollisionTriangles_;
        std::vector<CollisionPolygon2D> transformedCollisionPolygons_;
        std::vector<CollisionConvexPolygon2D> transformedCollisionConvexPolygons_;

        // The area of a shape is needed to determine its mass. Its mass is
        // needed to determine its moment of inertia. Its moment of inertia is
        // needed to determine the total body's moment of inertia.
        std::vector<float> collisionRectangleAreas_;
        std::vector<float> collisionOrientedRectangleAreas_;
        std::vector<float> collisionCircleAreas_;
        std::vector<float> collisionCapsuleAreas_;
        std::vector<float> collisionTriangleAreas_;
        std::vector<float> collisionConvexPolygonAreas_;

        uint32_t collisionGroupMask_;
        uint32_t allowedCollisionFilterMask_;

        bool tangible_;

        float boundingRadius_;
        float area_;
        float momentOfInertia_;

        bool boundingRadiusUpToDate_;
        bool areaUpToDate_;
        bool momentOfInertiaUpToDate_;

        bool transformedCollisionShapesUpToDate_;

        std::vector<CollisionData2D> collisions_;

        PhysicsType physicsType_;

        float mass_;

        float restitution_; // from 0.0f to 1.0f

        glm::vec2 velocity_;
        float angularVelocity_;

        glm::vec2 acceleration_;
        float angularAcceleration_;

        static const float s_minimumMass_;
    };

    // public ------------------------------------------------------------------

    inline CollisionBody2D::CollisionBody2D()
        : collisionGroupMask_(0b00000000000000000000000000000001)
        , allowedCollisionFilterMask_(0b11111111111111111111111111111111)
        , tangible_(true)
        , boundingRadius_(0.0f)
        , area_(0.0f)
        , momentOfInertia_(0.0f)
        , boundingRadiusUpToDate_(false)
        , areaUpToDate_(false)
        , momentOfInertiaUpToDate_(false)
        , transformedCollisionShapesUpToDate_(false)
        , physicsType_(PhysicsType::PHYSICS_TYPE_REGULAR_PHYSICS)
        , mass_(1.0f) //(std::numeric_limits<float>::infinity())
        , restitution_(1.0f)
        , velocity_(0.0f, 0.0f)
        , angularVelocity_(0.0f)
        , acceleration_(0.0f, 0.0f)
        , angularAcceleration_(0.0f)
    {}

    inline const uint32_t& CollisionBody2D::GetCollisionGroupMask() const
    {
        return collisionGroupMask_;
    }

    inline void CollisionBody2D::SetCollisionGroupMask(uint32_t collisionGroupMask)
    {
        collisionGroupMask_ = collisionGroupMask;
    }

    inline const uint32_t& CollisionBody2D::GetAllowedCollisionFilterMask() const
    {
        return allowedCollisionFilterMask_;
    }

    inline void CollisionBody2D::SetAllowedCollisionFilterMask(uint32_t allowedCollisionFilterMask)
    {
        allowedCollisionFilterMask_ = allowedCollisionFilterMask;
    }

    inline const bool& CollisionBody2D::GetTangible() const
    {
        return tangible_;
    }

    inline void CollisionBody2D::SetTangible(bool tangible)
    {
        tangible_ = tangible;
    }

    inline std::vector<CollisionPoint2D>& CollisionBody2D::GetCollisionPoints()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionPoints_;
    }

    inline const std::vector<CollisionPoint2D>& CollisionBody2D::GetCollisionPoints() const
    {
        return collisionPoints_;
    }

    inline const std::vector<CollisionPoint2D>& CollisionBody2D::GetTransformedCollisionPoints()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionPoints_;
    }

    inline std::vector<CollisionLine2D>& CollisionBody2D::GetCollisionLines()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionLines_;
    }

    inline const std::vector<CollisionLine2D>& CollisionBody2D::GetCollisionLines() const
    {
        return collisionLines_;
    }

    inline const std::vector<CollisionLine2D>& CollisionBody2D::GetTransformedCollisionLines()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionLines_;
    }

    inline std::vector<CollisionRay2D>& CollisionBody2D::GetCollisionRays()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionRays_;
    }

    inline const std::vector<CollisionRay2D>& CollisionBody2D::GetCollisionRays() const
    {
        return collisionRays_;
    }

    inline const std::vector<CollisionRay2D>& CollisionBody2D::GetTransformedCollisionRays()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionRays_;
    }

    inline std::vector<CollisionLineSegment2D>& CollisionBody2D::GetCollisionLineSegments()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionLineSegments_;
    }

    inline const std::vector<CollisionLineSegment2D>& CollisionBody2D::GetCollisionLineSegments() const
    {
        return collisionLineSegments_;
    }

    inline const std::vector<CollisionLineSegment2D>& CollisionBody2D::GetTransformedCollisionLineSegments()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionLineSegments_;
    }

    inline std::vector<CollisionRectangle2D>& CollisionBody2D::GetCollisionRectangles()
    {
        boundingRadiusUpToDate_ = false;
        areaUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionRectangles_;
    }

    inline const std::vector<CollisionRectangle2D>& CollisionBody2D::GetCollisionRectangles() const
    {
        return collisionRectangles_;
    }

    inline const std::vector<CollisionRectangle2D>& CollisionBody2D::GetTransformedCollisionRectangles()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionRectangles_;
    }

    inline std::vector<CollisionOrientedRectangle2D>& CollisionBody2D::GetCollisionOrientedRectangles()
    {
        boundingRadiusUpToDate_ = false;
        areaUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionOrientedRectangles_;
    }

    inline const std::vector<CollisionOrientedRectangle2D>& CollisionBody2D::GetCollisionOrientedRectangles() const
    {
        return collisionOrientedRectangles_;
    }

    inline const std::vector<CollisionOrientedRectangle2D>& CollisionBody2D::GetTransformedCollisionOrientedRectangles()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionOrientedRectangles_;
    }

    inline std::vector<CollisionCircle2D>& CollisionBody2D::GetCollisionCircles()
    {
        boundingRadiusUpToDate_ = false;
        areaUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionCircles_;
    }

    inline const std::vector<CollisionCircle2D>& CollisionBody2D::GetCollisionCircles() const
    {
        return collisionCircles_;
    }

    inline const std::vector<CollisionCircle2D>& CollisionBody2D::GetTransformedCollisionCircles()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionCircles_;
    }

    inline std::vector<CollisionCapsule2D>& CollisionBody2D::GetCollisionCapsules()
    {
        boundingRadiusUpToDate_ = false;
        areaUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionCapsules_;
    }

    inline const std::vector<CollisionCapsule2D>& CollisionBody2D::GetCollisionCapsules() const
    {
        return collisionCapsules_;
    }

    inline const std::vector<CollisionCapsule2D>& CollisionBody2D::GetTransformedCollisionCapsules()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionCapsules_;
    }

    inline std::vector<CollisionTriangle2D>& CollisionBody2D::GetCollisionTriangles()
    {
        boundingRadiusUpToDate_ = false;
        areaUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionTriangles_;
    }

    inline const std::vector<CollisionTriangle2D>& CollisionBody2D::GetCollisionTriangles() const
    {
        return collisionTriangles_;
    }

    inline const std::vector<CollisionTriangle2D>& CollisionBody2D::GetTransformedCollisionTriangles()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionTriangles_;
    }

    inline std::vector<CollisionPolygon2D>& CollisionBody2D::GetCollisionPolygons()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionPolygons_;
    }

    inline const std::vector<CollisionPolygon2D>& CollisionBody2D::GetCollisionPolygons() const
    {
        return collisionPolygons_;
    }

    inline const std::vector<CollisionPolygon2D>& CollisionBody2D::GetTransformedCollisionPolygons()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionPolygons_;
    }

    inline std::vector<CollisionConvexPolygon2D>& CollisionBody2D::GetCollisionConvexPolygons()
    {
        boundingRadiusUpToDate_ = false;
        areaUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;

        return collisionConvexPolygons_;
    }

    inline const std::vector<CollisionConvexPolygon2D>& CollisionBody2D::GetCollisionConvexPolygons() const
    {
        return collisionConvexPolygons_;
    }

    inline const std::vector<CollisionConvexPolygon2D>& CollisionBody2D::GetTransformedCollisionConvexPolygons()
    {
        if (!transformedCollisionShapesUpToDate_)
        {
            CalculateTransformedCollisionShapes();
        }

        return transformedCollisionConvexPolygons_;
    }

    inline const float& CollisionBody2D::GetBoundingRadius()
    {
        if (!boundingRadiusUpToDate_)
        {
            CalculateBoundingRadius();
        }

        return boundingRadius_;
    }

    inline const float& CollisionBody2D::GetArea()
    {
        if (!areaUpToDate_)
        {
            CalculateArea();
        }

        return area_;
    }

    inline const float& CollisionBody2D::GetMomentOfInertia()
    {
        if (!momentOfInertiaUpToDate_)
        {
            CalculateMomentOfInertia();
        }

        return momentOfInertia_;
    }

    inline void CollisionBody2D::AddCollision(const CollisionData2D& collision)
    {
        collisions_.push_back(collision);
    }

    inline void CollisionBody2D::ClearCollisions()
    {
        collisions_.clear();
    }

    inline const std::vector<CollisionData2D>& CollisionBody2D::GetCollisions() const
    {
        return collisions_;
    }

    inline const CollisionBody2D::PhysicsType& CollisionBody2D::GetPhysicsType() const
    {
        return physicsType_;
    }

    inline void CollisionBody2D::SetPhysicsType(PhysicsType physicsType)
    {
        physicsType_ = physicsType;
    }

    inline const float& CollisionBody2D::GetMass() const
    {
        return mass_;
    }

    inline void CollisionBody2D::SetMass(float mass)
    {
        mass_ = mass;
        if (mass_ < s_minimumMass_)
        {
            mass_ = s_minimumMass_;
        }
    }

    inline const float& CollisionBody2D::GetRestitution() const
    {
        return restitution_;
    }

    inline void CollisionBody2D::SetRestitution(float restitution)
    {
        restitution_ = restitution;

        if (restitution_ > 1.0f)
        {
            restitution_ = 1.0f;
        }
        else if (restitution_ < 0.0f)
        {
            restitution_ = 0.0f;
        }
    }

    inline const glm::vec2& CollisionBody2D::GetVelocity() const
    {
        return velocity_;
    }

    inline void CollisionBody2D::SetVelocity(const glm::vec2& velocity)
    {
        velocity_ = velocity;
    }

    inline const float& CollisionBody2D::GetAngularVelocity() const
    {
        return angularVelocity_;
    }

    inline void CollisionBody2D::SetAngularVelocity(float angularVelocity)
    {
        angularVelocity_ = angularVelocity;
    }

    inline const glm::vec2& CollisionBody2D::GetAcceleration() const
    {
        return acceleration_;
    }

    inline void CollisionBody2D::SetAcceleration(const glm::vec2& acceleration)
    {
        acceleration_ = acceleration;
    }

    inline const float& CollisionBody2D::GetAngularAcceleration() const
    {
        return angularAcceleration_;
    }

    inline void CollisionBody2D::SetAngularAcceleration(float angularAcceleration)
    {
        angularAcceleration_ = angularAcceleration;
    }

    inline void CollisionBody2D::SetPosition(const glm::vec2& position)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::SetPosition(position);
    }

    inline void CollisionBody2D::SetPosition(float x, float y)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::SetPosition(x, y);
    }

    inline void CollisionBody2D::SetPositionX(float x)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::SetPositionX(x);
    }

    inline void CollisionBody2D::SetPositionY(float y)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::SetPositionY(y);
    }

    inline void CollisionBody2D::AddTranslation(const glm::vec2& translation)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::AddTranslation(translation);
    }

    inline void CollisionBody2D::AddTranslation(float x, float y)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::AddTranslation(x, y);
    }

    inline void CollisionBody2D::AddTranslationX(float x)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::AddTranslationX(x);
    }

    inline void CollisionBody2D::AddTranslationY(float y)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::AddTranslationY(y);
    }

    inline void CollisionBody2D::RevolveAround(const glm::vec2& focalPoint, float angleInRadians)
    {
        transformedCollisionShapesUpToDate_ = false;
        Position2D::RevolveAround(focalPoint, angleInRadians);
    }

    inline void CollisionBody2D::TranslateUp(float d)
    {
        transformedCollisionShapesUpToDate_ = false;
        Placement2D::TranslateUp(d);
    }

    inline void CollisionBody2D::TranslateRight(float d)
    {
        transformedCollisionShapesUpToDate_ = false;
        Placement2D::TranslateRight(d);
    }

    inline void CollisionBody2D::AddRotation(float angleInRadians)
    {
        transformedCollisionShapesUpToDate_ = false;
        Placement2D::AddRotation(angleInRadians);
    }

    inline void CollisionBody2D::SetRotation(float angleInRadians)
    {
        transformedCollisionShapesUpToDate_ = false;
        Placement2D::SetRotation(angleInRadians);
    }
}