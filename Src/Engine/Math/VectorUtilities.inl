


namespace Project001
{
    inline float Get2DVectorAngle(
        const glm::vec2& vector1,
        const glm::vec2& vector2)
    {
        float dotProduct = vector1.x * vector2.x + vector1.y * vector2.y;
        float determinant = vector1.x * vector2.y - vector1.y * vector2.x;
        return std::atan2f(determinant, dotProduct);
    }

    inline float Get3DVectorAngle(
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

    inline glm::vec2 Rotate2DVector(
        const glm::vec2& vector,
        float rotationInRadians)
    {
        float cosAngle = std::cosf(rotationInRadians);
        float sinAngle = std::sinf(rotationInRadians);
        return glm::vec2(cosAngle * vector.x - sinAngle * vector.y, sinAngle * vector.x + cosAngle * vector.y);
    }

    inline glm::vec2 Get2DVectorProjection(
        const glm::vec2& vector1,
        const glm::vec2& vector2)
    {
        // dot product formula
        // dot(a, b) = |a| * |b| * cos(theta) = a.x * b.x + a.y * b.y
        // 
        // scalar projection calculation
        // cos(theta) = scalarProjection / |a|
        // scalarProjection = cos(theta) * |a|
        // 
        // scalarProjection = dot(a, b) / |b|
        // 
        // unitVector = b / |b|
        // |b| = sqrt(dot(b, b))
        // 
        // vectorProjection = scalarProjection * unitVector
        // vectorProjection = b * dot(a, b) / (|b|)^2
        // vectorProjection = b * dot(a, b) / dot(b, b)
        return vector2 * glm::dot(vector1, vector2) / glm::dot(vector2, vector2);
    }
}