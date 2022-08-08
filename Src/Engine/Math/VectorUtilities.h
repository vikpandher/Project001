#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    float Get2DVectorAngle(
        const glm::vec2& vector1,
        const glm::vec2& vector2);

    float Get3DVectorAngle(
        const glm::vec3& vector1,
        const glm::vec3& vector2);

    glm::vec2 Rotate2DVector(
        const glm::vec2& vector,
        float rotationInRadians);

    glm::vec2 Get2DVectorProjection(
        const glm::vec2& vector1,
        const glm::vec2& vector2);

}

#include "VectorUtilities.inl"