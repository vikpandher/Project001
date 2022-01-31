#pragma once

#include "Engine/Collision2D/Capsule2D.h"
#include "Engine/Collision2D/Circle2D.h"
#include "Engine/Collision2D/OrientedRectangle2D.h"
#include "Engine/Collision2D/Point2D.h"

#include <vector>>



namespace Project001
{
    class OrientedCollisionBody2D : public Placement2D
    {
    public:
        OrientedCollisionBody2D();

        void CalculateBoundingRadius();
        float GetBoundingRadius() const;

        std::vector<Point2D>& GetCollisionPoints();
        std::vector<LineSegment2D>& GetCollisionLineSegments();
        std::vector<OrientedRectangle2D>& GetCollisionOrientedRectangles();
        std::vector<Circle2D>& GetCollisionCircles();
        std::vector<Capsule2D>& GetCollisionCapsules();

    protected:
        // Inherited:
        // glm::vec2 position_;
        // float rotation_;

        std::vector<Point2D> collisionPoints_;
        std::vector<LineSegment2D> collisionLineSegments_;
        std::vector<OrientedRectangle2D> collisionOrientedRectangles_;
        std::vector<Circle2D> collisionCircles_;
        std::vector<Capsule2D> collisionCapsules_;

        float boundingRadius_;
    };

    inline OrientedCollisionBody2D::OrientedCollisionBody2D()
        : boundingRadius_(0.0f)
    {}

    inline void OrientedCollisionBody2D::CalculateBoundingRadius()
    {
        // TODO
        float largestRadius = 0.0f;

        for (size_t i = 0; i < collisionPoints_.size(); ++i)
        {
            const Point2D& current = collisionPoints_[i];
            const glm::vec2& position = current.GetPosition();
            float radius = std::sqrt(position.x * position.x + position.y * position.y);
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const LineSegment2D& current = collisionLineSegments_[i];
            const glm::vec2& start = current.GetStart();
            const glm::vec2& end = current.GetEnd();
            float radius1 = std::sqrt(start.x * start.x + start.y * start.y);
            float radius2 = std::sqrt(end.x * end.x + end.y * end.y);
            if (radius1 > largestRadius)
            {
                largestRadius = radius1;
            }
            if (radius2 > largestRadius)
            {
                largestRadius = radius2;
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const OrientedRectangle2D& current = collisionOrientedRectangles_[i];
            const glm::vec2& halfSize = current.GetHalfSize();
            const glm::vec2& position = current.GetPosition();
            float rotation = current.GetRotation();
            glm::vec2 corner1(halfSize.x, halfSize.y);
            glm::vec2 corner2(halfSize.x, -1.0f * halfSize.y);
            glm::vec2 corner3(-1.0f * halfSize.x, -1.0f * halfSize.y);
            glm::vec2 corner4(-1.0f * halfSize.x, halfSize.y);
            Rotate2DVector(corner1, rotation);
            Rotate2DVector(corner2, rotation);
            Rotate2DVector(corner3, rotation);
            Rotate2DVector(corner4, rotation);
            corner1 += position;
            corner2 += position;
            corner3 += position;
            corner4 += position;
            float radius1 = std::sqrt(corner1.x * corner1.x + corner1.y * corner1.y);
            float radius2 = std::sqrt(corner2.x * corner2.x + corner2.y * corner2.y);
            float radius3 = std::sqrt(corner3.x * corner3.x + corner3.y * corner3.y);
            float radius4 = std::sqrt(corner4.x * corner4.x + corner4.y * corner4.y);
            if (radius1 > largestRadius)
            {
                largestRadius = radius1;
            }
            if (radius2 > largestRadius)
            {
                largestRadius = radius2;
            }
            if (radius3 > largestRadius)
            {
                largestRadius = radius3;
            }
            if (radius4 > largestRadius)
            {
                largestRadius = radius4;
            }
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const Circle2D& current = collisionCircles_[i];
            const glm::vec2& position = current.GetPosition();
            float radius = std::sqrt(position.x * position.x + position.y * position.y);
            radius += current.GetRadius();
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const Capsule2D& current = collisionCapsules_[i];
            const glm::vec2& start = current.GetStart();
            const glm::vec2& end = current.GetEnd();
            float radius1 = std::sqrt(start.x * start.x + start.y * start.y);
            radius1 += current.GetRadius();
            float radius2 = std::sqrt(end.x * end.x + end.y * end.y);
            radius2 += current.GetRadius();
            if (radius1 > largestRadius)
            {
                largestRadius = radius1;
            }
            if (radius2 > largestRadius)
            {
                largestRadius = radius2;
            }
        }
    }

    inline float OrientedCollisionBody2D::GetBoundingRadius() const
    {
        return boundingRadius_;
    }

    inline std::vector<Point2D>& OrientedCollisionBody2D::GetCollisionPoints()
    {
        return collisionPoints_;
    }

    inline std::vector<LineSegment2D>& OrientedCollisionBody2D::GetCollisionLineSegments()
    {
        return collisionLineSegments_;
    }

    inline std::vector<OrientedRectangle2D>& OrientedCollisionBody2D::GetCollisionOrientedRectangles()
    {
        return collisionOrientedRectangles_;
    }

    inline std::vector<Circle2D>& OrientedCollisionBody2D::GetCollisionCircles()
    {
        return collisionCircles_;
    }

    inline std::vector<Capsule2D>& OrientedCollisionBody2D::GetCollisionCapsules()
    {
        return collisionCapsules_;
    }
}