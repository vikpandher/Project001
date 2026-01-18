// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-17

#pragma once

#include "Components/Placement2D.h"
#include "Components/CollisionShape2D.h"



namespace Project001
{
    struct CollisionBody2DCreationInfo;

    struct CollisionOverlapData2D
    {
        CollisionOverlapData2D()
            : myShapeTag(static_cast<unsigned int>(-1))
            , otherEntityId(static_cast<unsigned int>(-1))
            , otherShapeTag(static_cast<unsigned int>(-1))
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

    struct CollisionImpulseData2D
    {
        CollisionImpulseData2D()
            : otherEntityId(static_cast<unsigned int>(-1))
            , impulse(0.0f, 0.0f)
            , angularImpulse(0.0f)
        {}
    
        unsigned int otherEntityId;
    
        glm::vec2 impulse;
        float angularImpulse;
    };

    class CollisionBody2D : public Placement2D
    {
    public:
        CollisionBody2D();

        CollisionBody2D(const CollisionBody2DCreationInfo& collisionBody2DCreationInfo);

        // For the collision system to count a collision, at least one group in the
        // collisionGroupMask of each body needs to be in the other body's 
        // allowedCollisionFilterMask

        const uint32_t& GetCollisionGroupMask() const;
        void SetCollisionGroupMask(uint32_t collisionGroupMask);

        const uint32_t& GetAllowedCollisionFilterMask() const;
        void SetAllowedCollisionFilterMask(uint32_t allowedCollisionFilterMask);

        const bool& GetEnabled() const;
        void SetEnabled(bool enabled);

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

        // Only enabled_ shapes will be included in the bounding radius
        // calucation and in the transformed collision shape vectors

        const float& GetBoundingRadius();

        const float& GetArea();

        const float& GetMomentOfInertia();

        void AddCollisionOverlap(const CollisionOverlapData2D& collisionOverlap);
        void ClearCollisionOverlaps();
        const std::vector<CollisionOverlapData2D>& GetCollisionOverlaps() const;

        void AddCollisionImpulse(const CollisionImpulseData2D& collisionImpulse);
        void ClearCollisionImpulses();
        const std::vector<CollisionImpulseData2D>& GetCollisionImpulses() const;

        float GetDistanceSquaredToPoint(const glm::vec2& point_position) const;

        float GetDistanceSquaredToPointAndClosestPointToPoint(
            const glm::vec2& point_position,
            glm::vec2& closestPoint_position) const;

        float GetLinearKeneticEnergy() const;
        float GetRotationalKeneticEnergy();

        const CollisionShape2D::PhysicsType& GetPhysicsType() const;
        void SetPhysicsType(CollisionShape2D::PhysicsType physicsType);

        const bool& GetFixedTranslation() const;
        void SetFixedTranslation(bool fixedTranslation);

        const bool& GetFixedRotation() const;
        void SetFixedRotation(bool fixedRotation);

        const bool& GetMassCalculatedFromDensity() const;
        void SetMassCalculatedFromDensity(bool massCalculatedFromDensity);

        void FlagMassToBeRecalculated();

        const float& GetDensity() const;
        void SetDensity(float density);

        const float& GetMass();
        void SetMass(float mass);

        const float& GetRestitution() const;
        void SetRestitution(float restitution);

        const float& GetFriction() const;
        void SetFriction(float friction);

        const float& GetVelocityDamping() const;
        void SetVelocityDamping(float velocityDamping);

        const float& GetAngularVelocityDamping() const;
        void SetAngularVelocityDamping(float angularVelocityDamping);

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
        void CalculateArea(); // Ignores shapes that are sensors or simple-sensors
        void CalculateMassUsingDensity();
        void CalculateMomentOfInertiaUsingDensity();
        void CalculateMomentOfInertiaUsingMass();
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

        bool enabled_;

        float boundingRadius_;
        float area_;
        float momentOfInertia_;

        bool boundingRadiusUpToDate_;
        bool areaUpToDate_;
        bool massFromDensityUpToDate_;
        bool momentOfInertiaUpToDate_;

        bool transformedCollisionShapesUpToDate_;

        std::vector<CollisionOverlapData2D> collisionOverlaps_;
        std::vector<CollisionImpulseData2D> collisionImpulses_;

        CollisionShape2D::PhysicsType physicsType_;

        bool fixedTranslation_;
        bool fixedRotation_;

        bool massCalculatedFromDensity_;
        float density_; // used when massCalculatedFromDensity_ is true;
        float mass_; // used when massCalculatedFromDensity_ is false;
        float restitution_; // from 0.0f to 1.0f; 1.0f means no loss
        float friction_; // from 0.0f to 1.0f; 0.0f means no friction
        float velocityDamping_; // exponential so 0.0f means no decay, 5.0 means 99.3f decay
        float angularVelocityDamping_; // exponential so 0.0f means no decay, 5.0 means 99.3f decay

