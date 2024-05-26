#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"



namespace Project001
{
    // Math Utility Groups:
    // * Coordinate Systems
    // * Fast Square Root
    // * Float Comparisons
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

    // Vector Manipulation -----------------------------------------------------

    float Get2DVectorAngle(const glm::vec2& vector1, const glm::vec2& vector2);

    // projects vector1 onto vector2
    glm::vec2 Get2DVectorProjection(const glm::vec2& vector1, const glm::vec2& vector2);

    float Get3DVectorAngle(const glm::vec3& vector1, const glm::vec3& vector2);

    glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians);
}

#include "MathUtilities.inl"