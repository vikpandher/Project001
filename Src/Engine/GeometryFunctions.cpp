#include "GeometryFunctions.h"

#include "glm/gtc/constants.hpp"



namespace Project001
{
    glm::vec2 Get2DLineIntersection(
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

    float Get2DVectorAngle(
        const glm::vec2& vector1,
        const glm::vec2& vector2)
    {
        float dotProduct = vector1.x * vector2.x + vector1.y * vector2.y;
        float determinant = vector1.x * vector2.y - vector1.y * vector2.x;
        return std::atan2f(determinant, dotProduct);
    }

    glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians)
    {
        float cosAngle = std::cosf(rotationInRadians);
        float sinAngle = std::sinf(rotationInRadians);
        return glm::vec2(cosAngle * vector.x - sinAngle * vector.y, sinAngle * vector.x + cosAngle * vector.y);
    }

    float Get3DVectorAngle(
        const glm::vec3& vector1,
        const glm::vec3& vector2)
    {
        // cos(A) = (v1 dot v2) / (|v1| * |v2|)
        // A = arccos((v1 dot v2) / (|v1| * |v2|))

        float angle;

        float magnitude1 = glm::length(vector1);
        float magnitude2 = glm::length(vector2);
        float magnitude12 = magnitude1 * magnitude2;
        if (magnitude12 == 0.0f)
        {
            angle = NAN;
        }
        else
        {
            angle = std::acosf(glm::dot(vector1, vector2) / magnitude12);
        }

        return angle;
    }

