// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-06

#pragma once

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    // Math Utility Groups:
    // * Coordinate Systems
    // * Fast Square Root
    // * Float Comparisons
    // * Miscellaneous Algorithms
    // * Vector Manipulation

    // Coordinate Systems ------------------------------------------------------

    // Polar Coordinate angle starts and positive x-axis
    // and goes counter clockwise

    glm::vec2 CartesianToPolar(float x, float y);

    glm::vec2 CartesianToPolar(const glm::vec2& cartesian);

    glm::vec2 PolarToCartesian(float r, float t);

    glm::vec2 PolarToCartesian(const glm::vec2& polar);

    // Fast Square Root --------------------------------------------------------

    // Unused
    // Nolonger faster on modern hardware
    float FastInverseSquareRoot(float number);

    // Float Comparisons -------------------------------------------------------

    static const float g_floatMarginOfError = 0.00001f;

    bool FloatEqualToFloat(float a, float b, float epsilon = g_floatMarginOfError);

    bool FloatGreaterThanOrEqualToFloat(float a, float b, float epsilon = g_floatMarginOfError);

    bool FloatLessThanOrEqualToFloat(float a, float b, float epsilon = g_floatMarginOfError);

    void GetMinMax(float a, float b, float& min, float& max);

    // Returns normalized float [0, 1)
    float GetRandomFloat(uint32_t seed);

    // Miscellaneous Algorithms ------------------------------------------------

    bool EarClipPolygon(std::vector<size_t>& indices, const std::vector<glm::vec2>& corners);

    bool EarClipPolygon_v2(std::vector<size_t>& indices, const std::vector<glm::vec2>& corners);

    void OrderPointsCCW(std::vector<glm::vec2>& points, float startAngle);

    // startDir is the start direction from the center of all the points, not the origin
    void OrderPointsCCW(std::vector<glm::vec3>& points, const glm::vec3& axis, const glm::vec3& startDir);

    void RemoveDuplicates(std::vector<glm::vec2>& dst, const std::vector<glm::vec2>& src, float epsilon = g_floatMarginOfError);

    void RemoveDuplicates(std::vector<glm::vec3>& dst, const std::vector<glm::vec3>& src, float epsilon = g_floatMarginOfError);

    // Vector Manipulation -----------------------------------------------------

    // angle from vector1 to vector2 (from -pi to pi)
    // positive result is a counterclockwise rotation
    float Get2DVectorAngle(const glm::vec2& vector1, const glm::vec2& vector2);

    // angle from x-axis to vector in range (-pi, pi]
    // positive result is a counterclockwise rotation
    float Get2DVectorAngle(const glm::vec2& vector);

    // projects vector1 onto vector2
    glm::vec2 Get2DVectorProjection(const glm::vec2& vector1, const glm::vec2& vector2);

    float Get3DVectorAngle(const glm::vec3& vector1, const glm::vec3& vector2);

    glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians);

    // converts the angle to be in range [-pi, pi)
    float WrapAngleToPiRange(float angle);
}