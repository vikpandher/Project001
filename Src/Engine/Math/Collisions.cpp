#include "Collisions.h"

#include "Engine/Math/VectorAngles.h"



namespace Project001
{
    // Point Interactions ------------------------------------------------------

    bool PointPointIntersection2d(
        const glm::vec2& pointA,
        const glm::vec2& pointB,
        float marginOfError)
    {
        return pointA.x + marginOfError >= pointB.x &&
            pointA.x - marginOfError <= pointB.x &&
            pointA.y + marginOfError >= pointB.y &&
            pointA.y - marginOfError <= pointB.y;
    }

    bool PointLineIntersection2d(
        const glm::vec2& point,
        const glm::vec2& linePoint,
        float slope,
        float marginOfError)
    {
        // y = slope * x + yIntercept
        // yIntercept = y - slope * x

        float yIntercept = linePoint.y - slope * linePoint.x;

        float calculatedPointY = slope * point.x + yIntercept;

        return calculatedPointY + marginOfError >= point.y &&
            calculatedPointY - marginOfError <= point.y;
    }

    bool PointLineIntersection2d(
        const glm::vec2& point,
        const glm::vec2& lineStart,
        const glm::vec2& lineEnd,
        float marginOfError)
    {
        // y = slope * x + yIntercept
        // yIntercept = y - slope * x

        float slope = (lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x);
        float yIntercept = lineStart.y - slope * lineStart.x;

        float calculatedPointY = slope * point.x + yIntercept;

        return calculatedPointY + marginOfError >= point.y &&
            calculatedPointY - marginOfError <= point.y;
    }

    bool PointLineSegmentIntersection2d(
        const glm::vec2& point,
        const glm::vec2& lineStart,
        const glm::vec2& lineEnd,
        float marginOfError)
    {
        float xMax = lineStart.x;
        float xMin = lineEnd.x;
        if (lineStart.x < lineEnd.x)
        {
            xMax = lineEnd.x;
            xMin = lineStart.x;
        }

        float yMax = lineStart.y;
        float yMin = lineEnd.y;
        if (lineStart.y < lineEnd.y)
        {
            yMax = lineEnd.y;
            yMin = lineStart.y;
        }

        bool pointInRange = point.x + marginOfError >= xMin &&
            point.x - marginOfError <= xMax &&
            point.y + marginOfError >= yMin &&
            point.y - marginOfError <= yMax;

        if (!pointInRange)
        {
            return false;
        }

        float slope = (lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x);
        float yIntercept = lineStart.y - slope * lineStart.x;

        float calculatedPointY = slope * point.x + yIntercept;

        bool pointOnLine = calculatedPointY + marginOfError >= point.y &&
            calculatedPointY - marginOfError <= point.y;

        return pointOnLine;
    }

    bool PointFixedRectangleIntersection2d(
        const glm::vec2& point,
        const glm::vec2& rectangleCornerA,
        const glm::vec2& rectangleCornerB,
        float marginOfError)
    {
        float xMax = rectangleCornerA.x;
        float xMin = rectangleCornerB.x;
        if (rectangleCornerA.x < rectangleCornerB.x)
        {
            xMax = rectangleCornerB.x;
            xMin = rectangleCornerA.x;
        }

        float yMax = rectangleCornerA.y;
        float yMin = rectangleCornerB.y;
        if (rectangleCornerA.y < rectangleCornerB.y)
        {
            yMax = rectangleCornerB.y;
            yMin = rectangleCornerA.y;
        }

        return point.x + marginOfError >= xMin &&
            point.x - marginOfError <= xMax &&
            point.y + marginOfError >= yMin &&
            point.y - marginOfError <= yMax;
    }

    bool PointRectangleIntersection2d(
        const glm::vec2& point,
        const glm::vec2& rectangleCenter,
        const glm::vec2& rectangleDimensions,
        float rectangleRotation,
        float marginOfError)
    {
        glm::vec2 translatedPoint = point - rectangleCenter;

        glm::vec2 rotatedPoint = Rotate2DVector(translatedPoint, -1.0f * rectangleRotation);

        float xMin = -0.5f * rectangleDimensions.x;
        float yMin = -0.5f * rectangleDimensions.y;
        float xMax = 0.5f * rectangleDimensions.x;
        float yMax = 0.5f * rectangleDimensions.y;

        return rotatedPoint.x + marginOfError >= xMin &&
            rotatedPoint.x - marginOfError <= xMax &&
            rotatedPoint.y + marginOfError >= yMin &&
            rotatedPoint.y - marginOfError <= yMax;;
    }

    // Line Interactions -------------------------------------------------------

