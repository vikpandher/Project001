// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-12

#include "MathUtility.h"

#include "glm/gtc/constants.hpp"

#include <algorithm> // for std::sort



namespace Project001
{
namespace Math
{
    // Coordinate Systems ------------------------------------------------------

    glm::vec2 CartesianToPolar(float x, float y)
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

    glm::vec2 CartesianToPolar(const glm::vec2& cartesian)
    {
        return CartesianToPolar(cartesian.x, cartesian.y);
    }

    glm::vec2 PolarToCartesian(float r, float t)
    {
        glm::vec2 cartesian;
        float& x = cartesian.x;
        float& y = cartesian.y;

        x = r * glm::cos(t);
        y = r * glm::sin(t);

        return cartesian;
    }

    glm::vec2 PolarToCartesian(const glm::vec2& polar)
    {
        return PolarToCartesian(polar.x, polar.y);
    }

    // Fast Square Root --------------------------------------------------------

    // From Quake III Arena
    float FastInverseSquareRoot(float number)
    {
        return 1.0f / glm::sqrt(number);

        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *reinterpret_cast<long*>(&y); // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1); // what?
        y = *reinterpret_cast<float*>(&i);
        y = y * (threehalfs - (x2 * y * y)); // 1st iteration
        // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

        return y;
    }

    // Float Comparisons -------------------------------------------------------

    bool FloatEqualToFloat(float a, float b, float epsilon)
    {
        return glm::abs(a - b) < epsilon;
    }

    bool FloatGreaterThanOrEqualToFloat(float a, float b, float epsilon)
    {
        return (a + epsilon) >= b;
    }

    bool FloatLessThanOrEqualToFloat(float a, float b, float epsilon)
    {
        return (a - epsilon) <= b;
    }

    void GetMinMax(float a, float b, float& min, float& max)
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

    float GetRandomFloat(uint32_t seed)
    {
        // Scramble the seed
        seed ^= seed >> 17;
        seed *= 0xed5ad4bb;
        seed ^= seed >> 11;
        seed *= 0xac4c1b51;
        seed ^= seed >> 15;
        seed *= 0x31848bab;
        seed ^= seed >> 14;

        // mask out with a 24-bit mask and then divide it by 2^24
        return (seed & 0xFFFFFF) / static_cast<float>(0x1000000);
    }

    // Miscellaneous Algorithms ------------------------------------------------