    unsigned int FindLineSegmentCircleIntersections(
        glm::vec2& intersection1, glm::vec2& intersection2,
        const glm::vec2& circleCenter, float circleRadius,
        const glm::vec2& lineStart, const glm::vec2& lineEnd)
    {
        glm::vec2 lineStartToEnd = lineEnd - lineStart;
        glm::vec2 circleCenterToLineStart = lineStart - circleCenter;

        float a = lineStartToEnd.x * lineStartToEnd.x + lineStartToEnd.y * lineStartToEnd.y;
        float b = 2.0f * (lineStartToEnd.x * circleCenterToLineStart.x + lineStartToEnd.y * circleCenterToLineStart.y);
        float c = circleCenterToLineStart.x * circleCenterToLineStart.x + circleCenterToLineStart.y * circleCenterToLineStart.y - circleRadius * circleRadius;

        float d = b * b - 4 * a * c;

        if (a < 0.0000001f || d < 0.0f)
        {
            // no intersections
            return 0;
        }
        else
        {
            float t1 = (-1.0f * b - std::sqrtf(d)) / (2.0f * a);
            float t2 = (-1.0f * b + std::sqrtf(d)) / (2.0f * a);

            // Impale: --(---)->
            if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 <= 1.0f)
            {
                intersection1.x = lineStart.x + t1 * lineStartToEnd.x;
                intersection1.y = lineStart.y + t1 * lineStartToEnd.y;

                intersection2.x = lineStart.x + t2 * lineStartToEnd.x;
                intersection2.y = lineStart.y + t2 * lineStartToEnd.y;

                return 2;
            }

            // Stab: --(-> )
            if (t1 >= 0.0f && t1 <= 1.0f && t2 > 1.0f)
            {
                intersection1.x = lineStart.x + t1 * lineStartToEnd.x;
                intersection1.y = lineStart.y + t1 * lineStartToEnd.y;

                return 1;
            }

            // Burst: ( --)->
            if (t1 < 0.0f && t2 >= 0.0f && t2 <= 1.0f)
            {
                intersection1.x = lineStart.x + t2 * lineStartToEnd.x;
                intersection1.y = lineStart.y + t2 * lineStartToEnd.y;

                return 1;
            }

            // Short: ---> (   )
            if (t1 > 1.0f && t2 > 1.0f)
            {
                return 0;
            }

            // Past: (   ) --->
            if (t1 < 0.0f && t2 < 0.0f)
            {
                return 0;
            }

            // Inside: ( ---> )
            if (t1 < 0.0f && t2 > 1.0f)
            {
                return 0;
            }

            // Should never reach here
            return 0;
        }
    }

    glm::vec2 CartesianToPolar(const glm::vec2& cartesian)
    {
        return CartesianToPolar(cartesian.x, cartesian.y);
    }

    glm::vec2 CartesianToPolar(float x, float y)
    {
        glm::vec2 polar;
        float& r = polar.x;
        float& t = polar.y;

        r = std::sqrtf(x * x + y * y);

        if (r == 0.0f)
        {
            t = 0.0f;
        }
        else if (y < 0.0f)
        {
            t = glm::two_pi<float>() - std::acosf(x / r);
        }
        else
        {
            t = std::acosf(x / r);
        }

        return polar;
    }

    glm::vec2 PolarToCartesian(const glm::vec2& polar)
    {
        return PolarToCartesian(polar.x, polar.y);
    }

    glm::vec2 PolarToCartesian(float r, float t)
    {
        glm::vec2 cartesian;
        float& x = cartesian.x;
        float& y = cartesian.y;

        x = r * std::cosf(t);
        y = r * std::sinf(t);

        return cartesian;
    }

    // Tests -------------------------------------------------------------------

    void TestGet2DVectorAngle()
    {
        glm::vec2 vector00(0.0f, 0.0f);
        glm::vec2 vector01(1.0f, 0.0f);
        glm::vec2 vector02(1.0f, 1.0f);
        glm::vec2 vector03(0.0f, 1.0f);
        glm::vec2 vector04(-1.0f, 1.0f);
        glm::vec2 vector05(-1.0f, 0.0f);
        glm::vec2 vector06(-1.0f, -1.0f);
        glm::vec2 vector07(0.0f, -1.0f);
        glm::vec2 vector08(1.0f, -1.0f);
        float angle0000 = Get2DVectorAngle(vector00, vector00);
        float angle0001 = Get2DVectorAngle(vector00, vector01);
        float angle0003 = Get2DVectorAngle(vector00, vector03);
        float angle0101 = Get2DVectorAngle(vector01, vector01);
        float angle0102 = Get2DVectorAngle(vector01, vector02);
        float angle0201 = Get2DVectorAngle(vector02, vector01);
        float angle0103 = Get2DVectorAngle(vector01, vector03);
        float angle0104 = Get2DVectorAngle(vector01, vector04);
        float angle0105 = Get2DVectorAngle(vector01, vector05);
        float angle0106 = Get2DVectorAngle(vector01, vector06);
        float angle0107 = Get2DVectorAngle(vector01, vector07);
        float angle0108 = Get2DVectorAngle(vector01, vector08);
    }

    void TestCartesianToPolar()
    {
        glm::vec2 input00(0.0f, 0.0f);
        glm::vec2 output00 = CartesianToPolar(input00);

        glm::vec2 input01(1.0f, 0.0f);
        glm::vec2 output01 = CartesianToPolar(input01);

        glm::vec2 input02(1.0f, 1.0f);
        glm::vec2 output02 = CartesianToPolar(input02);

        glm::vec2 input03(0.0f, 1.0f);
        glm::vec2 output03 = CartesianToPolar(input03);

        glm::vec2 input04(-1.0f, 1.0f);
        glm::vec2 output04 = CartesianToPolar(input04);

        glm::vec2 input05(-1.0f, 0.0f);
        glm::vec2 output05 = CartesianToPolar(input05);

        glm::vec2 input06(-1.0f, -1.0f);
        glm::vec2 output06 = CartesianToPolar(input06);

        glm::vec2 input07(0.0f, -1.0f);
        glm::vec2 output07 = CartesianToPolar(input07);

        glm::vec2 input08(1.0f, -1.0f);
        glm::vec2 output08 = CartesianToPolar(input08);
    }

    void TestPolarToCartesian()
    {
        glm::vec2 input00(0.0f, 0.0f);
        glm::vec2 output00 = PolarToCartesian(input00);

        glm::vec2 input01(1.0f, 0.0f);
        glm::vec2 output01 = PolarToCartesian(input01);

        glm::vec2 input02(1.0f, glm::quarter_pi<float>());
        glm::vec2 output02 = PolarToCartesian(input02);

        glm::vec2 input03(1.0f, glm::half_pi<float>());
        glm::vec2 output03 = PolarToCartesian(input03);

        glm::vec2 input04(1.0f, glm::half_pi<float>() + glm::quarter_pi<float>());
        glm::vec2 output04 = PolarToCartesian(input04);

        glm::vec2 input05(1.0f, glm::pi<float>());
        glm::vec2 output05 = PolarToCartesian(input05);

        glm::vec2 input06(1.0f, glm::pi<float>() + glm::quarter_pi<float>());
        glm::vec2 output06 = PolarToCartesian(input06);

        glm::vec2 input07(1.0f, glm::pi<float>() + glm::half_pi<float>());
        glm::vec2 output07 = PolarToCartesian(input07);

        glm::vec2 input08(1.0f, glm::pi<float>() + glm::half_pi<float>() + glm::quarter_pi<float>());
        glm::vec2 output08 = PolarToCartesian(input08);
    }
}