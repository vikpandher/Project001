// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "CollisionBody2D.h"

#include "Utilities/Overlap2D.h"

#include "glm/gtc/constants.hpp"

#include <limits>



namespace Project001
{
    // public ------------------------------------------------------------------

    float CollisionBody2D::GetDistanceSquaredToPoint(const glm::vec2& point_position) const
    {
        float minDistanceSquared = std::numeric_limits<float>::infinity();

        glm::vec2 translatedPointPosition = point_position - position_;
        glm::vec2 rotatedPointPosition = Math::Rotate2DVector(translatedPointPosition, -1.0f * rotation_);

        for (size_t i = 0; i < collisionPoints_.size(); ++i)
        {
            const CollisionPoint2D& collisionPoint = collisionPoints_[i];

            float distanceSquared = Get2D_Point_Point_DistanceSquared(
                rotatedPointPosition,
                collisionPoint.position);

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& collisionLine = collisionLines_[i];

            float distanceSquared = Get2D_Point_Line_DistanceSquared(
                rotatedPointPosition,
                collisionLine.position,
                collisionLine.slope);

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& collisionRay = collisionRays_[i];

            float distanceSquared = Get2D_Point_Ray_DistanceSquared(
                rotatedPointPosition,
                collisionRay.position,
                collisionRay.direction);

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& collisionLineSegment = collisionLineSegments_[i];

            float distanceSquared = Get2D_Point_LineSegment_DistanceSquared(
                rotatedPointPosition,
                collisionLineSegment.start,
                collisionLineSegment.end);

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& collisionRectangle = collisionRectangles_[i];

            float distanceSquared = Get2D_Point_Rectangle_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionRectangle.bottomLeft,
                collisionRectangle.topRight,
                glm::vec2());

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& collisionOrientedRectangle = collisionOrientedRectangles_[i];

            float distanceSquared = Get2D_Point_OrientedRectangle_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionOrientedRectangle.halfSize,
                collisionOrientedRectangle.position,
                collisionOrientedRectangle.rotation,
                glm::vec2());

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[i];

            float distance = Get2D_Point_Circle_Distance(
                rotatedPointPosition,
                collisionCircle.position,
                collisionCircle.radius);

            float distanceSquared = distance * distance;
            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& collisionCapsule = collisionCapsules_[i];

            float distance = Get2D_Point_Capsule_Distance(
                rotatedPointPosition,
                collisionCapsule.start,
                collisionCapsule.end,
                collisionCapsule.radius);

            float distanceSquared = distance * distance;
            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& collisionTriangle = collisionTriangles_[i];

