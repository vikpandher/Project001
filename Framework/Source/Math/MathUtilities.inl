// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-01-31



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

    // Miscellaneous Algorithms ------------------------------------------------

    inline bool EarClipPolygon(std::vector<size_t>& indices, const std::vector<glm::vec2>& corners)
    {
        // Ear Clipping Algorithm
        // 
        // Valid polygons must have their corners ordered counter-clockwise.
        // 
        // Indices will represent tirangles with their corners ordered
        // counter-clockwise.

        if (corners.size() < 3)
        {
            return false;
        }

        std::vector<size_t> polygon(corners.size());
        for (size_t i = 0; i < corners.size(); ++i)
        {
            polygon[i] = i;
        }

        while (polygon.size() > 3)
        {
            bool earFound = false;

            for (size_t i = 0; i < polygon.size(); ++i)
            {
                size_t prev = polygon[(i - 1 + polygon.size()) % polygon.size()];
                size_t curr = polygon[i];
                size_t next = polygon[(i + 1) % polygon.size()];

                const glm::vec2& p0 = corners[prev];
                const glm::vec2& p1 = corners[curr];
                const glm::vec2& p2 = corners[next];

                // check if the triangle is counter-clockwise
                float cross = (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
                if (cross <= 0)
                {
                    continue; // not counter-clockwise
                }

                // check if any other corner is inside the triangle
                bool isEar = true;

                for (size_t j = 0; j < polygon.size(); ++j)
                {
                    if (j == (i - 1 + polygon.size()) % polygon.size() ||
                        j == i ||
                        j == (i + 1) % polygon.size())
                    {
                        continue;
                    }

                    const glm::vec2& pt = corners[polygon[j]];

                    // Barycentric test for point inside the triangle
                    float alpha = ((p1.y - p2.y) * (pt.x - p2.x) + (p2.x - p1.x) * (pt.y - p2.y)) /
                        ((p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y));
                    float beta = ((p2.y - p0.y) * (pt.x - p2.x) + (p0.x - p2.x) * (pt.y - p2.y)) /
                        ((p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y));
                    float gamma = 1.0f - alpha - beta;

                    if (alpha > 0 && beta > 0 && gamma > 0)
                    {
                        isEar = false;
                        break; // corner is inside the triangle
                    }
                }

                if (isEar)
                {
                    // Add the ear triangle
                    indices.push_back(prev);
                    indices.push_back(curr);
                    indices.push_back(next);

                    // Remove the ear corner
                    polygon.erase(polygon.begin() + i);
                    earFound = true;
                    break;
                }
            }

            if (!earFound)
            {
                return false; // No ear found, invalid polygon
            }
        }

        // Add the last remaining triangle
        indices.push_back(polygon[0]);
        indices.push_back(polygon[1]);
        indices.push_back(polygon[2]);

        return true;
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

        float dotProduct = glm::dot(vector1, vector2);
        float magnitudeSquared = glm::dot(vector2, vector2);

        // Prevent division by zero
        // if (magnitudeSquared == 0.0f) {
        //     return glm::vec2(0.0f, 0.0f);
        // }

        return vector2 * dotProduct / magnitudeSquared;
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