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

    // Tests -------------------------------------------------------------------

    void TestGet2DVectorAngle();
}