    glm::vec2 GetLineLineIntersection2d(
        const glm::vec2& point1,
        const float& slope1,
        const glm::vec2& point2,
        const float& slope2)
    {
        glm::vec2 intersectionPoint;

        if (slope1 == slope2)
        {
            intersectionPoint.x = NAN;
            intersectionPoint.y = NAN;
        }
        else if (slope1 == INFINITY || slope1 == -INFINITY)
        {
            // x = x1
            // y = m2 * (x - x2) + y2
            // y = m2 * (x1 - x2) + y2
            intersectionPoint.x = point1.x;
            intersectionPoint.y = slope2 * (point1.x - point2.x) + point2.y;
        }
        else if (slope2 == INFINITY || slope2 == -INFINITY)
        {
            // y = m1 * (x - x1) + y1
            // x = x2
            // y = m1 * (x2 - x1) + y1
            intersectionPoint.x = point2.x;
            intersectionPoint.y = slope1 * (point2.x - point1.x) + point1.y;
        }
        else if (slope1 != 0.0f)
        {
            // x = (1 / m1) * (y - y1) + x1
            // y = m2 * (x - x2) + y2
            // y = m2 * ((1 / m1) * (y - y1) + x1 - x2) + y2
            // y = (m2 * m1 * x2 - m2 * m1 * x1 + m2 * y1 - m1 * y2) / (m2 - m1)
            intersectionPoint.y = (slope2 * slope1 * point2.x - slope2 * slope1 * point1.x + slope2 * point1.y - slope1 * point2.y) / (slope2 - slope1);
            intersectionPoint.x = (1 / slope1) * (intersectionPoint.y - point1.y) + point1.x;
        }
        else if (slope2 != 0.0f)
        {
            // y = m1 * (x - x1) + y1
            // x = (1 / m2) * (y - y2) + x2
            // y = m1 * ((1 / m2) * (y - y2) + x2 - x1) + y1
            // y = (m1 * m2 * x1 - m1 * m2 * x2 + m1 * y2 - m2 * y1) / (m1 - m2)
            intersectionPoint.y = (slope1 * slope2 * point1.x - slope1 * slope2 * point2.x + slope1 * point2.y - slope2 * point1.y) / (slope1 - slope2);
            intersectionPoint.x = (1 / slope2) * (intersectionPoint.y - point2.y) + point2.x;
        }

        return intersectionPoint;
    }

    // Tests -------------------------------------------------------------------

    void TestPointLineIntersection2d()
    {
        glm::vec2 point_000_000(0.0f, 0.0f);
        glm::vec2 point_049_050(0.49f, 0.5f);
        glm::vec2 point_050_050(0.5f, 0.5f);
        glm::vec2 point_051_050(0.51f, 0.5f);
        glm::vec2 point_100_100(1.0f, 1.0f);
        glm::vec2 point_150_150(1.0f, 1.0f);

        bool result01 = PointLineIntersection2d(point_050_050, point_000_000, 1.0f);
        bool result02 = PointLineIntersection2d(point_049_050, point_000_000, 1.0f);
        bool result03 = PointLineIntersection2d(point_051_050, point_000_000, 1.0f);
        bool result04 = PointLineIntersection2d(point_050_050, point_000_000, point_100_100);
        bool result05 = PointLineIntersection2d(point_049_050, point_000_000, point_100_100);
        bool result06 = PointLineIntersection2d(point_051_050, point_000_000, point_100_100);
        bool result07 = PointLineIntersection2d(point_150_150, point_000_000, point_100_100);
    }

    void TestPointFixedRectangleIntersection2d()
    {
        glm::vec2 bottomLeftCorner(-1.0f, -1.0f);
        glm::vec2 topRightCorner(1.0f, 1.0f);

        bool result01 = PointFixedRectangleIntersection2d(
            glm::vec2(0.0f, 0.0f),
            bottomLeftCorner,
            topRightCorner
        );

        bool result02 = PointFixedRectangleIntersection2d(
            glm::vec2(1.5f, 0.0f),
            bottomLeftCorner,
            topRightCorner
        );

        bool result03 = PointFixedRectangleIntersection2d(
            glm::vec2(0.0f, 1.5f),
            bottomLeftCorner,
            topRightCorner
        );

        bool result04 = PointFixedRectangleIntersection2d(
            glm::vec2(-1.5f, 0.0f),
            bottomLeftCorner,
            topRightCorner
        );

        bool result05 = PointFixedRectangleIntersection2d(
            glm::vec2(0.0f, -1.5f),
            bottomLeftCorner,
            topRightCorner
        );

        bool result06 = PointFixedRectangleIntersection2d(
            topRightCorner,
            bottomLeftCorner,
            topRightCorner
        );
    }

    void TestPointRectangleIntersection2d()
    {
        glm::vec2 rectangleDimensions(2.0f, 1.0f);

        bool result01 = PointRectangleIntersection2d(
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            rectangleDimensions,
            0.0f
        );
    }
}