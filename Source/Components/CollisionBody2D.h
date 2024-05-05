#pragma once

#include "Components/Placement2D.h"
#include "Components/CollisionShape2D.h"



namespace Project001
{
    struct CollisionData2D
    {
        unsigned int myShapeTag;

        unsigned int otherEntityId;
        unsigned int otherShapeTag;

        glm::vec2 collisionPoint;
        glm::vec2 collisionNormal;
    };

    class CollisionBody2D : public Placement2D
    {
    public:
        CollisionBody2D();

        // For the collision system to count a collision, at least one group in the
        // collisionGroupMask of each body needs to be in the other body's allowedCollisionFilterMask

        const uint32_t& GetCollisionGroupMask() const;
        void SetCollisionGroupMask(uint32_t collisionGroupMask);

        const uint32_t& GetAllowedCollisionFilterMask() const;
        void SetAllowedCollisionFilterMask(uint32_t allowedCollisionFilterMask);

        const bool& GetTangible() const;
        void SetTangible(bool tangible);

        // Note: Use std::swap to swap vectors of CollisionShape2D

        std::vector<CollisionPoint2D>& GetCollisionPoints();
        const std::vector<CollisionPoint2D>& GetCollisionPoints() const;
        const std::vector<CollisionPoint2D>& GetTransformedCollisionPoints() const;

        std::vector<CollisionLine2D>& GetCollisionLines();
        const std::vector<CollisionLine2D>& GetCollisionLines() const;
        const std::vector<CollisionLine2D>& GetTransformedCollisionLines() const;

        std::vector<CollisionRay2D>& GetCollisionRays();
        const std::vector<CollisionRay2D>& GetCollisionRays() const;
        const std::vector<CollisionRay2D>& GetTransformedCollisionRays() const;

        std::vector<CollisionLineSegment2D>& GetCollisionLineSegments();
        const std::vector<CollisionLineSegment2D>& GetCollisionLineSegments() const;
        const std::vector<CollisionLineSegment2D>& GetTransformedCollisionLineSegments() const;

        std::vector<CollisionRectangle2D>& GetCollisionRectangles();
        const std::vector<CollisionRectangle2D>& GetCollisionRectangles() const;
        const std::vector<CollisionRectangle2D>& GetTransformedCollisionRectangles() const;

        std::vector<CollisionOrientedRectangle2D>& GetCollisionOrientedRectangles();
        const std::vector<CollisionOrientedRectangle2D>& GetCollisionOrientedRectangles() const;
        const std::vector<CollisionOrientedRectangle2D>& GetTransformedCollisionOrientedRectangles() const;

        std::vector<CollisionCircle2D>& GetCollisionCircles();
        const std::vector<CollisionCircle2D>& GetCollisionCircles() const;
        const std::vector<CollisionCircle2D>& GetTransformedCollisionCircles() const;

        std::vector<CollisionCapsule2D>& GetCollisionCapsules();
        const std::vector<CollisionCapsule2D>& GetCollisionCapsules() const;
        const std::vector<CollisionCapsule2D>& GetTransformedCollisionCapsules() const;

        std::vector<CollisionTriangle2D>& GetCollisionTriangles();
        const std::vector<CollisionTriangle2D>& GetCollisionTriangles() const;
        const std::vector<CollisionTriangle2D>& GetTransformedCollisionTriangles() const;

        std::vector<CollisionPolygon2D>& GetCollisionPolygons();
        const std::vector<CollisionPolygon2D>& GetCollisionPolygons() const;
        const std::vector<CollisionPolygon2D>& GetTransformedCollisionPolygons() const;

        std::vector<CollisionConvexPolygon2D>& GetCollisionConvexPolygons();
        const std::vector<CollisionConvexPolygon2D>& GetCollisionConvexPolygons() const;
        const std::vector<CollisionConvexPolygon2D>& GetTransformedCollisionConvexPolygons() const;

        const float& GetBoundingRadius() const;

        bool BoundingRadiusUpToDate() const;
        bool TransformedCollisionShapesUpToDate() const;

        // Only tangible_ shapes will be included in the bounding radius
        // calucation and in the transformed collision shape vectors

        void CalculateBoundingRadius();
        void CalculateTransformedCollisionShapes();

        void AddCollision(const CollisionData2D& collision);
        void ClearCollisions();
        const std::vector<CollisionData2D>& GetCollisions() const;

        float GetDistanceSquaredToPoint(const glm::vec2& point_position) const;

        float GetDistanceSquaredToPointAndClosestPointToPoint(
            const glm::vec2& point_position,
            glm::vec2& closestPoint_position) const;

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

        uint32_t collisionGroupMask_;
        uint32_t allowedCollisionFilterMask_;