        glm::vec2 velocity_;
        float angularVelocity_;

        glm::vec2 acceleration_;
        float angularAcceleration_;

        static const float s_minimumDensity_;
        static const float s_minimumMass_;
    };

    struct CollisionBody2DCreationInfo
    {
        uint32_t collisionGroupMask = 0b00000000000000000000000000000001;
        uint32_t allowedCollisionFilterMask = 0b11111111111111111111111111111111;
        bool enabled = true;
        CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY;
        bool fixedTranslation = false;
        bool fixedRotation = false;
        float massCalculatedFromDensity_ = true;
        float density_ = 1.0f;
        float mass = 1.0f;
        float restitution = 1.0f;
        float friction = 0.0f;
        float velocityDamping = 0.0f;
        float angularVelocityDamping = 0.0f;
        glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
        float angularVelocity = 0.0f;
        glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);
        float angularAcceleration = 0.0f;
    };

    // public ------------------------------------------------------------------

    inline CollisionBody2D::CollisionBody2D()
        : collisionGroupMask_(0b00000000000000000000000000000001)
        , allowedCollisionFilterMask_(0b11111111111111111111111111111111)
        , enabled_(true)
        , boundingRadius_(0.0f)
        , area_(0.0f)
        , momentOfInertia_(0.0f)
        , boundingRadiusUpToDate_(false)
        , areaUpToDate_(false)
        , massFromDensityUpToDate_(false)
        , momentOfInertiaUpToDate_(false)
        , transformedCollisionShapesUpToDate_(false)
        , physicsType_(CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
        , fixedTranslation_(false)
        , fixedRotation_(false)
        , massCalculatedFromDensity_(true)
        , density_(1.0f)
        , mass_(1.0f) //(std::numeric_limits<float>::infinity())
        , restitution_(1.0f)
        , friction_(0.0f)
        , velocityDamping_(0.0f)
        , angularVelocityDamping_(0.0f)
        , velocity_(0.0f, 0.0f)
        , angularVelocity_(0.0f)
        , acceleration_(0.0f, 0.0f)
        , angularAcceleration_(0.0f)
    {}

    inline CollisionBody2D::CollisionBody2D(const CollisionBody2DCreationInfo& collisionBody2DCreationInfo)
        : collisionGroupMask_(collisionBody2DCreationInfo.collisionGroupMask)
        , allowedCollisionFilterMask_(collisionBody2DCreationInfo.allowedCollisionFilterMask)
        , enabled_(collisionBody2DCreationInfo.enabled)
        , boundingRadius_(0.0f)
        , area_(0.0f)
        , momentOfInertia_(0.0f)
        , boundingRadiusUpToDate_(false)
        , areaUpToDate_(false)
        , massFromDensityUpToDate_(false)
        , momentOfInertiaUpToDate_(false)
        , transformedCollisionShapesUpToDate_(false)
        , physicsType_(collisionBody2DCreationInfo.physicsType)
        , fixedTranslation_(collisionBody2DCreationInfo.fixedTranslation)
        , fixedRotation_(collisionBody2DCreationInfo.fixedRotation)
        , massCalculatedFromDensity_(collisionBody2DCreationInfo.massCalculatedFromDensity_)
        , density_(collisionBody2DCreationInfo.density_)
        , mass_(collisionBody2DCreationInfo.mass)
        , restitution_(collisionBody2DCreationInfo.restitution)
        , friction_(collisionBody2DCreationInfo.friction)
        , velocityDamping_(collisionBody2DCreationInfo.velocityDamping)
        , angularVelocityDamping_(collisionBody2DCreationInfo.angularVelocityDamping)
        , velocity_(collisionBody2DCreationInfo.velocity)
        , angularVelocity_(collisionBody2DCreationInfo.angularVelocity)
        , acceleration_(collisionBody2DCreationInfo.acceleration)
        , angularAcceleration_(collisionBody2DCreationInfo.angularAcceleration)
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

    inline const bool& CollisionBody2D::GetEnabled() const
    {
        return enabled_;
    }

    inline void CollisionBody2D::SetEnabled(bool enabled)
    {
        enabled_ = enabled;
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
            if (massCalculatedFromDensity_)
            {
                CalculateMomentOfInertiaUsingDensity();
            }
            else
            {
                CalculateMomentOfInertiaUsingMass();
            }
        }

        return momentOfInertia_;
    }