    bool EarClipPolygon(std::vector<size_t>& indices, const std::vector<glm::vec2>& corners)
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
                break; // return false; // No ear found, invalid polygon, stop the loop
            }
        }

        // Add the last remaining triangle
        indices.push_back(polygon[0]);
        indices.push_back(polygon[1]);
        indices.push_back(polygon[2]);

        return true;
    }

    bool EarClipPolygon_v2(std::vector<size_t>& indices, const std::vector<glm::vec2>& corners)
    {
        // Ear Clipping Algorithm v2
        // 
        // This clips the ear with the smallest cut distance first.
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
            std::vector<size_t> earIndexIndices;

            for (size_t i = 0; i < polygon.size(); ++i)
            {
                const size_t& prev = polygon[(i - 1 + polygon.size()) % polygon.size()];
                const size_t& curr = polygon[i];
                const size_t& next = polygon[(i + 1) % polygon.size()];

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
                    earIndexIndices.push_back(i);
                }
            }

            if (earIndexIndices.empty())
            {
                break; // return false; // No ear found, invalid polygon, stop the loop
            }

            float minCutLength = std::numeric_limits<float>::infinity();
            size_t minEarIndex = static_cast<size_t>(-1);

            for (size_t i = 0; i < earIndexIndices.size(); ++i)
            {
                const size_t earIndex = earIndexIndices[i];

                const size_t& prev = polygon[(earIndex - 1 + polygon.size()) % polygon.size()];
                const size_t& curr = polygon[earIndex];
                const size_t& next = polygon[(earIndex + 1) % polygon.size()];

                const glm::vec2& p0 = corners[prev];
                const glm::vec2& p1 = corners[curr];
                const glm::vec2& p2 = corners[next];

                float cutLength = glm::length(p2 - p0);
                if (cutLength < minCutLength)
                {
                    minCutLength = cutLength;
                    minEarIndex = earIndex;
                }
            }

            const size_t& prev = polygon[(minEarIndex - 1 + polygon.size()) % polygon.size()];
            const size_t& curr = polygon[minEarIndex];
            const size_t& next = polygon[(minEarIndex + 1) % polygon.size()];

            // Add the ear triangle
            indices.push_back(prev);
            indices.push_back(curr);
            indices.push_back(next);

            // Remove the ear corner
            polygon.erase(polygon.begin() + minEarIndex);
        }

        // Add the last remaining triangle
        indices.push_back(polygon[0]);
        indices.push_back(polygon[1]);
        indices.push_back(polygon[2]);

        return true;
    }

    void OrderPointsCCW(std::vector<glm::vec2>& points, float startAngle)
    {
        if (points.empty())
        {
            return;
        }

        glm::vec2 center(0.0f, 0.0f);
        for (size_t i = 0; i < points.size(); ++i)
        {
            center += points[i];
        }
        center /= static_cast<float>(points.size());

        struct PointAngle
        {
            glm::vec2 point;
            float angle;
        };

        std::vector<PointAngle> pointAngles;
        pointAngles.reserve(points.size());

        for (size_t i = 0; i < points.size(); ++i)
        {
            glm::vec2 dir = points[i] - center;
            float angle = glm::atan(dir.y, dir.x); // CCW from startAngle, (-pi, pi]
            if (angle < 0.0f)
            {
                angle += glm::two_pi<float>();
            }
            angle = glm::mod(angle - startAngle, glm::two_pi<float>());
            pointAngles.push_back({ points[i], angle });
        }

        std::sort(
            pointAngles.begin(), pointAngles.end(),
            [](const PointAngle& a, const PointAngle& b)
            {
                return a.angle < b.angle;
            }
        );

        for (size_t i = 0; i < points.size(); ++i)
        {
            points[i] = pointAngles[i].point;
        }
    }

    void OrderPointsCCW(std::vector<glm::vec3>& points, const glm::vec3& axis, const glm::vec3& startDir)
    {
        if (points.empty())
        {
            return;
        }

        glm::vec3 center(0.0f, 0.0f, 0.0f);
        for (size_t i = 0; i < points.size(); ++i)
        {
            center += points[i];
        }
        center /= static_cast<float>(points.size());

        glm::vec3 normal = glm::normalize(axis);
        glm::vec3 right = glm::normalize(startDir - normal * glm::dot(startDir, normal)); // project startDir onto the plane
        glm::vec3 up = glm::normalize(glm::cross(normal, right)); // 90 degrees CCW from right

        struct PointAngle
        {
            glm::vec3 point;
            float angle;
        };

        std::vector<PointAngle> pointAngles;
        pointAngles.reserve(points.size());

        for (size_t i = 0; i < points.size(); ++i)
        {
            glm::vec3 v = points[i] - center;
            float x = glm::dot(v, right);
            float y = glm::dot(v, up);
            float angle = glm::atan(y, x); // CCW from startDir, (-pi, pi]
            if (angle < 0.0f)
            {
                angle += glm::two_pi<float>();
            }
            pointAngles.push_back({ points[i], angle});
        }

        std::sort(
            pointAngles.begin(), pointAngles.end(),
            [](const PointAngle& a, const PointAngle& b)
            {
                return a.angle < b.angle;
            }
        );

        for (size_t i = 0; i < points.size(); ++i)
        {
            points[i] = pointAngles[i].point;
        }
    }

    void RemoveDuplicates(std::vector<glm::vec2>& dst, const std::vector<glm::vec2>& src, float epsilon)
    {
        // O(n^2)

        size_t oldDstSize = dst.size();

        for (size_t i = 0; i < src.size(); ++i)
        {
            const glm::vec2& current = src[i];

            bool duplicate = false;
            for (size_t j = oldDstSize; j < dst.size(); ++j)
            {
                const glm::vec2& other = dst[j];

                if (FloatEqualToFloat(current.x, other.x) &&
                    FloatEqualToFloat(current.y, other.y))
                {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate)
            {
                dst.push_back(current);
            }
        }
    }

    void RemoveDuplicates(std::vector<glm::vec3>& dst, const std::vector<glm::vec3>& src, float epsilon)
    {
        // O(n^2)

        size_t oldDstSize = dst.size();

        for (size_t i = 0; i < src.size(); ++i)
        {
            const glm::vec3& current = src[i];

            bool duplicate = false;
            for (size_t j = oldDstSize; j < dst.size(); ++j)
            {
                const glm::vec3& other = dst[j];

                if (FloatEqualToFloat(current.x, other.x) &&
                    FloatEqualToFloat(current.y, other.y) &&
                    FloatEqualToFloat(current.z, other.z))
                {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate)
            {
                dst.push_back(current);
            }
        }
    }

    float GetCircleRaidusGivenSphereOnGroundAndHorizontalPlaneIntersection(float sphereRadius, float horizontalPlaneHeight)
    {
        // plane must intersect the sphere: 0 <= H <= 2R
        // clamp to avoid negative values due to floating-point error
        // r = circle radius
        // R = sphere radius
        // H = height
        // r^2 = 2*R*H - H^2
        float value = 2.0f * sphereRadius * horizontalPlaneHeight - horizontalPlaneHeight * horizontalPlaneHeight;
        return glm::sqrt(std::max(0.0f, value));
    }

    float GetSphereRadiusGivenHorizontalCircleRadiusAtGivenHeight(float circleRadius, float height)
    {
        // if circleRadius < height :  intersection on bottom half of sphere
        // if circleRadius == height : intersection on center of sphere
        // if circleRadius > height :  intersection on top half of sphere
        if (height <= 0.0f)
        {
            return 0.0f;
        }
        // r = circle radius
        // R = sphere radius
        // H = height
        // R = (r^2 + H^2) / (2*H)
        return (circleRadius * circleRadius + height * height) / (2.0f * height);
    }

    // Vector Manipulation -----------------------------------------------------

    float Get2DVectorAngle(const glm::vec2& vector1, const glm::vec2& vector2)
    {
        float dotProduct = vector1.x * vector2.x + vector1.y * vector2.y;
        float determinant = vector1.x * vector2.y - vector1.y * vector2.x;
        return glm::atan(determinant, dotProduct);
    }

    float Get2DVectorAngle(const glm::vec2& vector)
    {
        return glm::atan(vector.y, vector.x);
    }

    glm::vec2 Get2DVectorProjection(const glm::vec2& vector1, const glm::vec2& vector2)
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

    float Get3DVectorAngle(const glm::vec3& vector1, const glm::vec3& vector2)
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

    glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians)
    {
        float cosAngle = glm::cos(rotationInRadians);
        float sinAngle = glm::sin(rotationInRadians);
        return glm::vec2(cosAngle * vector.x - sinAngle * vector.y, sinAngle * vector.x + cosAngle * vector.y);
    }

    float WrapAngleToPiRange(float angle)
    {
        float wrapped = glm::mod(angle + glm::pi<float>(), glm::two_pi<float>());
        return wrapped - glm::pi<float>();
    }
}
}