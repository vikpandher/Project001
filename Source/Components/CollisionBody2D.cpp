#include "CollisionBody2D.h"

#include "Math/Overlap2D.h"

#include <limits>



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionBody2D::CalculateBoundingRadius()
    {
        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& currentLine = collisionLines_[i];
            if (currentLine.tangible)
            {
                boundingRadius_ = std::numeric_limits<float>::infinity();
                return;
            }
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& current = collisionRays_[i];
            if (current.tangible)
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
            if (!currentPoint.tangible)
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
            if (!currentLineSegment.tangible)
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
            if (!currentRectangle.tangible)
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
            if (!currentOrientedRectangle.tangible)
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
            Rotate2DVector(corner1, rotation);
            Rotate2DVector(corner2, rotation);
            Rotate2DVector(corner3, rotation);
            Rotate2DVector(corner4, rotation);
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
            if (!currentCircle.tangible)
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
            if (!currentCapsule.tangible)
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
            if (!currentTriangle.tangible)
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
            if (!currentPolygon.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < currentPolygon.positions.size(); ++j)
            {
                const glm::vec2& corner = currentPolygon.positions[j];
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
            if (!currentConvexPolygon.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < currentConvexPolygon.positions.size(); ++j)
            {
                const glm::vec2& corner = currentConvexPolygon.positions[j];
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
            if (!currentCollisionPoint.tangible)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionPoint.position, rotation_) + position_;
            transformedCollisionPoints_.emplace_back(
                newPosition,
                currentCollisionPoint.tag,
                currentCollisionPoint.tangible
            );
        }

        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& currentCollisionLine = collisionLines_[i];
            if (!currentCollisionLine.tangible)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionLine.position, rotation_) + position_;
            float newSlope = RotateSlope(currentCollisionLine.slope, rotation_);
            transformedCollisionLines_.emplace_back(
                newPosition,
                newSlope,
                currentCollisionLine.tag,
                currentCollisionLine.tangible
            );
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& currentCollisionRay = collisionRays_[i];
            if (!currentCollisionRay.tangible)
            {
                continue;
            }

            glm::vec2 newPosition = currentCollisionRay.position + position_;
            glm::vec2 newDirectoin = Rotate2DVector(currentCollisionRay.direction, rotation_);
            transformedCollisionRays_.emplace_back(
                newPosition,
                newDirectoin,
                currentCollisionRay.tag,
                currentCollisionRay.tangible
            );
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& currentCollisionLineSegment = collisionLineSegments_[i];
            if (!currentCollisionLineSegment.tangible)
            {
                continue;
            }

            glm::vec2 newStart = Rotate2DVector(currentCollisionLineSegment.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentCollisionLineSegment.end, rotation_) + position_;
            transformedCollisionLineSegments_.emplace_back(
                newStart,
                newEnd,
                currentCollisionLineSegment.tag,
                currentCollisionLineSegment.tangible
            );
        }

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& currentCollisionRectangle = collisionRectangles_[i];
            if (!currentCollisionRectangle.tangible)
            {
                continue;
            }

            if (FloatEqualToFloat(rotation_, 0.0f))
            {
                glm::vec2 newBottomLeft = currentCollisionRectangle.bottomLeft + position_;
                glm::vec2 newTopRight = currentCollisionRectangle.topRight + position_;
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.tangible
                );
            }
            else if (FloatEqualToFloat(rotation_, glm::half_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::half_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentCollisionRectangle.topRight, glm::half_pi<float>()) + position_;
                std::swap(newBottomLeft.x, newTopRight.x);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.tangible
                );
            }
            else if (FloatEqualToFloat(rotation_, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentCollisionRectangle.topRight, glm::pi<float>()) + position_;
                transformedCollisionRectangles_.emplace_back(
                    newTopRight,
                    newBottomLeft,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.tangible
                );
            }
            else if (FloatEqualToFloat(rotation_, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::three_over_two_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentCollisionRectangle.topRight, glm::three_over_two_pi<float>()) + position_;
                std::swap(newBottomLeft.y, newTopRight.y);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.tangible
                );
            }
            else
            {
                glm::vec2 newHalfSize = (currentCollisionRectangle.topRight - currentCollisionRectangle.bottomLeft) / 2.0f;
                glm::vec2 newPosition = (currentCollisionRectangle.bottomLeft + currentCollisionRectangle.topRight) / 2.0f;
                newPosition = Rotate2DVector(newPosition, rotation_) + position_;
                transformedCollisionOrientedRectangles_.emplace_back(
                    newHalfSize,
                    newPosition,
                    rotation_,
                    currentCollisionRectangle.tag,
                    currentCollisionRectangle.tangible
                );
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& currentCollisionOrientedRectangle = collisionOrientedRectangles_[i];
            if (!currentCollisionOrientedRectangle.tangible)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionOrientedRectangle.position, rotation_) + position_;
            float newRotation = currentCollisionOrientedRectangle.rotation + rotation_;
            if (FloatEqualToFloat(newRotation, 0.0f) || FloatEqualToFloat(newRotation, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = newPosition - currentCollisionOrientedRectangle.halfSize;
                glm::vec2 newTopRight = newPosition + currentCollisionOrientedRectangle.halfSize;
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionOrientedRectangle.tag,
                    currentCollisionOrientedRectangle.tangible
                );
            }
            else if (FloatEqualToFloat(newRotation, glm::half_pi<float>()) ||
                FloatEqualToFloat(newRotation, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft(newPosition.x - currentCollisionOrientedRectangle.halfSize.y,
                    newPosition.y - currentCollisionOrientedRectangle.halfSize.x);
                glm::vec2 newTopRight(newPosition.y - currentCollisionOrientedRectangle.halfSize.x,
                    newPosition.x - currentCollisionOrientedRectangle.halfSize.y);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionOrientedRectangle.tag,
                    currentCollisionOrientedRectangle.tangible
                );
            }
            else
            {
                transformedCollisionOrientedRectangles_.emplace_back(
                    currentCollisionOrientedRectangle.halfSize,
                    newPosition,
                    newRotation,
                    currentCollisionOrientedRectangle.tag,
                    currentCollisionOrientedRectangle.tangible
                );
            }
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& currentCollisionCircle = collisionCircles_[i];
            if (!currentCollisionCircle.tangible)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionCircle.position, rotation_) + position_;
            transformedCollisionCircles_.emplace_back(
                newPosition,
                currentCollisionCircle.radius,
                currentCollisionCircle.tag,
                currentCollisionCircle.tangible
            );
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& currentCollisionCapsule = collisionCapsules_[i];
            if (!currentCollisionCapsule.tangible)
            {
                continue;
            }

            glm::vec2 newStart = Rotate2DVector(currentCollisionCapsule.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentCollisionCapsule.end, rotation_) + position_;
            transformedCollisionCapsules_.emplace_back(
                newStart,
                newEnd,
                currentCollisionCapsule.radius,
                currentCollisionCapsule.tag,
                currentCollisionCapsule.tangible
            );
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& currentCollisionTriangle = collisionTriangles_[i];
            if (!currentCollisionTriangle.tangible)
            {
                continue;
            }

            glm::vec2 newCorner1 = Rotate2DVector(currentCollisionTriangle.corner1, rotation_) + position_;
            glm::vec2 newCorner2 = Rotate2DVector(currentCollisionTriangle.corner2, rotation_) + position_;
            glm::vec2 newCorner3 = Rotate2DVector(currentCollisionTriangle.corner3, rotation_) + position_;
            transformedCollisionTriangles_.emplace_back(
                newCorner1,
                newCorner2,
                newCorner3,
                currentCollisionTriangle.tag,
                currentCollisionTriangle.tangible
            );
        }

        for (size_t i = 0; i < collisionPolygons_.size(); ++i)
        {
            const CollisionPolygon2D& currentCollisionPolygon = collisionPolygons_[i];
            if (!currentCollisionPolygon.tangible)
            {
                continue;
            }

            if (currentCollisionPolygon.positions.size() > 2)
            {
                transformedCollisionPolygons_.emplace_back();
                CollisionPolygon2D& transformedCollisionPolygon = transformedCollisionPolygons_.back();
                transformedCollisionPolygon.positions.reserve(currentCollisionPolygon.positions.size());
                for (size_t j = 0; j < currentCollisionPolygon.positions.size(); ++j)
                {
                    const glm::vec2& currentCorner = currentCollisionPolygon.positions[j];
                    glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                    transformedCollisionPolygon.positions.push_back(newCorner);
                }
                transformedCollisionPolygon.tag = currentCollisionPolygon.tag;
                transformedCollisionPolygon.tangible = currentCollisionPolygon.tangible;
            }
            else if (currentCollisionPolygon.positions.size() == 2)
            {

                glm::vec2 newStart = Rotate2DVector(currentCollisionPolygon.positions[0], rotation_) + position_;
                glm::vec2 newEnd = Rotate2DVector(currentCollisionPolygon.positions[1], rotation_) + position_;
                transformedCollisionLineSegments_.emplace_back(
                    newStart,
                    newEnd,
                    currentCollisionPolygon.tag,
                    currentCollisionPolygon.tangible
                );
            }
            else if (currentCollisionPolygon.positions.size() == 1)
            {
                glm::vec2 newPosition = Rotate2DVector(currentCollisionPolygon.positions[0], rotation_) + position_;
                transformedCollisionPoints_.emplace_back(
                    newPosition,
                    currentCollisionPolygon.tag,
                    currentCollisionPolygon.tangible
                );
            }
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& currentCollisionConvexPolygon = collisionConvexPolygons_[i];
            if (!currentCollisionConvexPolygon.tangible)
            {
                continue;
            }

            if (currentCollisionConvexPolygon.positions.size() > 2)
            {
                transformedCollisionConvexPolygons_.emplace_back();
                CollisionConvexPolygon2D& transformedCollisionConvexPolygon = transformedCollisionConvexPolygons_.back();
                transformedCollisionConvexPolygon.positions.reserve(currentCollisionConvexPolygon.positions.size());
                for (size_t j = 0; j < currentCollisionConvexPolygon.positions.size(); ++j)
                {
                    const glm::vec2& currentCorner = currentCollisionConvexPolygon.positions[j];
                    glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                    transformedCollisionConvexPolygon.positions.push_back(newCorner);
                }
                transformedCollisionConvexPolygon.tag = currentCollisionConvexPolygon.tag;
                transformedCollisionConvexPolygon.tangible = currentCollisionConvexPolygon.tangible;
            }
            else if (currentCollisionConvexPolygon.positions.size() == 2)
            {

                glm::vec2 newStart = Rotate2DVector(currentCollisionConvexPolygon.positions[0], rotation_) + position_;
                glm::vec2 newEnd = Rotate2DVector(currentCollisionConvexPolygon.positions[1], rotation_) + position_;
                transformedCollisionLineSegments_.emplace_back(
                    newStart,
                    newEnd,
                    currentCollisionConvexPolygon.tag,
                    currentCollisionConvexPolygon.tangible
                );
            }
            else if (currentCollisionConvexPolygon.positions.size() == 1)
            {
                glm::vec2 newPosition = Rotate2DVector(currentCollisionConvexPolygon.positions[0], rotation_) + position_;
                transformedCollisionPoints_.emplace_back(
                    newPosition,
                    currentCollisionConvexPolygon.tag,
                    currentCollisionConvexPolygon.tangible
                );
            }
        }

        transformedCollisionShapesUpToDate_ = true;
    }

    float CollisionBody2D::GetDistanceSquaredToPoint(const glm::vec2& point_position) const
    {
        float minDistanceSquared = std::numeric_limits<float>::infinity();

        glm::vec2 translatedPointPosition = point_position - position_;
        glm::vec2 rotatedPointPosition = Rotate2DVector(translatedPointPosition, -1.0f * rotation_);

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
                collisionPolygon.positions.data(),
                collisionPolygon.positions.size(),
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
                collisionConvexPolygon.positions.data(),
                collisionConvexPolygon.positions.size(),
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
        CollisionShape2D_Type collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_NONE;
        size_t collisionShapeIndex = -1;
        float minDistanceSquared = std::numeric_limits<float>::infinity();

        glm::vec2 translatedPointPosition = point_position - position_;
        glm::vec2 rotatedPointPosition = Rotate2DVector(translatedPointPosition, -1.0f * rotation_);

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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_POINT;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_LINE;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_RAY;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_LINE_SEGMENT;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_RECTANGLE;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_ORIENTED_RECTANGLE;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_CIRCLE;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_CAPSULE;
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
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_TRIANGLE;
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
                collisionPolygon.positions.data(),
                collisionPolygon.positions.size(),
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_POLYGON;
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
                collisionConvexPolygon.positions.data(),
                collisionConvexPolygon.positions.size(),
                currentClosestPointPosition);

            if (distanceSquared < minDistanceSquared)
            {
                collisionShape2D_Type = CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_CONVEX_POLYGON;
                collisionShapeIndex = i;
                closestPoint_position = currentClosestPointPosition;
                minDistanceSquared = distanceSquared;
            }
        }

        switch (collisionShape2D_Type)
        {
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_POINT:
        {
            const CollisionPoint2D& collisionPoint = collisionPoints_[collisionShapeIndex];
            closestPoint_position = collisionPoint.position;
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_LINE:
        {
            const CollisionLine2D& collisionLine = collisionLines_[collisionShapeIndex];
            Get2D_Point_Line_ClosestPoint(
                rotatedPointPosition,
                collisionLine.position,
                collisionLine.slope,
                closestPoint_position);
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_RAY:
        {
            const CollisionRay2D& collisionRay = collisionRays_[collisionShapeIndex];
            Get2D_Point_Ray_ClosestPoint(
                rotatedPointPosition,
                collisionRay.position,
                collisionRay.direction,
                closestPoint_position);
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_LINE_SEGMENT:
        {
            const CollisionLineSegment2D& collisionLineSegment = collisionLineSegments_[collisionShapeIndex];
            Get2D_Point_LineSegment_ClosestPoint(
                rotatedPointPosition,
                collisionLineSegment.start,
                collisionLineSegment.end,
                closestPoint_position);
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_RECTANGLE:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_ORIENTED_RECTANGLE:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_CIRCLE:
        {
            const CollisionCircle2D& collisionCircle = collisionCircles_[collisionShapeIndex];
            Get2D_Point_Circle_ClosestPoint(
                rotatedPointPosition,
                collisionCircle.position,
                collisionCircle.radius,
                closestPoint_position);
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_CAPSULE:
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
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_TRIANGLE:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_POLYGON:
        {
            // Already calculated
            break;
        }
        case CollisionShape2D_Type::COLLISION_SHAPE_2D_TYPE_CONVEX_POLYGON:
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

        closestPoint_position = Rotate2DVector(closestPoint_position, rotation_);
        closestPoint_position += position_;

        return minDistanceSquared;
    }
}