            float distanceSquared = Get2D_Point_Triangle_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionTriangle.corner1,
                collisionTriangle.corner2,
                collisionTriangle.corner3,
                glm::vec2());

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionPolygons_.size(); ++i)
        {
            const CollisionPolygon2D& collisionPolygon = collisionPolygons_[i];

            float distanceSquared = Get2D_Point_Polygon_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionPolygon.corners.data(),
                collisionPolygon.corners.size(),
                glm::vec2());

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& collisionConvexPolygon = collisionConvexPolygons_[i];

            float distanceSquared = Get2D_Point_Polygon_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionConvexPolygon.corners.data(),
                collisionConvexPolygon.corners.size(),
                glm::vec2());

            if (distanceSquared < minDistanceSquared)
            {
                minDistanceSquared = distanceSquared;
            }
        }

        return minDistanceSquared;
    }

    float CollisionBody2D::GetDistanceSquaredToPointAndClosestPointToPoint(
        const glm::vec2& point_position,
        glm::vec2& closestPoint_position) const
    {
        CollisionShape2D::ShapeType collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_NONE;
        size_t collisionShapeIndex = -1;
        float minDistanceSquared = std::numeric_limits<float>::infinity();

        glm::vec2 translatedPointPosition = point_position - position_;
        glm::vec2 rotatedPointPosition = Math::Rotate2DVector(translatedPointPosition, -1.0f * rotation_);

        // For some shapes the closest point is calculated while the distance squared is calculated
        // These are: Rectangles, OrientedReactangles, Triangles, Polygons, ConvexPolygons

        for (size_t i = 0; i < collisionPoints_.size(); ++i)
        {
            const CollisionPoint2D& collisionPoint = collisionPoints_[i];

            float distanceSquared = Get2D_Point_Point_DistanceSquared(
                rotatedPointPosition,
                collisionPoint.position);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_POINT;
                collisionShapeIndex = i;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& collisionLine = collisionLines_[i];

            float distanceSquared = Get2D_Point_Line_DistanceSquared(
                rotatedPointPosition,
                collisionLine.position,
                collisionLine.slope);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_LINE;
                collisionShapeIndex = i;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& collisionRay = collisionRays_[i];

            float distanceSquared = Get2D_Point_Ray_DistanceSquared(
                rotatedPointPosition,
                collisionRay.position,
                collisionRay.direction);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_RAY;
                collisionShapeIndex = i;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& collisionLineSegment = collisionLineSegments_[i];

            float distanceSquared = Get2D_Point_LineSegment_DistanceSquared(
                rotatedPointPosition,
                collisionLineSegment.start,
                collisionLineSegment.end);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_LINE_SEGMENT;
                collisionShapeIndex = i;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& collisionRectangle = collisionRectangles_[i];

            glm::vec2 currentClosestPointPosition;
            float distanceSquared = Get2D_Point_Rectangle_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionRectangle.bottomLeft,
                collisionRectangle.topRight,
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_RECTANGLE;
                collisionShapeIndex = i;
                closestPoint_position = currentClosestPointPosition;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& collisionOrientedRectangle = collisionOrientedRectangles_[i];

            glm::vec2 currentClosestPointPosition;
            float distanceSquared = Get2D_Point_OrientedRectangle_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionOrientedRectangle.halfSize,
                collisionOrientedRectangle.position,
                collisionOrientedRectangle.rotation,
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_ORIENTED_RECTANGLE;
                collisionShapeIndex = i;
                closestPoint_position = currentClosestPointPosition;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[i];

            float distance = Get2D_Point_Circle_Distance(
                rotatedPointPosition,
                collisionCircle.position,
                collisionCircle.radius);

            float distanceSquared = distance * distance;
            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_CIRCLE;
                collisionShapeIndex = i;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& collisionCapsule = collisionCapsules_[i];

            float distance = Get2D_Point_Capsule_Distance(
                rotatedPointPosition,
                collisionCapsule.start,
                collisionCapsule.end,
                collisionCapsule.radius);

            float distanceSquared = distance * distance;
            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_CAPSULE;
                collisionShapeIndex = i;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& collisionTriangle = collisionTriangles_[i];

            glm::vec2 currentClosestPointPosition;
            float distanceSquared = Get2D_Point_Triangle_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionTriangle.corner1,
                collisionTriangle.corner2,
                collisionTriangle.corner3,
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_TRIANGLE;
                collisionShapeIndex = i;
                closestPoint_position = currentClosestPointPosition;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionPolygons_.size(); ++i)
        {
            const CollisionPolygon2D& collisionPolygon = collisionPolygons_[i];

            glm::vec2 currentClosestPointPosition;
            float distanceSquared = Get2D_Point_Polygon_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionPolygon.corners.data(),
                collisionPolygon.corners.size(),
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_POLYGON;
                collisionShapeIndex = i;
                closestPoint_position = currentClosestPointPosition;
                minDistanceSquared = distanceSquared;
            }
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& collisionConvexPolygon = collisionConvexPolygons_[i];

            glm::vec2 currentClosestPointPosition;
            float distanceSquared = Get2D_Point_Polygon_ClosestPointAndDistanceSquared(
                rotatedPointPosition,
                collisionConvexPolygon.corners.data(),
                collisionConvexPolygon.corners.size(),
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShapeType = CollisionShape2D::ShapeType::SHAPE_TYPE_CONVEX_POLYGON;
                collisionShapeIndex = i;
                closestPoint_position = currentClosestPointPosition;
                minDistanceSquared = distanceSquared;
            }
        }

        switch (collisionShapeType)
        {
        case CollisionShape2D::ShapeType::SHAPE_TYPE_POINT:
        {
            const CollisionPoint2D& collisionPoint = collisionPoints_[collisionShapeIndex];
            closestPoint_position = collisionPoint.position;
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_LINE:
        {
            const CollisionLine2D& collisionLine = collisionLines_[collisionShapeIndex];
            Get2D_Point_Line_ClosestPoint(
                rotatedPointPosition,
                collisionLine.position,
                collisionLine.slope,
                closestPoint_position);
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_RAY:
        {
            const CollisionRay2D& collisionRay = collisionRays_[collisionShapeIndex];
            Get2D_Point_Ray_ClosestPoint(
                rotatedPointPosition,
                collisionRay.position,
                collisionRay.direction,
                closestPoint_position);
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_LINE_SEGMENT:
        {
            const CollisionLineSegment2D& collisionLineSegment = collisionLineSegments_[collisionShapeIndex];
            Get2D_Point_LineSegment_ClosestPoint(
                rotatedPointPosition,
                collisionLineSegment.start,
                collisionLineSegment.end,
                closestPoint_position);
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_RECTANGLE:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_ORIENTED_RECTANGLE:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_CIRCLE:
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[collisionShapeIndex];
            Get2D_Point_Circle_ClosestPoint(
                rotatedPointPosition,
                collisionCircle.position,
                collisionCircle.radius,
                closestPoint_position);
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_CAPSULE:
        {
            closestPoint_position = rotatedPointPosition;
            const CollisionCapsule2D& collisionCapsule = collisionCapsules_[collisionShapeIndex];
            Get2D_Point_Capsule_ClosestPoint(
                rotatedPointPosition,
                collisionCapsule.start,
                collisionCapsule.end,
                collisionCapsule.radius,
                closestPoint_position);
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_TRIANGLE:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_POLYGON:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D::ShapeType::SHAPE_TYPE_CONVEX_POLYGON:
        {
            // Already calculated
            break;
        }
        default:
        {
            closestPoint_position = rotatedPointPosition;
            break;
        }
        }

        // Move point to world space

        closestPoint_position = Math::Rotate2DVector(closestPoint_position, rotation_);
        closestPoint_position += position_;

        return minDistanceSquared;
    }

    const float CollisionBody2D::s_minimumDensity_ = 0.00001f;
    const float CollisionBody2D::s_minimumMass_ = 0.00001f;

    // protected ---------------------------------------------------------------

    void CollisionBody2D::CalculateBoundingRadius()
    {
        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& currentLine = collisionLines_[i];
            if (currentLine.enabled)
            {
                boundingRadius_ = std::numeric_limits<float>::infinity();
                return;
            }
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& current = collisionRays_[i];
            if (current.enabled)
            {
                boundingRadius_ = std::numeric_limits<float>::infinity();
                return;
            }
        }

        float largestRadius = 0.0f;

        if (!collisionPoints_.empty())
        {
            largestRadius = std::numeric_limits<float>::lowest();
        }

        for (size_t i = 0; i < collisionPoints_.size(); ++i)
        {
            const CollisionPoint2D& currentPoint = collisionPoints_[i];
            if (!currentPoint.enabled)
            {
                continue;
            }

            const glm::vec2& position = currentPoint.position;
            float radius = glm::sqrt(position.x * position.x + position.y * position.y);
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& currentLineSegment = collisionLineSegments_[i];
            if (!currentLineSegment.enabled)
            {
                continue;
            }

            const glm::vec2& start = currentLineSegment.start;
            const glm::vec2& end = currentLineSegment.end;
            float radius1 = glm::sqrt(start.x * start.x + start.y * start.y);
            float radius2 = glm::sqrt(end.x * end.x + end.y * end.y);
            if (radius1 > largestRadius)
            {
                largestRadius = radius1;
            }
            if (radius2 > largestRadius)
            {
                largestRadius = radius2;
            }
        }

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& currentRectangle = collisionRectangles_[i];
            if (!currentRectangle.enabled)
            {
                continue;
            }

            const glm::vec2& bottomLeft = currentRectangle.bottomLeft;
            const glm::vec2& topRight = currentRectangle.topRight;
            float radiusBottomLeft = glm::sqrt(bottomLeft.x * bottomLeft.x + bottomLeft.y * bottomLeft.y);
            float radiusBottomRight = glm::sqrt(topRight.x * topRight.x + bottomLeft.y * bottomLeft.y);
            float radiusTopRight = glm::sqrt(topRight.x * topRight.x + topRight.y * topRight.y);
            float radiusTopLeft = glm::sqrt(bottomLeft.x * bottomLeft.x + topRight.y * topRight.y);
            if (radiusBottomLeft > largestRadius)
            {
                largestRadius = radiusBottomLeft;
            }
            if (radiusBottomRight > largestRadius)
            {
                largestRadius = radiusBottomRight;
            }
            if (radiusTopRight > largestRadius)
            {
                largestRadius = radiusTopRight;
            }
            if (radiusTopLeft > largestRadius)
            {
                largestRadius = radiusTopLeft;
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& currentOrientedRectangle = collisionOrientedRectangles_[i];
            if (!currentOrientedRectangle.enabled)
            {
                continue;
            }

            const glm::vec2& halfSize = currentOrientedRectangle.halfSize;
            const glm::vec2& position = currentOrientedRectangle.position;
            float rotation = currentOrientedRectangle.rotation;
            glm::vec2 corner1(halfSize.x, halfSize.y);
            glm::vec2 corner2(halfSize.x, -1.0f * halfSize.y);
            glm::vec2 corner3(-1.0f * halfSize.x, -1.0f * halfSize.y);
            glm::vec2 corner4(-1.0f * halfSize.x, halfSize.y);
            Math::Rotate2DVector(corner1, rotation);
            Math::Rotate2DVector(corner2, rotation);
            Math::Rotate2DVector(corner3, rotation);
            Math::Rotate2DVector(corner4, rotation);
            corner1 += position;
            corner2 += position;
            corner3 += position;
            corner4 += position;
            float radius1 = glm::sqrt(corner1.x * corner1.x + corner1.y * corner1.y);
            float radius2 = glm::sqrt(corner2.x * corner2.x + corner2.y * corner2.y);
            float radius3 = glm::sqrt(corner3.x * corner3.x + corner3.y * corner3.y);
            float radius4 = glm::sqrt(corner4.x * corner4.x + corner4.y * corner4.y);
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
            const CollisionCircle2D& currentCircle = collisionCircles_[i];
            if (!currentCircle.enabled)
            {
                continue;
            }

            const glm::vec2& position = currentCircle.position;
            float radius = glm::sqrt(position.x * position.x + position.y * position.y);
            radius += currentCircle.radius;
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& currentCapsule = collisionCapsules_[i];
            if (!currentCapsule.enabled)
            {
                continue;
            }

            const glm::vec2& start = currentCapsule.start;
            const glm::vec2& end = currentCapsule.end;
            float radius1 = glm::sqrt(start.x * start.x + start.y * start.y);
            radius1 += currentCapsule.radius;
            float radius2 = glm::sqrt(end.x * end.x + end.y * end.y);
            radius2 += currentCapsule.radius;
            if (radius1 > largestRadius)
            {
                largestRadius = radius1;
            }
            if (radius2 > largestRadius)
            {
                largestRadius = radius2;
            }
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& currentTriangle = collisionTriangles_[i];
            if (!currentTriangle.enabled)
            {
                continue;
            }

            const glm::vec2& corner1 = currentTriangle.corner1;
            const glm::vec2& corner2 = currentTriangle.corner2;
            const glm::vec2& corner3 = currentTriangle.corner3;
            float radius1 = glm::sqrt(corner1.x * corner1.x + corner1.y * corner1.y);
            float radius2 = glm::sqrt(corner2.x * corner2.x + corner2.y * corner2.y);
            float radius3 = glm::sqrt(corner3.x * corner3.x + corner3.y * corner3.y);
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
        }

        for (size_t i = 0; i < collisionPolygons_.size(); ++i)
        {
            const CollisionPolygon2D& currentPolygon = collisionPolygons_[i];
            if (!currentPolygon.enabled)
            {
                continue;
            }

            for (size_t j = 0; j < currentPolygon.corners.size(); ++j)
            {
                const glm::vec2& corner = currentPolygon.corners[j];
                float radius = glm::sqrt(corner.x * corner.x + corner.y * corner.y);
                if (radius > largestRadius)
                {
                    largestRadius = radius;
                }
            }
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& currentConvexPolygon = collisionConvexPolygons_[i];
            if (!currentConvexPolygon.enabled)
            {
                continue;
            }

            for (size_t j = 0; j < currentConvexPolygon.corners.size(); ++j)
            {
                const glm::vec2& corner = currentConvexPolygon.corners[j];
                float radius = glm::sqrt(corner.x * corner.x + corner.y * corner.y);
                if (radius > largestRadius)
                {
                    largestRadius = radius;
                }
            }
        }

        boundingRadius_ = largestRadius;
        boundingRadiusUpToDate_ = true;
    }

    void CollisionBody2D::CalculateArea()
    {
        collisionRectangleAreas_.clear();
        collisionOrientedRectangleAreas_.clear();
        collisionCircleAreas_.clear();
        collisionCapsuleAreas_.clear();
        collisionTriangleAreas_.clear();
        collisionConvexPolygonAreas_.clear();

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& collisionRectangle = collisionRectangles_[i];

            float area = 0.0f;
            if (collisionRectangle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR &&
                collisionRectangle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
            {
                area = Get2D_Rectangle_Area(collisionRectangle.bottomLeft, collisionRectangle.topRight);
            }

            collisionRectangleAreas_.push_back(area);

            area_ += area;
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& collisionOrientedRectangle = collisionOrientedRectangles_[i];

            float area = 0.0f;
            if (collisionOrientedRectangle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR &&
                collisionOrientedRectangle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
            {
                area = Get2D_Rectangle_Area_2(collisionOrientedRectangle.halfSize);
            }

            collisionOrientedRectangleAreas_.push_back(area);

            area_ += area;
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[i];

            float area = 0.0f;
            if (collisionCircle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR &&
                collisionCircle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
            {
                area = Get2D_Circle_Area(collisionCircle.radius);
            }

            collisionCircleAreas_.push_back(area);

            area_ += area;
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& collisionCapsule = collisionCapsules_[i];

            float area = 0.0f;
            if (collisionCapsule.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR &&
                collisionCapsule.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
            {
                area = Get2D_Capsule_Area(collisionCapsule.start, collisionCapsule.end, collisionCapsule.radius);
            }

            collisionCapsuleAreas_.push_back(area);

            area_ += area;
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& collisionTriangle = collisionTriangles_[i];

            float area = 0.0f;
            if (collisionTriangle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR &&
                collisionTriangle.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
            {
                area = Get2D_Triangle_Area(collisionTriangle.corner1, collisionTriangle.corner2, collisionTriangle.corner3);
            }

            collisionTriangleAreas_.push_back(area);

            area_ += area;
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& collisionConvexPolygon = collisionConvexPolygons_[i];

            float area = 0.0f;
            if (collisionConvexPolygon.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR &&
                collisionConvexPolygon.physicsType != CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
            {
                area = Get2D_ConvexPolygon_Area(collisionConvexPolygon.corners.data(), collisionConvexPolygon.corners.size());
            }

            collisionConvexPolygonAreas_.push_back(area);

            area_ += area;
        }

        areaUpToDate_ = true;
    }

    void CollisionBody2D::CalculateMassUsingDensity()
    {
        if (!areaUpToDate_)
        {
            CalculateArea();
        }

        mass_ = density_ * area_;

        massFromDensityUpToDate_ = true;
    }

    void CollisionBody2D::CalculateMomentOfInertiaUsingDensity()
    {
        if (!areaUpToDate_)
        {
            CalculateArea();
        }

        if (std::isinf(density_))
        {
            momentOfInertia_ = std::numeric_limits<float>::infinity();
            momentOfInertiaUpToDate_ = true;
            return;
        }

        momentOfInertia_ = 0.0f;

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& collisionRectangle = collisionRectangles_[i];
            const float& area = collisionRectangleAreas_[i];
            float mass = density_ * area;

            momentOfInertia_ += Get2D_Rectangle_MomentOfInertia(mass, collisionRectangle.bottomLeft, collisionRectangle.topRight);
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& collisionOrientedRectangle = collisionOrientedRectangles_[i];
            const float& area = collisionOrientedRectangleAreas_[i];
            float mass = density_ * area;

            momentOfInertia_ += Get2D_Rectangle_MomentOfInertia_2(mass, collisionOrientedRectangle.position, collisionOrientedRectangle.halfSize);
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[i];
            const float& area = collisionCircleAreas_[i];
            float mass = density_ * area;

            momentOfInertia_ += Get2D_Circle_MomentOfInertia(mass, collisionCircle.position, collisionCircle.radius);
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& collisionCapsule = collisionCapsules_[i];
            const float& area = collisionCapsuleAreas_[i];
            float mass = density_ * area;

            momentOfInertia_ += Get2D_Capsule_MomentOfInertia(mass, collisionCapsule.start, collisionCapsule.end, collisionCapsule.radius);
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& collisionTriangle = collisionTriangles_[i];
            const float& area = collisionTriangleAreas_[i];
            float mass = density_ * area;

            momentOfInertia_ += Get2D_Triangle_MomentOfInertia(mass, collisionTriangle.corner1, collisionTriangle.corner2, collisionTriangle.corner3);
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& collisionConvexPolygon = collisionConvexPolygons_[i];
            const float& area = collisionConvexPolygonAreas_[i];
            float mass = density_ * area;

            momentOfInertia_ += Get2D_ConvexPolygon_MomentOfInertia(area, mass, collisionConvexPolygon.corners.data(), collisionConvexPolygon.corners.size());
        }

        momentOfInertiaUpToDate_ = true;
    }

    void CollisionBody2D::CalculateMomentOfInertiaUsingMass()
    {
        if (!areaUpToDate_)
        {
            CalculateArea();
        }

        if (std::isinf(mass_))
        {
            momentOfInertia_ = std::numeric_limits<float>::infinity();
            momentOfInertiaUpToDate_ = true;
            return;
        }

        momentOfInertia_ = 0.0f;

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& collisionRectangle = collisionRectangles_[i];
            const float& area = collisionRectangleAreas_[i];
            float mass = mass_ * area / area_;

            momentOfInertia_ += Get2D_Rectangle_MomentOfInertia(mass, collisionRectangle.bottomLeft, collisionRectangle.topRight);
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& collisionOrientedRectangle = collisionOrientedRectangles_[i];
            const float& area = collisionOrientedRectangleAreas_[i];
            float mass = mass_ * area / area_;

            momentOfInertia_ += Get2D_Rectangle_MomentOfInertia_2(mass, collisionOrientedRectangle.position, collisionOrientedRectangle.halfSize);
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[i];
            const float& area = collisionCircleAreas_[i];
            float mass = mass_ * area / area_;

            momentOfInertia_ += Get2D_Circle_MomentOfInertia(mass, collisionCircle.position, collisionCircle.radius);
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& collisionCapsule = collisionCapsules_[i];
            const float& area = collisionCapsuleAreas_[i];
            float mass = mass_ * area / area_;

            momentOfInertia_ += Get2D_Capsule_MomentOfInertia(mass, collisionCapsule.start, collisionCapsule.end, collisionCapsule.radius);
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& collisionTriangle = collisionTriangles_[i];
            const float& area = collisionTriangleAreas_[i];
            float mass = mass_ * area / area_;

            momentOfInertia_ += Get2D_Triangle_MomentOfInertia(mass, collisionTriangle.corner1, collisionTriangle.corner2, collisionTriangle.corner3);
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& collisionConvexPolygon = collisionConvexPolygons_[i];
            const float& area = collisionConvexPolygonAreas_[i];
            float mass = mass_ * area / area_;

            momentOfInertia_ += Get2D_ConvexPolygon_MomentOfInertia(area, mass, collisionConvexPolygon.corners.data(), collisionConvexPolygon.corners.size());
        }

        momentOfInertiaUpToDate_ = true;
    }

    void CollisionBody2D::CalculateTransformedCollisionShapes()
    {
        transformedCollisionPoints_.clear();
        transformedCollisionLines_.clear();
        transformedCollisionRays_.clear();
        transformedCollisionLineSegments_.clear();
        transformedCollisionRectangles_.clear();
        transformedCollisionOrientedRectangles_.clear();
        transformedCollisionCircles_.clear();
        transformedCollisionCapsules_.clear();
        transformedCollisionTriangles_.clear();
        transformedCollisionPolygons_.clear();
        transformedCollisionConvexPolygons_.clear();

        for (size_t i = 0; i < collisionPoints_.size(); ++i)
        {
            const CollisionPoint2D& currentCollisionPoint = collisionPoints_[i];
            if (!currentCollisionPoint.enabled)
            {
                continue;
            }

            glm::vec2 newPosition = Math::Rotate2DVector(currentCollisionPoint.position, rotation_) + position_;
            transformedCollisionPoints_.emplace_back(
                newPosition,
                currentCollisionPoint.tag,
                currentCollisionPoint.enabled
            );
        }

        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& currentCollisionLine = collisionLines_[i];
            if (!currentCollisionLine.enabled)
            {
                continue;
            }

            glm::vec2 newPosition = Math::Rotate2DVector(currentCollisionLine.position, rotation_) + position_;
            float newSlope = RotateSlope(currentCollisionLine.slope, rotation_);
            transformedCollisionLines_.emplace_back(
                newPosition,
                newSlope,
                currentCollisionLine.tag,
                currentCollisionLine.enabled
            );
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& currentCollisionRay = collisionRays_[i];
            if (!currentCollisionRay.enabled)
            {
                continue;
            }

            glm::vec2 newPosition = currentCollisionRay.position + position_;
            glm::vec2 newDirectoin = Math::Rotate2DVector(currentCollisionRay.direction, rotation_);
            transformedCollisionRays_.emplace_back(
                newPosition,
                newDirectoin,
                currentCollisionRay.tag,
                currentCollisionRay.enabled
            );
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& currentCollisionLineSegment = collisionLineSegments_[i];
            if (!currentCollisionLineSegment.enabled)
            {
                continue;
            }

            glm::vec2 newStart = Math::Rotate2DVector(currentCollisionLineSegment.start, rotation_) + position_;
            glm::vec2 newEnd = Math::Rotate2DVector(currentCollisionLineSegment.end, rotation_) + position_;
            transformedCollisionLineSegments_.emplace_back(
                newStart,
                newEnd,
                currentCollisionLineSegment.tag,
                currentCollisionLineSegment.enabled
            );
        }

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& currentCollisionRectangle = collisionRectangles_[i];
            if (!currentCollisionRectangle.enabled)
            {
                continue;
            }

            CollisionShape2D::PhysicsType currentPhysicsType = currentCollisionRectangle.physicsType;
            if (currentPhysicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE)
            {
                currentPhysicsType = physicsType_;
            }

            if (Math::FloatEqualToFloat(rotation_, 0.0f))
            {
                glm::vec2 newBottomLeft = currentCollisionRectangle.bottomLeft + position_;
                glm::vec2 newTopRight = currentCollisionRectangle.topRight + position_;
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.enabled,
                    currentPhysicsType
                );
            }
            else if (Math::FloatEqualToFloat(rotation_, glm::half_pi<float>()))
            {
                glm::vec2 newBottomLeft = Math::Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::half_pi<float>()) + position_;
                glm::vec2 newTopRight = Math::Rotate2DVector(currentCollisionRectangle.topRight, glm::half_pi<float>()) + position_;
                std::swap(newBottomLeft.x, newTopRight.x);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.enabled,
                    currentPhysicsType
                );
            }
            else if (Math::FloatEqualToFloat(rotation_, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = Math::Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::pi<float>()) + position_;
                glm::vec2 newTopRight = Math::Rotate2DVector(currentCollisionRectangle.topRight, glm::pi<float>()) + position_;
                transformedCollisionRectangles_.emplace_back(
                    newTopRight,
                    newBottomLeft,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.enabled,
                    currentPhysicsType
                );
            }
            else if (Math::FloatEqualToFloat(rotation_, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft = Math::Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::three_over_two_pi<float>()) + position_;
                glm::vec2 newTopRight = Math::Rotate2DVector(currentCollisionRectangle.topRight, glm::three_over_two_pi<float>()) + position_;
                std::swap(newBottomLeft.y, newTopRight.y);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.enabled,
                    currentPhysicsType
                );
            }
            else
            {
                glm::vec2 newHalfSize = (currentCollisionRectangle.topRight - currentCollisionRectangle.bottomLeft) / 2.0f;
                glm::vec2 newPosition = (currentCollisionRectangle.bottomLeft + currentCollisionRectangle.topRight) / 2.0f;
                newPosition = Math::Rotate2DVector(newPosition, rotation_) + position_;
                transformedCollisionOrientedRectangles_.emplace_back(
                    newHalfSize,
                    newPosition,
                    rotation_,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.enabled,
                    currentPhysicsType
                );
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& currentCollisionOrientedRectangle = collisionOrientedRectangles_[i];
            if (!currentCollisionOrientedRectangle.enabled)
            {
                continue;
            }

            CollisionShape2D::PhysicsType currentPhysicsType = currentCollisionOrientedRectangle.physicsType;
            if (currentPhysicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE)
            {
                currentPhysicsType = physicsType_;
            }

            glm::vec2 newPosition = Math::Rotate2DVector(currentCollisionOrientedRectangle.position, rotation_) + position_;
            float newRotation = currentCollisionOrientedRectangle.rotation + rotation_;
            if (Math::FloatEqualToFloat(newRotation, 0.0f) || Math::FloatEqualToFloat(newRotation, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = newPosition - currentCollisionOrientedRectangle.halfSize;
                glm::vec2 newTopRight = newPosition + currentCollisionOrientedRectangle.halfSize;
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionOrientedRectangle.tag,
                    currentCollisionOrientedRectangle.enabled,
                    currentPhysicsType
                );
            }
            else if (Math::FloatEqualToFloat(newRotation, glm::half_pi<float>()) ||
                Math::FloatEqualToFloat(newRotation, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft(newPosition.x - currentCollisionOrientedRectangle.halfSize.y,
                    newPosition.y - currentCollisionOrientedRectangle.halfSize.x);
                glm::vec2 newTopRight(newPosition.y - currentCollisionOrientedRectangle.halfSize.x,
                    newPosition.x - currentCollisionOrientedRectangle.halfSize.y);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionOrientedRectangle.tag,
                    currentCollisionOrientedRectangle.enabled,
                    currentPhysicsType
                );
            }
            else
            {
                transformedCollisionOrientedRectangles_.emplace_back(
                    currentCollisionOrientedRectangle.halfSize,
                    newPosition,
                    newRotation,
                    currentCollisionOrientedRectangle.tag,
                    currentCollisionOrientedRectangle.enabled,
                    currentPhysicsType
                );
            }
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& currentCollisionCircle = collisionCircles_[i];
            if (!currentCollisionCircle.enabled)
            {
                continue;
            }

            CollisionShape2D::PhysicsType currentPhysicsType = currentCollisionCircle.physicsType;
            if (currentPhysicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE)
            {
                currentPhysicsType = physicsType_;
            }

            glm::vec2 newPosition = Math::Rotate2DVector(currentCollisionCircle.position, rotation_) + position_;
            transformedCollisionCircles_.emplace_back(
                newPosition,
                currentCollisionCircle.radius,
                currentCollisionCircle.tag,
                currentCollisionCircle.enabled,
                currentPhysicsType
            );
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& currentCollisionCapsule = collisionCapsules_[i];
            if (!currentCollisionCapsule.enabled)
            {
                continue;
            }

            CollisionShape2D::PhysicsType currentPhysicsType = currentCollisionCapsule.physicsType;
            if (currentPhysicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE)
            {
                currentPhysicsType = physicsType_;
            }

            glm::vec2 newStart = Math::Rotate2DVector(currentCollisionCapsule.start, rotation_) + position_;
            glm::vec2 newEnd = Math::Rotate2DVector(currentCollisionCapsule.end, rotation_) + position_;
            transformedCollisionCapsules_.emplace_back(
                newStart,
                newEnd,
                currentCollisionCapsule.radius,
                currentCollisionCapsule.tag,
                currentCollisionCapsule.enabled,
                currentPhysicsType
            );
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& currentCollisionTriangle = collisionTriangles_[i];
            if (!currentCollisionTriangle.enabled)
            {
                continue;
            }

            CollisionShape2D::PhysicsType currentPhysicsType = currentCollisionTriangle.physicsType;
            if (currentPhysicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE)
            {
                currentPhysicsType = physicsType_;
            }

            glm::vec2 newCorner1 = Math::Rotate2DVector(currentCollisionTriangle.corner1, rotation_) + position_;
            glm::vec2 newCorner2 = Math::Rotate2DVector(currentCollisionTriangle.corner2, rotation_) + position_;
            glm::vec2 newCorner3 = Math::Rotate2DVector(currentCollisionTriangle.corner3, rotation_) + position_;
            transformedCollisionTriangles_.emplace_back(
                newCorner1,
                newCorner2,
                newCorner3,
                currentCollisionTriangle.tag,
                currentCollisionTriangle.enabled,
                currentPhysicsType
            );
        }

        for (size_t i = 0; i < collisionPolygons_.size(); ++i)
        {
            const CollisionPolygon2D& currentCollisionPolygon = collisionPolygons_[i];
            if (!currentCollisionPolygon.enabled)
            {
                continue;
            }

            if (currentCollisionPolygon.corners.size() > 2)
            {
                transformedCollisionPolygons_.emplace_back();
                CollisionPolygon2D& transformedCollisionPolygon = transformedCollisionPolygons_.back();
                transformedCollisionPolygon.corners.reserve(currentCollisionPolygon.corners.size());
                for (size_t j = 0; j < currentCollisionPolygon.corners.size(); ++j)
                {
                    const glm::vec2& currentCorner = currentCollisionPolygon.corners[j];
                    glm::vec2 newCorner = Math::Rotate2DVector(currentCorner, rotation_) + position_;
                    transformedCollisionPolygon.corners.push_back(newCorner);
                }
                transformedCollisionPolygon.tag = currentCollisionPolygon.tag;
                transformedCollisionPolygon.enabled = currentCollisionPolygon.enabled;
            }
            else if (currentCollisionPolygon.corners.size() == 2)
            {

                glm::vec2 newStart = Math::Rotate2DVector(currentCollisionPolygon.corners[0], rotation_) + position_;
                glm::vec2 newEnd = Math::Rotate2DVector(currentCollisionPolygon.corners[1], rotation_) + position_;
                transformedCollisionLineSegments_.emplace_back(
                    newStart,
                    newEnd,
                    currentCollisionPolygon.tag,
                    currentCollisionPolygon.enabled
                );
            }
            else if (currentCollisionPolygon.corners.size() == 1)
            {
                glm::vec2 newPosition = Math::Rotate2DVector(currentCollisionPolygon.corners[0], rotation_) + position_;
                transformedCollisionPoints_.emplace_back(
                    newPosition,
                    currentCollisionPolygon.tag,
                    currentCollisionPolygon.enabled
                );
            }
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& currentCollisionConvexPolygon = collisionConvexPolygons_[i];
            if (!currentCollisionConvexPolygon.enabled)
            {
                continue;
            }

            CollisionShape2D::PhysicsType currentPhysicsType = currentCollisionConvexPolygon.physicsType;
            if (currentPhysicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE)
            {
                currentPhysicsType = physicsType_;
            }

            if (currentCollisionConvexPolygon.corners.size() > 2)
            {
                transformedCollisionConvexPolygons_.emplace_back();
                CollisionConvexPolygon2D& transformedCollisionConvexPolygon = transformedCollisionConvexPolygons_.back();
                transformedCollisionConvexPolygon.corners.reserve(currentCollisionConvexPolygon.corners.size());
                for (size_t j = 0; j < currentCollisionConvexPolygon.corners.size(); ++j)
                {
                    const glm::vec2& currentCorner = currentCollisionConvexPolygon.corners[j];
                    glm::vec2 newCorner = Math::Rotate2DVector(currentCorner, rotation_) + position_;
                    transformedCollisionConvexPolygon.corners.push_back(newCorner);
                }
                transformedCollisionConvexPolygon.tag = currentCollisionConvexPolygon.tag;
                transformedCollisionConvexPolygon.enabled = currentCollisionConvexPolygon.enabled;
                transformedCollisionConvexPolygon.physicsType = currentPhysicsType;
            }
            else if (currentCollisionConvexPolygon.corners.size() == 2)
            {

                glm::vec2 newStart = Math::Rotate2DVector(currentCollisionConvexPolygon.corners[0], rotation_) + position_;
                glm::vec2 newEnd = Math::Rotate2DVector(currentCollisionConvexPolygon.corners[1], rotation_) + position_;
                transformedCollisionLineSegments_.emplace_back(
                    newStart,
                    newEnd,
                    currentCollisionConvexPolygon.tag,
                    currentCollisionConvexPolygon.enabled
                );
            }
            else if (currentCollisionConvexPolygon.corners.size() == 1)
            {
                glm::vec2 newPosition = Math::Rotate2DVector(currentCollisionConvexPolygon.corners[0], rotation_) + position_;
                transformedCollisionPoints_.emplace_back(
                    newPosition,
                    currentCollisionConvexPolygon.tag,
                    currentCollisionConvexPolygon.enabled
                );
            }
        }

        transformedCollisionShapesUpToDate_ = true;
    }
}