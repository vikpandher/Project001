#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    // Polar Coordinate angle starts and positive x-axis
    // and goes counter clockwise

    glm::vec2 CartesianToPolar(const glm::vec2& cartesian);
    glm::vec2 CartesianToPolar(float x, float y);

    glm::vec2 PolarToCartesian(const glm::vec2& polar);
    glm::vec2 PolarToCartesian(float r, float t);

    // Tests -------------------------------------------------------------------

    void TestCartesianToPolar();
    void TestPolarToCartesian();
}