    inline void CollisionBody2D::AddCollisionOverlap(const CollisionOverlapData2D& collisionOverlap)
    {
        collisionOverlaps_.push_back(collisionOverlap);
    }

    inline void CollisionBody2D::ClearCollisionOverlaps()
    {
        collisionOverlaps_.clear();
    }

    inline const std::vector<CollisionOverlapData2D>& CollisionBody2D::GetCollisionOverlaps() const
    {
        return collisionOverlaps_;
    }

    inline void CollisionBody2D::AddCollisionImpulse(const CollisionImpulseData2D& collisionImpulse)
    {
        collisionImpulses_.push_back(collisionImpulse);
    }

    inline void CollisionBody2D::ClearCollisionImpulses()
    {
        collisionImpulses_.clear();
    }

    inline const std::vector<CollisionImpulseData2D>& CollisionBody2D::GetCollisionImpulses() const
    {
        return collisionImpulses_;
    }

    inline float CollisionBody2D::GetLinearKeneticEnergy() const
    {
        return 0.5f * mass_ * glm::dot(velocity_, velocity_);  // v^2 is dot(velocity, velocity)
    }

    inline float CollisionBody2D::GetRotationalKeneticEnergy()
    {
        return 0.5f * GetMomentOfInertia() * angularVelocity_ * angularVelocity_;
    }

    inline const CollisionShape2D::PhysicsType& CollisionBody2D::GetPhysicsType() const
    {
        return physicsType_;
    }

    inline void CollisionBody2D::SetPhysicsType(CollisionShape2D::PhysicsType physicsType)
    {
        physicsType_ = physicsType;
    }

    inline const bool& CollisionBody2D::GetFixedTranslation() const
    {
        return fixedTranslation_;
    }

    inline void CollisionBody2D::SetFixedTranslation(bool fixedTranslation)
    {
        fixedTranslation_ = fixedTranslation;
    }

    inline const bool& CollisionBody2D::GetFixedRotation() const
    {
        return fixedRotation_;
    }

    inline void CollisionBody2D::SetFixedRotation(bool fixedRotation)
    {
        fixedRotation_ = fixedRotation;
    }

    inline const bool& CollisionBody2D::GetMassCalculatedFromDensity() const
    {
        return massCalculatedFromDensity_;
    }

    inline void CollisionBody2D::SetMassCalculatedFromDensity(bool massCalculatedFromDensity)
    {
        massCalculatedFromDensity = massCalculatedFromDensity_;
        massFromDensityUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
    }

    inline void CollisionBody2D::FlagMassToBeRecalculated()
    {
        massFromDensityUpToDate_ = false;
        momentOfInertiaUpToDate_ = false;
    }

    inline const float& CollisionBody2D::GetDensity() const
    {
        return density_;
    }

    inline void CollisionBody2D::SetDensity(float density)
    {
        density_ = density;
        if (density_ < s_minimumDensity_)
        {
            density_ = s_minimumDensity_;
        }

        if (massCalculatedFromDensity_)
        {
            massFromDensityUpToDate_ = false;
            momentOfInertiaUpToDate_ = false;
        }
    }

    inline const float& CollisionBody2D::GetMass()
    {
        if (massCalculatedFromDensity_ && !massFromDensityUpToDate_)
        {
            CalculateMassUsingDensity();
        }
        return mass_;
    }

    inline void CollisionBody2D::SetMass(float mass)
    {
        if (!massCalculatedFromDensity_)
        {
            mass_ = mass;
            if (mass_ < s_minimumMass_)
            {
                mass_ = s_minimumMass_;
            }

            momentOfInertiaUpToDate_ = false;
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

    inline const float& CollisionBody2D::GetFriction() const
    {
        return friction_;
    }

    inline void CollisionBody2D::SetFriction(float friction)
    {
        friction_ = friction;
        if (friction_ < 0.0f)
        {
            friction_ = 0.0f;
        }
    }

    inline const float& CollisionBody2D::GetVelocityDamping() const
    {
        return velocityDamping_;
    }

    inline void CollisionBody2D::SetVelocityDamping(float velocityDamping)
    {
        velocityDamping_ = velocityDamping;
        if (velocityDamping_ < 0.0f)
        {
            velocityDamping_ = 0.0f;
        }
    }

    inline const float& CollisionBody2D::GetAngularVelocityDamping() const
    {
        return angularVelocityDamping_;
    }

    inline void CollisionBody2D::SetAngularVelocityDamping(float angularVelocityDamping)
    {
        angularVelocityDamping_ = angularVelocityDamping;
        if (angularVelocityDamping_ < 0.0f)
        {
            angularVelocityDamping_ = 0.0f;
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