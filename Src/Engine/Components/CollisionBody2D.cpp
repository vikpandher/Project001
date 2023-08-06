#include "CollisionBody2D.h"

#include "Engine/Math/Overlap2D.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionBody2D::CalculateBoundingRadius()
    {
        float largestRadius = 0.0f;

        for (size_t i = 0; i < points_.size(); ++i)
        {
            const Point2D& current = points_[i];
            const glm::vec2& position = current.position;
            float radius = std::sqrtf(position.x * position.x + position.y * position.y);
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        if (lines_.size() > 0 || rays_.size() > 0)
        {
            boundingRadius_ = std::numeric_limits<float>::infinity();
            return;
        }

        for (size_t i = 0; i < lineSegments_.size(); ++i)
        {
            const LineSegment2D& current = lineSegments_[i];
            const glm::vec2& start = current.start;
            const glm::vec2& end = current.end;
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

        for (size_t i = 0; i < rectangles_.size(); ++i)
        {
            const Rectangle2D& current = rectangles_[i];
            const glm::vec2& bottomLeft = current.bottomLeft;
            const glm::vec2& topRight = current.topRight;
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

        for (size_t i = 0; i < orientedRectangles_.size(); ++i)
        {
            const OrientedRectangle2D& current = orientedRectangles_[i];
            const glm::vec2& halfSize = current.halfSize;
            const glm::vec2& position = current.position;
            float rotation = current.rotation;
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

        for (size_t i = 0; i < circles_.size(); ++i)
        {
            const Circle2D& current = circles_[i];
            const glm::vec2& position = current.position;
            float radius = std::sqrtf(position.x * position.x + position.y * position.y);
            radius += current.radius;
            if (radius > largestRadius)
            {
                largestRadius = radius;
            }
        }

        for (size_t i = 0; i < capsules_.size(); ++i)
        {
            const Capsule2D& current = capsules_[i];
            const glm::vec2& start = current.start;
            const glm::vec2& end = current.end;
            float radius1 = std::sqrtf(start.x * start.x + start.y * start.y);
            radius1 += current.radius;
            float radius2 = std::sqrtf(end.x * end.x + end.y * end.y);
            radius2 += current.radius;
            if (radius1 > largestRadius)
            {
                largestRadius = radius1;
            }
            if (radius2 > largestRadius)
            {
                largestRadius = radius2;
            }
        }

        for (size_t i = 0; i < triangles_.size(); ++i)
        {
            const Triangle2D& current = triangles_[i];
            const glm::vec2& corner1 = current.corner1;
            const glm::vec2& corner2 = current.corner2;
            const glm::vec2& corner3 = current.corner3;
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

        for (size_t i = 0; i < polygons_.size(); ++i)
        {
            const Polygon2D& current = polygons_[i];
            for (size_t j = 0; j < current.positions.size(); ++j)
            {
                const glm::vec2& corner = current.positions[j];
                float radius = std::sqrtf(corner.x * corner.x + corner.y * corner.y);
                if (radius > largestRadius)
                {
                    largestRadius = radius;
                }
            }
        }

        for (size_t i = 0; i < convexPolygons_.size(); ++i)
        {
            const ConvexPolygon2D& current = convexPolygons_[i];
            for (size_t j = 0; j < current.positions.size(); ++j)
            {
                const glm::vec2& corner = current.positions[j];
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

    void CollisionBody2D::CalculateTransformedShapes()
    {
        transformedPoints_.clear();
        transformedLines_.clear();
        transformedRays_.clear();
        transformedLineSegments_.clear();
        transformedRectangles_.clear();
        transformedOrientedRectangles_.clear();
        transformedCircles_.clear();
        transformedCapsules_.clear();
        transformedTriangles_.clear();
        transformedPolygons_.clear();
        transformedConvexPolygons_.clear();

        transformedPointIds_.clear();
        transformedLineIds_.clear();
        transformedRayIds_.clear();
        transformedLineSegmentIds_.clear();
        transformedRectangleIds_.clear();
        transformedOrientedRectangleIds_.clear();
        transformedCircleIds_.clear();
        transformedCapsuleIds_.clear();
        transformedTriangleIds_.clear();
        transformedPolygonIds_.clear();
        transformedConvexPolygonIds_.clear();

        for (size_t i = 0; i < points_.size(); ++i)
        {
            const Point2D& currentPoint = points_[i];
            glm::vec2 newPosition = Rotate2DVector(currentPoint.position, rotation_) + position_;
            transformedPoints_.emplace_back(newPosition);
            transformedPointIds_.emplace_back(pointIds_[i]);
        }

        for (size_t i = 0; i < lines_.size(); ++i)
        {
            const Line2D& currentLine = lines_[i];
            glm::vec2 newPosition = Rotate2DVector(currentLine.position, rotation_) + position_;
            float newSlope = RotateSlope(currentLine.slope, rotation_);
            transformedLines_.emplace_back(
                newPosition,
                newSlope);
            transformedLineIds_.emplace_back(lineIds_[i]);
        }

        for (size_t i = 0; i < rays_.size(); ++i)
        {
            const Ray2D& currentRay = rays_[i];
            glm::vec2 newPosition = currentRay.position + position_;
            glm::vec2 newDirectoin = Rotate2DVector(currentRay.direction, rotation_);
            transformedRays_.emplace_back(
                newPosition,
                newDirectoin);
            transformedRayIds_.emplace_back(rayIds_[i]);
        }

        for (size_t i = 0; i < lineSegments_.size(); ++i)
        {
            const LineSegment2D& currentLineSegment = lineSegments_[i];
            glm::vec2 newStart = Rotate2DVector(currentLineSegment.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentLineSegment.end, rotation_) + position_;
            transformedLineSegments_.emplace_back(
                newStart,
                newEnd);
            transformedLineSegmentIds_.emplace_back(lineSegmentIds_[i]);
        }

        for (size_t i = 0; i < rectangles_.size(); ++i)
        {
            const Rectangle2D& currentRectangle = rectangles_[i];
            if (FloatEqualToFloat(rotation_, 0.0f))
            {
                glm::vec2 newBottomLeft = currentRectangle.bottomLeft + position_;
                glm::vec2 newTopRight = currentRectangle.topRight + position_;
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
                transformedRectangleIds_.emplace_back(rectangleIds_[i]);
            }
            else if (FloatEqualToFloat(rotation_, glm::half_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentRectangle.bottomLeft, glm::half_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentRectangle.topRight, glm::half_pi<float>()) + position_;
                Swap(newBottomLeft.x, newTopRight.x);
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
                transformedRectangleIds_.emplace_back(rectangleIds_[i]);
            }
            else if (FloatEqualToFloat(rotation_, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentRectangle.bottomLeft, glm::pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentRectangle.topRight, glm::pi<float>()) + position_;
                transformedRectangles_.emplace_back(
                    newTopRight,
                    newBottomLeft);
                transformedRectangleIds_.emplace_back(rectangleIds_[i]);
            }
            else if (FloatEqualToFloat(rotation_, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentRectangle.bottomLeft, glm::three_over_two_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentRectangle.topRight, glm::three_over_two_pi<float>()) + position_;
                Swap(newBottomLeft.y, newTopRight.y);
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
                transformedRectangleIds_.emplace_back(rectangleIds_[i]);
            }
            else
            {
                glm::vec2 newHalfSize = (currentRectangle.topRight - currentRectangle.bottomLeft) / 2.0f;
                glm::vec2 newPosition = (currentRectangle.bottomLeft + currentRectangle.topRight) / 2.0f;
                newPosition = Rotate2DVector(newPosition, rotation_) + position_;
                transformedOrientedRectangles_.emplace_back(newHalfSize, newPosition, rotation_);
                transformedOrientedRectangleIds_.emplace_back(rectangleIds_[i]);
            }
        }

        for (size_t i = 0; i < orientedRectangles_.size(); ++i)
        {
            const OrientedRectangle2D& currentOrientedRectangle = orientedRectangles_[i];
            glm::vec2 newPosition = Rotate2DVector(currentOrientedRectangle.position, rotation_) + position_;
            float newRotation = currentOrientedRectangle.rotation + rotation_;
            if (FloatEqualToFloat(newRotation, 0.0f) || FloatEqualToFloat(newRotation, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = newPosition - currentOrientedRectangle.halfSize;
                glm::vec2 newTopRight = newPosition + currentOrientedRectangle.halfSize;
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
                transformedRectangleIds_.emplace_back(orientedRectangleIds_[i]);
            }
            else if (FloatEqualToFloat(newRotation, glm::half_pi<float>()) ||
                FloatEqualToFloat(newRotation, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft(newPosition.x - currentOrientedRectangle.halfSize.y,
                    newPosition.y - currentOrientedRectangle.halfSize.x);
                glm::vec2 newTopRight(newPosition.y - currentOrientedRectangle.halfSize.x,
                    newPosition.x - currentOrientedRectangle.halfSize.y);
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
                transformedRectangleIds_.emplace_back(orientedRectangleIds_[i]);
            }
            else
            {
                transformedOrientedRectangles_.emplace_back(
                    currentOrientedRectangle.halfSize,
                    newPosition,
                    newRotation);
                transformedOrientedRectangleIds_.emplace_back(orientedRectangleIds_[i]);
            }
        }

        for (size_t i = 0; i < circles_.size(); ++i)
        {
            const Circle2D& currentCircle = circles_[i];
            glm::vec2 newPosition = Rotate2DVector(currentCircle.position, rotation_) + position_;
            transformedCircles_.emplace_back(
                newPosition,
                currentCircle.radius);
            transformedCircleIds_.emplace_back(circleIds_[i]);
        }

        for (size_t i = 0; i < capsules_.size(); ++i)
        {
            const Capsule2D& currentCapsule = capsules_[i];
            glm::vec2 newStart = Rotate2DVector(currentCapsule.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentCapsule.end, rotation_) + position_;
            transformedCapsules_.emplace_back(
                newStart,
                newEnd,
                currentCapsule.radius);
            transformedCapsuleIds_.emplace_back(capsuleIds_[i]);
        }

        for (size_t i = 0; i < triangles_.size(); ++i)
        {
            const Triangle2D& currentTriangle = triangles_[i];
            glm::vec2 newCorner1 = Rotate2DVector(currentTriangle.corner1, rotation_) + position_;
            glm::vec2 newCorner2 = Rotate2DVector(currentTriangle.corner2, rotation_) + position_;
            glm::vec2 newCorner3 = Rotate2DVector(currentTriangle.corner3, rotation_) + position_;
            transformedTriangles_.emplace_back(
                newCorner1,
                newCorner2,
                newCorner3);
            transformedTriangleIds_.emplace_back(triangleIds_[i]);
        }

        for (size_t i = 0; i < polygons_.size(); ++i)
        {
            const Polygon2D& currentPolygon = polygons_[i];
            transformedPolygons_.emplace_back();
            Polygon2D& transformedPolygon = transformedPolygons_.back();
            transformedPolygon.positions.reserve(currentPolygon.positions.size());
            for (size_t j = 0; j < currentPolygon.positions.size(); ++j)
            {
                const glm::vec2& currentCorner = currentPolygon.positions[j];
                glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                transformedPolygon.positions.push_back(newCorner);
            }
            transformedPolygonIds_.emplace_back(polygonIds_[i]);
        }

        for (size_t i = 0; i < convexPolygons_.size(); ++i)
        {
            const ConvexPolygon2D& currentConvexPolygon = convexPolygons_[i];
            transformedConvexPolygons_.emplace_back();
            ConvexPolygon2D& transformedConvexPolygon = transformedConvexPolygons_.back();
            transformedConvexPolygon.positions.reserve(currentConvexPolygon.positions.size());
            for (size_t j = 0; j < currentConvexPolygon.positions.size(); ++j)
            {
                const glm::vec2& currentCorner = currentConvexPolygon.positions[j];
                glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                transformedConvexPolygon.positions.push_back(newCorner);
            }
            transformedConvexPolygonIds_.emplace_back(convexPolygonIds_[i]);
        }

        transformedShapesUpToDate_ = true;
    }
}