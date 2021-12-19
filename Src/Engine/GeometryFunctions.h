#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    glm::vec2 Get2DLineIntersection(
        const glm::vec2& point1,
        const float& slope1,
        const glm::vec2& point2,
        const float& slope2);

    float Get2DVectorAngle(
        const glm::vec2& vector1,
        const glm::vec2& vector2);

    glm::vec2 Rotate2DVector(const glm::vec2& vector, float rotationInRadians);

    // Unused
    float Get3DVectorAngle(
        const glm::vec3& vector1,
        const glm::vec3& vector2);

    // Unused
    unsigned int FindLineSegmentCircleIntersections(
        glm::vec2& intersection1, glm::vec2& intersection2,
        const glm::vec2& circleCenter, float circleRadius,
        const glm::vec2& lineStart, const glm::vec2& lineEnd);

    // Polar Coordinate angle starts and positive x-axis
    // and goes counter clockwise

    glm::vec2 CartesianToPolar(const glm::vec2& cartesian);
    glm::vec2 CartesianToPolar(float x, float y);

    glm::vec2 PolarToCartesian(const glm::vec2& polar);
    glm::vec2 PolarToCartesian(float r, float t);

    // Tests -------------------------------------------------------------------

    void TestGet2DVectorAngle();

    void TestCartesianToPolar();

    void TestPolarToCartesian();
}