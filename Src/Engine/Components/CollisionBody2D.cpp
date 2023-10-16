#include "CollisionBody2D.h"

#include "Engine/Math/Overlap2D.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionBody2D::CalculateBoundingRadius()
    {
        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& currentLine = collisionLines_[i];
            if (currentLine.tangible_)
            {
                boundingRadius_ = std::numeric_limits<float>::infinity();
                return;
            }
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& current = collisionRays_[i];
            if (current.tangible_)
            {
                boundingRadius_ = std::numeric_limits<float>::infinity();
                return;
            }
        }

        float largestRadius = 0.0f;

        for (size_t i = 0; i < collisionPoints_.size(); ++i)
        {
            const CollisionPoint2D& currentPoint = collisionPoints_[i];
            if (!currentPoint.tangible_)
            {
                continue;
            }

            const glm::vec2& position = currentPoint.position;
            float radius = std::sqrtf(position.x * position.x + position.y * position.y);
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& currentLineSegment = collisionLineSegments_[i];
            if (!currentLineSegment.tangible_)
            {
                continue;
            }

            const glm::vec2& start = currentLineSegment.start;
            const glm::vec2& end = currentLineSegment.end;
            float radius1 = std::sqrtf(start.x * start.x + start.y * start.y);
            float radius2 = std::sqrtf(end.x * end.x + end.y * end.y);
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
            if (!currentRectangle.tangible_)
            {
                continue;
            }

            const glm::vec2& bottomLeft = currentRectangle.bottomLeft;
            const glm::vec2& topRight = currentRectangle.topRight;
            float radiusBottomLeft = std::sqrtf(bottomLeft.x * bottomLeft.x + bottomLeft.y * bottomLeft.y);
            float radiusBottomRight = std::sqrtf(topRight.x * topRight.x + bottomLeft.y * bottomLeft.y);
            float radiusTopRight = std::sqrtf(topRight.x * topRight.x + topRight.y * topRight.y);
            float radiusTopLeft = std::sqrtf(bottomLeft.x * bottomLeft.x + topRight.y * topRight.y);
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
            if (!currentOrientedRectangle.tangible_)
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
            float radius1 = std::sqrtf(corner1.x * corner1.x + corner1.y * corner1.y);
            float radius2 = std::sqrtf(corner2.x * corner2.x + corner2.y * corner2.y);
            float radius3 = std::sqrtf(corner3.x * corner3.x + corner3.y * corner3.y);
            float radius4 = std::sqrtf(corner4.x * corner4.x + corner4.y * corner4.y);
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
            if (!currentCircle.tangible_)
            {
                continue;
            }

            const glm::vec2& position = currentCircle.position;
            float radius = std::sqrtf(position.x * position.x + position.y * position.y);
            radius += currentCircle.radius;
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& currentCapsule = collisionCapsules_[i];
            if (!currentCapsule.tangible_)
            {
                continue;
            }

            const glm::vec2& start = currentCapsule.start;
            const glm::vec2& end = currentCapsule.end;
            float radius1 = std::sqrtf(start.x * start.x + start.y * start.y);
            radius1 += currentCapsule.radius;
            float radius2 = std::sqrtf(end.x * end.x + end.y * end.y);
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
            if (!currentTriangle.tangible_)
            {
                continue;
            }

            const glm::vec2& corner1 = currentTriangle.corner1;
            const glm::vec2& corner2 = currentTriangle.corner2;
            const glm::vec2& corner3 = currentTriangle.corner3;
            float radius1 = std::sqrtf(corner1.x * corner1.x + corner1.y * corner1.y);
            float radius2 = std::sqrtf(corner2.x * corner2.x + corner2.y * corner2.y);
            float radius3 = std::sqrtf(corner3.x * corner3.x + corner3.y * corner3.y);
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
            if (!currentPolygon.tangible_)
            {
                continue;
            }

            for (size_t j = 0; j < currentPolygon.positions.size(); ++j)
            {
                const glm::vec2& corner = currentPolygon.positions[j];
                float radius = std::sqrtf(corner.x * corner.x + corner.y * corner.y);
                if (radius > largestRadius)
                {
                    largestRadius = radius;
                }
            }
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& currentConvexPolygon = collisionConvexPolygons_[i];
            if (!currentConvexPolygon.tangible_)
            {
                continue;
            }

            for (size_t j = 0; j < currentConvexPolygon.positions.size(); ++j)
            {
                const glm::vec2& corner = currentConvexPolygon.positions[j];
                float radius = std::sqrtf(corner.x * corner.x + corner.y * corner.y);
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
            if (!currentCollisionPoint.tangible_)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionPoint.position, rotation_) + position_;
            transformedCollisionPoints_.emplace_back(
                newPosition,
                currentCollisionPoint.id_,
                currentCollisionPoint.tangible_
            );
        }

        for (size_t i = 0; i < collisionLines_.size(); ++i)
        {
            const CollisionLine2D& currentCollisionLine = collisionLines_[i];
            if (!currentCollisionLine.tangible_)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionLine.position, rotation_) + position_;
            float newSlope = RotateSlope(currentCollisionLine.slope, rotation_);
            transformedCollisionLines_.emplace_back(
                newPosition,
                newSlope,
                currentCollisionLine.id_,
                currentCollisionLine.tangible_
            );
        }

        for (size_t i = 0; i < collisionRays_.size(); ++i)
        {
            const CollisionRay2D& currentCollisionRay = collisionRays_[i];
            if (!currentCollisionRay.tangible_)
            {
                continue;
            }

            glm::vec2 newPosition = currentCollisionRay.position + position_;
            glm::vec2 newDirectoin = Rotate2DVector(currentCollisionRay.direction, rotation_);
            transformedCollisionRays_.emplace_back(
                newPosition,
                newDirectoin,
                currentCollisionRay.id_,
                currentCollisionRay.tangible_
            );
        }

        for (size_t i = 0; i < collisionLineSegments_.size(); ++i)
        {
            const CollisionLineSegment2D& currentCollisionLineSegment = collisionLineSegments_[i];
            if (!currentCollisionLineSegment.tangible_)
            {
                continue;
            }

            glm::vec2 newStart = Rotate2DVector(currentCollisionLineSegment.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentCollisionLineSegment.end, rotation_) + position_;
            transformedCollisionLineSegments_.emplace_back(
                newStart,
                newEnd,
                currentCollisionLineSegment.id_,
                currentCollisionLineSegment.tangible_
            );
        }

        for (size_t i = 0; i < collisionRectangles_.size(); ++i)
        {
            const CollisionRectangle2D& currentCollisionRectangle = collisionRectangles_[i];
            if (!currentCollisionRectangle.tangible_)
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
                    currentCollisionRectangle.id_,
                    currentCollisionRectangle.tangible_
                );
            }
            else if (FloatEqualToFloat(rotation_, glm::half_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::half_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentCollisionRectangle.topRight, glm::half_pi<float>()) + position_;
                Swap(newBottomLeft.x, newTopRight.x);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.id_,
                    currentCollisionRectangle.tangible_
                );
            }
            else if (FloatEqualToFloat(rotation_, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentCollisionRectangle.topRight, glm::pi<float>()) + position_;
                transformedCollisionRectangles_.emplace_back(
                    newTopRight,
                    newBottomLeft,
                    currentCollisionRectangle.id_,
                    currentCollisionRectangle.tangible_
                );
            }
            else if (FloatEqualToFloat(rotation_, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentCollisionRectangle.bottomLeft, glm::three_over_two_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentCollisionRectangle.topRight, glm::three_over_two_pi<float>()) + position_;
                Swap(newBottomLeft.y, newTopRight.y);
                transformedCollisionRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight,
                    currentCollisionRectangle.id_,
                    currentCollisionRectangle.tangible_
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
                    currentCollisionRectangle.id_,
                    currentCollisionRectangle.tangible_
                );
            }
        }

        for (size_t i = 0; i < collisionOrientedRectangles_.size(); ++i)
        {
            const CollisionOrientedRectangle2D& currentCollisionOrientedRectangle = collisionOrientedRectangles_[i];
            if (!currentCollisionOrientedRectangle.tangible_)
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
                    currentCollisionOrientedRectangle.id_,
                    currentCollisionOrientedRectangle.tangible_
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
                    currentCollisionOrientedRectangle.id_,
                    currentCollisionOrientedRectangle.tangible_
                );
            }
            else
            {
                transformedCollisionOrientedRectangles_.emplace_back(
                    currentCollisionOrientedRectangle.halfSize,
                    newPosition,
                    newRotation,
                    currentCollisionOrientedRectangle.id_,
                    currentCollisionOrientedRectangle.tangible_
                );
            }
        }

        for (size_t i = 0; i < collisionCircles_.size(); ++i)
        {
            const CollisionCircle2D& currentCollisionCircle = collisionCircles_[i];
            if (!currentCollisionCircle.tangible_)
            {
                continue;
            }

            glm::vec2 newPosition = Rotate2DVector(currentCollisionCircle.position, rotation_) + position_;
            transformedCollisionCircles_.emplace_back(
                newPosition,
                currentCollisionCircle.radius,
                currentCollisionCircle.id_,
                currentCollisionCircle.tangible_
            );
        }

        for (size_t i = 0; i < collisionCapsules_.size(); ++i)
        {
            const CollisionCapsule2D& currentCollisionCapsule = collisionCapsules_[i];
            if (!currentCollisionCapsule.tangible_)
            {
                continue;
            }

            glm::vec2 newStart = Rotate2DVector(currentCollisionCapsule.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentCollisionCapsule.end, rotation_) + position_;
            transformedCollisionCapsules_.emplace_back(
                newStart,
                newEnd,
                currentCollisionCapsule.radius,
                currentCollisionCapsule.id_,
                currentCollisionCapsule.tangible_
            );
        }

        for (size_t i = 0; i < collisionTriangles_.size(); ++i)
        {
            const CollisionTriangle2D& currentCollisionTriangle = collisionTriangles_[i];
            if (!currentCollisionTriangle.tangible_)
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
                currentCollisionTriangle.id_,
                currentCollisionTriangle.tangible_
            );
        }

        for (size_t i = 0; i < collisionPolygons_.size(); ++i)
        {
            const CollisionPolygon2D& currentCollisionPolygon = collisionPolygons_[i];
            if (!currentCollisionPolygon.tangible_)
            {
                continue;
            }

            transformedCollisionPolygons_.emplace_back();
            CollisionPolygon2D& transformedCollisionPolygon = transformedCollisionPolygons_.back();
            transformedCollisionPolygon.positions.reserve(currentCollisionPolygon.positions.size());
            for (size_t j = 0; j < currentCollisionPolygon.positions.size(); ++j)
            {
                const glm::vec2& currentCorner = currentCollisionPolygon.positions[j];
                glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                transformedCollisionPolygon.positions.push_back(newCorner);
            }
            transformedCollisionPolygon.id_ = currentCollisionPolygon.id_;
            transformedCollisionPolygon.tangible_ = currentCollisionPolygon.tangible_;
        }

        for (size_t i = 0; i < collisionConvexPolygons_.size(); ++i)
        {
            const CollisionConvexPolygon2D& currentCollisionConvexPolygon = collisionConvexPolygons_[i];
            if (!currentCollisionConvexPolygon.tangible_)
            {
                continue;
            }

            transformedCollisionConvexPolygons_.emplace_back();
            CollisionConvexPolygon2D& transformedCollisionConvexPolygon = transformedCollisionConvexPolygons_.back();
            transformedCollisionConvexPolygon.positions.reserve(currentCollisionConvexPolygon.positions.size());
            for (size_t j = 0; j < currentCollisionConvexPolygon.positions.size(); ++j)
            {
                const glm::vec2& currentCorner = currentCollisionConvexPolygon.positions[j];
                glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                transformedCollisionConvexPolygon.positions.push_back(newCorner);
            }
            transformedCollisionConvexPolygon.id_ = currentCollisionConvexPolygon.id_;
            transformedCollisionConvexPolygon.tangible_ = currentCollisionConvexPolygon.tangible_;
        }

        transformedCollisionShapesUpToDate_ = true;
    }
}