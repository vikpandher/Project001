#pragma once

#include "Engine/MathConstants.h"

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

    float Get3DVectorAngle(
        const glm::vec3& vector1,
        const glm::vec3& vector2);

    // Tests -------------------------------------------------------------------

    void TestGet2DVectorAngle();
}