#include "VectorAngles.h"



namespace Project001
{
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

    glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians)
    {
        float cosAngle = std::cosf(rotationInRadians);
        float sinAngle = std::sinf(rotationInRadians);
        return glm::vec2(cosAngle * vector.x - sinAngle * vector.y, sinAngle * vector.x + cosAngle * vector.y);
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
}