        bool tangible_;

        float boundingRadius_;

        bool boundingRadiusUpToDate_;
        bool transformedCollisionShapesUpToDate_;

        std::vector<CollisionData2D> collisions_;
    };

    inline CollisionBody2D::CollisionBody2D()
        : collisionGroupMask_(0b00000000000000000000000000000001)
        , allowedCollisionFilterMask_(0b11111111111111111111111111111111)
        , tangible_(true)
        , boundingRadius_(0.0f)
        , boundingRadiusUpToDate_(false)
        , transformedCollisionShapesUpToDate_(false)
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

    inline const std::vector<CollisionPoint2D>& CollisionBody2D::GetTransformedCollisionPoints() const
    {
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

    inline const std::vector<CollisionLine2D>& CollisionBody2D::GetTransformedCollisionLines() const
    {
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

    inline const std::vector<CollisionRay2D>& CollisionBody2D::GetTransformedCollisionRays() const
    {
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

    inline const std::vector<CollisionLineSegment2D>& CollisionBody2D::GetTransformedCollisionLineSegments() const
    {
        return transformedCollisionLineSegments_;
    }

    inline std::vector<CollisionRectangle2D>& CollisionBody2D::GetCollisionRectangles()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;
        return collisionRectangles_;
    }

    inline const std::vector<CollisionRectangle2D>& CollisionBody2D::GetCollisionRectangles() const
    {
        return collisionRectangles_;
    }

    inline const std::vector<CollisionRectangle2D>& CollisionBody2D::GetTransformedCollisionRectangles() const
    {
        return transformedCollisionRectangles_;
    }

    inline std::vector<CollisionOrientedRectangle2D>& CollisionBody2D::GetCollisionOrientedRectangles()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;
        return collisionOrientedRectangles_;
    }

    inline const std::vector<CollisionOrientedRectangle2D>& CollisionBody2D::GetCollisionOrientedRectangles() const
    {
        return collisionOrientedRectangles_;
    }

    inline const std::vector<CollisionOrientedRectangle2D>& CollisionBody2D::GetTransformedCollisionOrientedRectangles() const
    {
        return transformedCollisionOrientedRectangles_;
    }

    inline std::vector<CollisionCircle2D>& CollisionBody2D::GetCollisionCircles()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;
        return collisionCircles_;
    }

    inline const std::vector<CollisionCircle2D>& CollisionBody2D::GetCollisionCircles() const
    {
        return collisionCircles_;
    }

    inline const std::vector<CollisionCircle2D>& CollisionBody2D::GetTransformedCollisionCircles() const
    {
        return transformedCollisionCircles_;
    }

    inline std::vector<CollisionCapsule2D>& CollisionBody2D::GetCollisionCapsules()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;
        return collisionCapsules_;
    }

    inline const std::vector<CollisionCapsule2D>& CollisionBody2D::GetCollisionCapsules() const
    {
        return collisionCapsules_;
    }

    inline const std::vector<CollisionCapsule2D>& CollisionBody2D::GetTransformedCollisionCapsules() const
    {
        return transformedCollisionCapsules_;
    }

    inline std::vector<CollisionTriangle2D>& CollisionBody2D::GetCollisionTriangles()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;
        return collisionTriangles_;
    }

    inline const std::vector<CollisionTriangle2D>& CollisionBody2D::GetCollisionTriangles() const
    {
        return collisionTriangles_;
    }

    inline const std::vector<CollisionTriangle2D>& CollisionBody2D::GetTransformedCollisionTriangles() const
    {
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

    inline const std::vector<CollisionPolygon2D>& CollisionBody2D::GetTransformedCollisionPolygons() const
    {
        return transformedCollisionPolygons_;
    }

    inline std::vector<CollisionConvexPolygon2D>& CollisionBody2D::GetCollisionConvexPolygons()
    {
        boundingRadiusUpToDate_ = false;
        transformedCollisionShapesUpToDate_ = false;
        return collisionConvexPolygons_;
    }

    inline const std::vector<CollisionConvexPolygon2D>& CollisionBody2D::GetCollisionConvexPolygons() const
    {
        return collisionConvexPolygons_;
    }

    inline const std::vector<CollisionConvexPolygon2D>& CollisionBody2D::GetTransformedCollisionConvexPolygons() const
    {
        return transformedCollisionConvexPolygons_;
    }

    inline const float& CollisionBody2D::GetBoundingRadius() const
    {
        return boundingRadius_;
    }

    inline bool CollisionBody2D::BoundingRadiusUpToDate() const
    {
        return boundingRadiusUpToDate_;
    }

    inline bool CollisionBody2D::TransformedCollisionShapesUpToDate() const
    {
        return transformedCollisionShapesUpToDate_;
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