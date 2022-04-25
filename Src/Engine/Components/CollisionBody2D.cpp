#include "CollisionBody2D.h"

#include "Engine/Math/Overlap2D.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

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
            boundingRadius_ = INFINITY;
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
            const std::vector<glm::vec2>& current = polygons_[i];
            for (size_t j = 0; j < current.size(); ++j)
            {
                const glm::vec2& corner = current[j];
                float radius = std::sqrtf(corner.x * corner.x + corner.y * corner.y);
                if (radius > largestRadius)
                {
                    largestRadius = radius;
                }
            }
        }

        for (size_t i = 0; i < convexPolygons_.size(); ++i)
        {
            const std::vector<glm::vec2>& current = convexPolygons_[i];
            for (size_t j = 0; j < current.size(); ++j)
            {
                const glm::vec2& corner = current[j];
                float radius = std::sqrtf(corner.x * corner.x + corner.y * corner.y);
                if (radius > largestRadius)
                {
                    largestRadius = radius;
                }
            }
        }

        boundingRadius_ = largestRadius;
    }

    void CollisionBody2D::CalculateTransforedShapes()
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

        for (size_t i = 0; i < points_.size(); ++i)
        {
            const Point2D& currentPoint = points_[i];
            glm::vec2 newPosition = Rotate2DVector(currentPoint.position, rotation_) + position_;
            transformedPoints_.emplace_back(newPosition);
        }

        for (size_t i = 0; i < lines_.size(); ++i)
        {
            const Line2D& currentLine = lines_[i];
            glm::vec2 newPosition = Rotate2DVector(currentLine.position, rotation_) + position_;
            float newSlope = RotateSlope(currentLine.slope, rotation_);
            transformedLines_.emplace_back(
                newPosition,
                newSlope);
        }

        for (size_t i = 0; i < rays_.size(); ++i)
        {
            const Ray2D& currentRay = rays_[i];
            glm::vec2 newPosition = currentRay.position + position_;
            glm::vec2 newDirectoin = Rotate2DVector(currentRay.direction, rotation_);
            transformedRays_.emplace_back(
                newPosition,
                newDirectoin);
        }

        for (size_t i = 0; i < lineSegments_.size(); ++i)
        {
            const LineSegment2D& currentLineSegment = lineSegments_[i];
            glm::vec2 newStart = Rotate2DVector(currentLineSegment.start, rotation_) + position_;
            glm::vec2 newEnd = Rotate2DVector(currentLineSegment.end, rotation_) + position_;
            transformedLineSegments_.emplace_back(
                newStart,
                newEnd);
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
            }
            else if (FloatEqualToFloat(rotation_, glm::half_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentRectangle.bottomLeft, glm::half_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentRectangle.topRight, glm::half_pi<float>()) + position_;
                Swap(newBottomLeft.x, newTopRight.x);
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
            }
            else if (FloatEqualToFloat(rotation_, glm::pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentRectangle.bottomLeft, glm::pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentRectangle.topRight, glm::pi<float>()) + position_;
                transformedRectangles_.emplace_back(
                    newTopRight,
                    newBottomLeft);
            }
            else if (FloatEqualToFloat(rotation_, glm::three_over_two_pi<float>()))
            {
                glm::vec2 newBottomLeft = Rotate2DVector(currentRectangle.bottomLeft, glm::three_over_two_pi<float>()) + position_;
                glm::vec2 newTopRight = Rotate2DVector(currentRectangle.topRight, glm::three_over_two_pi<float>()) + position_;
                Swap(newBottomLeft.y, newTopRight.y);
                transformedRectangles_.emplace_back(
                    newBottomLeft,
                    newTopRight);
            }
            else
            {
                glm::vec2 newHalfSize = (currentRectangle.topRight - currentRectangle.bottomLeft) / 2.0f;
                glm::vec2 newPosition = (currentRectangle.bottomLeft + currentRectangle.topRight) / 2.0f;
                newPosition = Rotate2DVector(newPosition, rotation_) + position_;
                transformedOrientedRectangles_.emplace_back(newHalfSize, newPosition, rotation_);
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
            }
            else
            {
                transformedOrientedRectangles_.emplace_back(
                    currentOrientedRectangle.halfSize,
                    newPosition,
                    newRotation);
            }
        }

        for (size_t i = 0; i < circles_.size(); ++i)
        {
            const Circle2D& currentCircle = circles_[i];
            glm::vec2 newPosition = Rotate2DVector(currentCircle.position, rotation_) + position_;
            transformedCircles_.emplace_back(
                newPosition,
                currentCircle.radius);
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
        }

        for (size_t i = 0; i < polygons_.size(); ++i)
        {
            const std::vector<glm::vec2>& currentPolygon = polygons_[i];
            transformedPolygons_.emplace_back();
            std::vector<glm::vec2>& transformedPolygon = transformedPolygons_.back();
            transformedPolygon.reserve(currentPolygon.size());
            for (size_t j = 0; j < currentPolygon.size(); ++j)
            {
                const glm::vec2& currentCorner = currentPolygon[j];
                glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                transformedPolygon.push_back(newCorner);
            }
        }

        for (size_t i = 0; i < convexPolygons_.size(); ++i)
        {
            const std::vector<glm::vec2>& currentConvexPolygon = convexPolygons_[i];
            transformedConvexPolygons_.emplace_back();
            std::vector<glm::vec2>& transformedConvexPolygon = transformedConvexPolygons_.back();
            transformedConvexPolygon.reserve(currentConvexPolygon.size());
            for (size_t j = 0; j < currentConvexPolygon.size(); ++j)
            {
                const glm::vec2& currentCorner = currentConvexPolygon[j];
                glm::vec2 newCorner = Rotate2DVector(currentCorner, rotation_) + position_;
                transformedConvexPolygon.push_back(newCorner);
            }
        }
    }

    void CollisionBody2D::CalculateCollision(CollisionBody2D& other)
    {
        if (Check2D_Circle_Circle_Overlap(position_, boundingRadius_, other.position_, other.boundingRadius_))
        {
            // point & other point ---------------------------------------------
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Point_Point_Overlap(
                        currentPoint.position,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other line
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Point_Line_Overlap(
                        currentPoint.position,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other ray
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Point_Ray_Overlap(
                        currentPoint.position,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other lineSegment
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Point_LineSegment_Overlap(
                        currentPoint.position,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other rectangle
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Point_Rectangle_Overlap(
                        currentPoint.position,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other orientedRectangle
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Point_OrientedRectangle_Overlap(
                        currentPoint.position,
                        otherRectangle.halfSize,
                        otherRectangle.position,
                        otherRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other circle
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Point_Circle_Overlap(
                        currentPoint.position,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other capsule
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Point_Capsule_Overlap(
                        currentPoint.position,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other triangle
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Point_Triangle_Overlap(
                        currentPoint.position,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other polygon
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Point_Polygon_Overlap(
                        currentPoint.position,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // point & other convexPolygon
            for (size_t i = 0; i < transformedPoints_.size(); ++i)
            {
                const Point2D& currentPoint = transformedPoints_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Point_ConvexPolygon_Overlap(
                        currentPoint.position,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other point ----------------------------------------------
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Line_Point_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other line
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Line_Line_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other ray
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Line_Ray_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other lineSegment
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Line_LineSegment_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other rectangle
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Line_Rectangle_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other orientedRectangle
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Line_OrientedRectangle_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherRectangle.halfSize,
                        otherRectangle.position,
                        otherRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other circle
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Line_Circle_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other capsule
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Line_Capsule_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other triangle
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Line_Triangle_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other polygon
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Line_Polygon_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // line & other convexPolygon
            for (size_t i = 0; i < transformedLines_.size(); ++i)
            {
                const Line2D& currentLine = transformedLines_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Line_ConvexPolygon_Overlap(
                        currentLine.position,
                        currentLine.slope,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other point -----------------------------------------------
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Ray_Point_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other line
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Ray_Line_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other ray
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Ray_Ray_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other lineSegment
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Ray_LineSegment_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other rectangle
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Ray_Rectangle_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other orientedRectangle
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherOrientedRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Ray_OrientedRectangle_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherOrientedRectangle.halfSize,
                        otherOrientedRectangle.position,
                        otherOrientedRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other circle
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Ray_Circle_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other capsule
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Ray_Capsule_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other triangle
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Ray_Triangle_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other polygon
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Ray_Polygon_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // ray & other convexPolygon
            for (size_t i = 0; i < transformedRays_.size(); ++i)
            {
                const Ray2D& currentRay = transformedRays_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Ray_ConvexPolygon_Overlap(
                        currentRay.position,
                        currentRay.direction,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other point ---------------------------------------
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_LineSegment_Point_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other line
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_LineSegment_Line_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other ray
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_LineSegment_Ray_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other lineSegment
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_LineSegment_LineSegment_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other rectangle
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_LineSegment_Rectangle_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other orientedRectangle
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_LineSegment_OrientedRectangle_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherRectangle.halfSize,
                        otherRectangle.position,
                        otherRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other circle
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_LineSegment_Circle_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other capsule
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_LineSegment_Capsule_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other triangle
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_LineSegment_Triangle_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other polygon
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_LineSegment_Polygon_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // lineSegment & other convexPolygon
            for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
            {
                const LineSegment2D& currentLineSegment = transformedLineSegments_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_LineSegment_ConvexPolygon_Overlap(
                        currentLineSegment.start,
                        currentLineSegment.end,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other point -----------------------------------------
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Rectangle_Point_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other line
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Rectangle_Line_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other ray
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Rectangle_Ray_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other lineSegment
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Rectangle_LineSegment_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other rectangle
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Rectangle_Rectangle_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other orientedRectangle
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Rectangle_OrientedRectangle_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherRectangle.halfSize,
                        otherRectangle.position,
                        otherRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other circle
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Rectangle_Circle_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other capsule
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Rectangle_Capsule_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other triangle
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Rectangle_Triangle_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other polygon
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Rectangle_Polygon_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // rectangle & other convexPolygon
            for (size_t i = 0; i < transformedRectangles_.size(); ++i)
            {
                const Rectangle2D& currentRectangle = transformedRectangles_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Rectangle_ConvexPolygon_Overlap(
                        currentRectangle.bottomLeft,
                        currentRectangle.topRight,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other point ---------------------------------
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Point_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other line
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Line_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other ray
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Ray_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other lineSegment
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_OrientedRectangle_LineSegment_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other rectangle
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Rectangle_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other orientedRectangle
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_OrientedRectangle_OrientedRectangle_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherRectangle.halfSize,
                        otherRectangle.position,
                        otherRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other circle
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Circle_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other capsule
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Capsule_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other triangle
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_OrientedRectangle_Triangle_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other polygon
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_OrientedRectangle_Polygon_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // orientedRectangle & other convexPolygon
            for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
            {
                const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_OrientedRectangle_ConvexPolygon_Overlap(
                        currentOrientedRectangle.halfSize,
                        currentOrientedRectangle.position,
                        currentOrientedRectangle.rotation,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other point --------------------------------------------
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Circle_Point_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other line
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Circle_Line_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other ray
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Circle_Ray_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other lineSegment
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Circle_LineSegment_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other rectangle
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Circle_Rectangle_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other orientedRectangle
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherOrientedRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Circle_OrientedRectangle_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherOrientedRectangle.halfSize,
                        otherOrientedRectangle.position,
                        otherOrientedRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other circle
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Circle_Circle_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other capsule
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Circle_Capsule_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other triangle
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Circle_Triangle_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other polygon
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Circle_Polygon_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // circle & other convexPolygon
            for (size_t i = 0; i < transformedCircles_.size(); ++i)
            {
                const Circle2D& currentCircle = transformedCircles_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Circle_ConvexPolygon_Overlap(
                        currentCircle.position,
                        currentCircle.radius,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other point -------------------------------------------
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Capsule_Point_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other line
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Capsule_Line_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other ray
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Capsule_Ray_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other lineSegment
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Capsule_LineSegment_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other rectangle
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Capsule_Rectangle_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other orientedRectangle
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherOrientedRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Capsule_OrientedRectangle_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherOrientedRectangle.halfSize,
                        otherOrientedRectangle.position,
                        otherOrientedRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other circle
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Capsule_Circle_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other capsule
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Capsule_Capsule_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other triangle
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Capsule_Triangle_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other polygon
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Capsule_Polygon_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // capsule & other convexPolygon
            for (size_t i = 0; i < transformedCapsules_.size(); ++i)
            {
                const Capsule2D& currentCapsule = transformedCapsules_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Capsule_ConvexPolygon_Overlap(
                        currentCapsule.start,
                        currentCapsule.end,
                        currentCapsule.radius,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other point ------------------------------------------
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Triangle_Point_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other line
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Triangle_Line_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other ray
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Triangle_Ray_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other lineSegment
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Triangle_LineSegment_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other rectangle
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Triangle_Rectangle_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other orientedRectangle
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherOrientedRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Triangle_OrientedRectangle_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherOrientedRectangle.halfSize,
                        otherOrientedRectangle.position,
                        otherOrientedRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other circle
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Triangle_Circle_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other capsule
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Triangle_Capsule_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other triangle
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Triangle_Triangle_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other polygon
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Triangle_Polygon_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // triangle & other convexPolygon
            for (size_t i = 0; i < transformedTriangles_.size(); ++i)
            {
                const Triangle2D& currentTriangle = transformedTriangles_[i];
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Triangle_ConvexPolygon_Overlap(
                        currentTriangle.corner1,
                        currentTriangle.corner2,
                        currentTriangle.corner3,
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other point -------------------------------------------
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_Polygon_Point_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other line
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_Polygon_Line_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other ray
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_Polygon_Ray_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other lineSegment
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_Polygon_LineSegment_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other rectangle
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_Polygon_Rectangle_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other orientedRectangle
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherOrientedRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_Polygon_OrientedRectangle_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherOrientedRectangle.halfSize,
                        otherOrientedRectangle.position,
                        otherOrientedRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other circle
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_Polygon_Circle_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other capsule
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_Polygon_Capsule_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other triangle
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_Polygon_Triangle_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other polygon
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_Polygon_Polygon_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // polygon & other convexPolygon
            for (size_t i = 0; i < transformedPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
                const glm::vec2* currentPolygonData = currentPolygon.data();
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_Polygon_ConvexPolygon_Overlap(
                        currentPolygonData,
                        currentPolygon.size(),
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other point -------------------------------------------
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedPoints_.size(); ++j)
                {
                    const Point2D& otherPoint = other.transformedPoints_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Point_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherPoint.position);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other line
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedLines_.size(); ++j)
                {
                    const Line2D& otherLine = other.transformedLines_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Line_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherLine.position,
                        otherLine.slope);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other ray
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedRays_.size(); ++j)
                {
                    const Ray2D& otherRay = other.transformedRays_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Ray_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherRay.position,
                        otherRay.direction);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other lineSegment
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedLineSegments_.size(); ++j)
                {
                    const LineSegment2D& otherLineSegment = other.transformedLineSegments_[j];

                    bool collisionFound = Check2D_ConvexPolygon_LineSegment_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherLineSegment.start,
                        otherLineSegment.end);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other rectangle
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedRectangles_.size(); ++j)
                {
                    const Rectangle2D& otherRectangle = other.transformedRectangles_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Rectangle_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherRectangle.bottomLeft,
                        otherRectangle.topRight);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other orientedRectangle
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedOrientedRectangles_.size(); ++j)
                {
                    const OrientedRectangle2D& otherOrientedRectangle = other.transformedOrientedRectangles_[j];

                    bool collisionFound = Check2D_ConvexPolygon_OrientedRectangle_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherOrientedRectangle.halfSize,
                        otherOrientedRectangle.position,
                        otherOrientedRectangle.rotation);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other circle
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedCircles_.size(); ++j)
                {
                    const Circle2D& otherCircle = other.transformedCircles_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Circle_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherCircle.position,
                        otherCircle.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other capsule
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedCapsules_.size(); ++j)
                {
                    const Capsule2D& otherCapsule = other.transformedCapsules_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Capsule_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherCapsule.start,
                        otherCapsule.end,
                        otherCapsule.radius);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other triangle
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedTriangles_.size(); ++j)
                {
                    const Triangle2D& otherTriangle = other.transformedTriangles_[j];

                    bool collisionFound = Check2D_ConvexPolygon_Triangle_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherTriangle.corner1,
                        otherTriangle.corner2,
                        otherTriangle.corner3);

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other polygon
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherPolygon = other.transformedPolygons_[j];
                    const glm::vec2* otherPolygonData = otherPolygon.data();

                    bool collisionFound = Check2D_ConvexPolygon_Polygon_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherPolygonData,
                        otherPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }

            // convexPolygon & other convexPolygon
            for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
            {
                const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
                const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();
                for (size_t j = 0; j < other.transformedConvexPolygons_.size(); ++j)
                {
                    const std::vector<glm::vec2>& otherConvexPolygon = other.transformedConvexPolygons_[j];
                    const glm::vec2* otherConvexPolygonData = otherConvexPolygon.data();

                    bool collisionFound = Check2D_ConvexPolygon_ConvexPolygon_Overlap(
                        currentConvexPolygonData,
                        currentConvexPolygon.size(),
                        otherConvexPolygonData,
                        otherConvexPolygon.size());

                    if (collisionFound)
                    {
                        colliding_ = true;
                        other.colliding_ = true;
                        return;
                    }
                }
            }
        }
    }

    bool CollisionBody2D::GetCollision(const Point2D& point)
    {
        // point & other point
        for (size_t i = 0; i < transformedPoints_.size(); ++i)
        {
            const Point2D& currentPoint = transformedPoints_[i];

            bool collisionFound = Check2D_Point_Point_Overlap(
                currentPoint.position,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // line & other point
        for (size_t i = 0; i < transformedLines_.size(); ++i)
        {
            const Line2D& currentLine = transformedLines_[i];

            bool collisionFound = Check2D_Line_Point_Overlap(
                currentLine.position,
                currentLine.slope,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // ray & other point
        for (size_t i = 0; i < transformedRays_.size(); ++i)
        {
            const Ray2D& currentRay = transformedRays_[i];

            bool collisionFound = Check2D_Ray_Point_Overlap(
                currentRay.position,
                currentRay.direction,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // lineSegment & other point
        for (size_t i = 0; i < transformedLineSegments_.size(); ++i)
        {
            const LineSegment2D& currentLineSegment = transformedLineSegments_[i];

            bool collisionFound = Check2D_LineSegment_Point_Overlap(
                currentLineSegment.start,
                currentLineSegment.end,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // rectangle & other point
        for (size_t i = 0; i < transformedRectangles_.size(); ++i)
        {
            const Rectangle2D& currentRectangle = transformedRectangles_[i];

            bool collisionFound = Check2D_Rectangle_Point_Overlap(
                currentRectangle.bottomLeft,
                currentRectangle.topRight,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // orientedRectangle & other point
        for (size_t i = 0; i < transformedOrientedRectangles_.size(); ++i)
        {
            const OrientedRectangle2D& currentOrientedRectangle = transformedOrientedRectangles_[i];

            bool collisionFound = Check2D_OrientedRectangle_Point_Overlap(
                currentOrientedRectangle.halfSize,
                currentOrientedRectangle.position,
                currentOrientedRectangle.rotation,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // circle & other point
        for (size_t i = 0; i < transformedCircles_.size(); ++i)
        {
            const Circle2D& currentCircle = transformedCircles_[i];

            bool collisionFound = Check2D_Circle_Point_Overlap(
                currentCircle.position,
                currentCircle.radius,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // capsule & other point
        for (size_t i = 0; i < transformedCapsules_.size(); ++i)
        {
            const Capsule2D& currentCapsule = transformedCapsules_[i];

            bool collisionFound = Check2D_Capsule_Point_Overlap(
                currentCapsule.start,
                currentCapsule.end,
                currentCapsule.radius,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // triangle & other point
        for (size_t i = 0; i < transformedTriangles_.size(); ++i)
        {
            const Triangle2D& currentTriangle = transformedTriangles_[i];

            bool collisionFound = Check2D_Triangle_Point_Overlap(
                currentTriangle.corner1,
                currentTriangle.corner2,
                currentTriangle.corner3,
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // polygon & other point
        for (size_t i = 0; i < transformedPolygons_.size(); ++i)
        {
            const std::vector<glm::vec2>& currentPolygon = transformedPolygons_[i];
            const glm::vec2* currentPolygonData = currentPolygon.data();

            bool collisionFound = Check2D_Polygon_Point_Overlap(
                currentPolygonData,
                currentPolygon.size(),
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        // convexPolygon & other point
        for (size_t i = 0; i < transformedConvexPolygons_.size(); ++i)
        {
            const std::vector<glm::vec2>& currentConvexPolygon = transformedConvexPolygons_[i];
            const glm::vec2* currentConvexPolygonData = currentConvexPolygon.data();

            bool collisionFound = Check2D_ConvexPolygon_Point_Overlap(
                currentConvexPolygonData,
                currentConvexPolygon.size(),
                point.position);
            if (collisionFound)
            {
                return true;
            }
        }

        return false;
    }
}