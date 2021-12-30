#pragma once

#include "glm/gtc/constants.hpp"
#include "glm/glm.hpp"



namespace Project001
{
    // Polar Coordinate angle starts and positive x-axis
    // and goes counter clockwise

    inline glm::vec2 CartesianToPolar(float x, float y)
    {
        glm::vec2 polar;
        float& r = polar.x;
        float& t = polar.y;

        r = std::sqrtf(x * x + y * y);

        if (r == 0.0f)
        {
            t = 0.0f;
        }
        else if (y < 0.0f)
        {
            t = glm::two_pi<float>() - std::acosf(x / r);
        }
        else
        {
            t = std::acosf(x / r);
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

        x = r * std::cosf(t);
        y = r * std::sinf(t);

        return cartesian;
    }

    inline glm::vec2 PolarToCartesian(const glm::vec2& polar)
    {
        return PolarToCartesian(polar.x, polar.y);
    }
}