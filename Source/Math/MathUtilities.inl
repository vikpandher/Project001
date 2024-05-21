


namespace Project001
{
    // Coordinate Systems ------------------------------------------------------

    inline glm::vec2 CartesianToPolar(float x, float y)
    {
        glm::vec2 polar;
        float& r = polar.x;
        float& t = polar.y;

        r = glm::sqrt(x * x + y * y);

        if (r == 0.0f)
        {
            t = 0.0f;
        }
        else if (y < 0.0f)
        {
            t = glm::two_pi<float>() - glm::acos(x / r);
        }
        else
        {
            t = glm::acos(x / r);
        }

        return polar;
    }

    inline glm::vec2 CartesianToPolar(const glm::vec2& cartesian)
    {
        return CartesianToPolar(cartesian.x, cartesian.y);
    }

    inline glm::vec2 PolarToCartesian(float r, float t)
    {
        glm::vec2 cartesian;
        float& x = cartesian.x;
        float& y = cartesian.y;

        x = r * glm::cos(t);
        y = r * glm::sin(t);

        return cartesian;
    }

    inline glm::vec2 PolarToCartesian(const glm::vec2& polar)
    {
        return PolarToCartesian(polar.x, polar.y);
    }

    // Fast Square Root --------------------------------------------------------

    // From Quake III Arena
    inline float FastInverseSquareRoot(float number)
    {
        return 1.0f / glm::sqrt(number);

        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *(long*)&y; // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1); // what?
        y = *(float*)&i;
        y = y * (threehalfs - (x2 * y * y)); // 1st iteration
        // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

        return y;
    }

    // Float Comparisons -------------------------------------------------------

    inline bool FloatEqualToFloat(float a, float b, float epsilon)
    {
        return glm::abs(a - b) < epsilon;
    }

    inline bool FloatGreaterThanOrEqualToFloat(float a, float b, float epsilon)
    {
        return (a + epsilon) >= b;
    }

    inline bool FloatLessThanOrEqualToFloat(float a, float b, float epsilon)
    {
        return (a - epsilon) <= b;
    }

    inline void GetMinMax(float a, float b, float& min, float& max)
    {
        if (a > b)
        {
            min = b;
            max = a;
        }
        else
        {
            min = a;
            max = b;
        }
    }

    // Vector Manipulation -----------------------------------------------------

    inline float Get2DVectorAngle(const glm::vec2& vector1, const glm::vec2& vector2)
    {
        float dotProduct = vector1.x * vector2.x + vector1.y * vector2.y;
        float determinant = vector1.x * vector2.y - vector1.y * vector2.x;
        return glm::atan(determinant, dotProduct);
    }

    inline glm::vec2 Get2DVectorProjection(const glm::vec2& vector1, const glm::vec2& vector2)
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
        // |b| = glm::sqrt(dot(b, b))
        // 
        // vectorProjection = scalarProjection * unitVector
        // vectorProjection = b * dot(a, b) / (|b|)^2
        // vectorProjection = b * dot(a, b) / dot(b, b)
        return vector2 * glm::dot(vector1, vector2) / glm::dot(vector2, vector2);
    }

    inline float Get3DVectorAngle(const glm::vec3& vector1, const glm::vec3& vector2)
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
            angle = glm::acos(glm::dot(vector1, vector2) / magnitude12);
        }

        return angle;
    }

    inline glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians)
    {
        float cosAngle = glm::cos(rotationInRadians);
        float sinAngle = glm::sin(rotationInRadians);
        return glm::vec2(cosAngle * vector.x - sinAngle * vector.y, sinAngle * vector.x + cosAngle * vector.y);
    }
}