#include "MathFunctions.h"

#include "Engine/MathConstants.h"



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
}