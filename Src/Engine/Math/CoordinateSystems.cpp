#include "CoordinateSystems.h"

#include "glm/gtc/constants.hpp"



namespace Project001
{
    glm::vec2 CartesianToPolar(const glm::vec2& cartesian)
    {
        return CartesianToPolar(cartesian.x, cartesian.y);
    }

    glm::vec2 CartesianToPolar(float x, float y)
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

    glm::vec2 PolarToCartesian(const glm::vec2& polar)
    {
        return PolarToCartesian(polar.x, polar.y);
    }

    glm::vec2 PolarToCartesian(float r, float t)
    {
        glm::vec2 cartesian;
        float& x = cartesian.x;
        float& y = cartesian.y;

        x = r * std::cosf(t);
        y = r * std::sinf(t);

        return cartesian;
    }

    // Tests -------------------------------------------------------------------

    void TestCartesianToPolar()
    {
        glm::vec2 input00(0.0f, 0.0f);
        glm::vec2 output00 = CartesianToPolar(input00);

        glm::vec2 input01(1.0f, 0.0f);
        glm::vec2 output01 = CartesianToPolar(input01);

        glm::vec2 input02(1.0f, 1.0f);
        glm::vec2 output02 = CartesianToPolar(input02);

        glm::vec2 input03(0.0f, 1.0f);
        glm::vec2 output03 = CartesianToPolar(input03);

        glm::vec2 input04(-1.0f, 1.0f);
        glm::vec2 output04 = CartesianToPolar(input04);

        glm::vec2 input05(-1.0f, 0.0f);
        glm::vec2 output05 = CartesianToPolar(input05);

        glm::vec2 input06(-1.0f, -1.0f);
        glm::vec2 output06 = CartesianToPolar(input06);

        glm::vec2 input07(0.0f, -1.0f);
        glm::vec2 output07 = CartesianToPolar(input07);

        glm::vec2 input08(1.0f, -1.0f);
        glm::vec2 output08 = CartesianToPolar(input08);
    }

    void TestPolarToCartesian()
    {
        glm::vec2 input00(0.0f, 0.0f);
        glm::vec2 output00 = PolarToCartesian(input00);

        glm::vec2 input01(1.0f, 0.0f);
        glm::vec2 output01 = PolarToCartesian(input01);

        glm::vec2 input02(1.0f, glm::quarter_pi<float>());
        glm::vec2 output02 = PolarToCartesian(input02);

        glm::vec2 input03(1.0f, glm::half_pi<float>());
        glm::vec2 output03 = PolarToCartesian(input03);

        glm::vec2 input04(1.0f, glm::half_pi<float>() + glm::quarter_pi<float>());
        glm::vec2 output04 = PolarToCartesian(input04);

        glm::vec2 input05(1.0f, glm::pi<float>());
        glm::vec2 output05 = PolarToCartesian(input05);

        glm::vec2 input06(1.0f, glm::pi<float>() + glm::quarter_pi<float>());
        glm::vec2 output06 = PolarToCartesian(input06);

        glm::vec2 input07(1.0f, glm::pi<float>() + glm::half_pi<float>());
        glm::vec2 output07 = PolarToCartesian(input07);

        glm::vec2 input08(1.0f, glm::pi<float>() + glm::half_pi<float>() + glm::quarter_pi<float>());
        glm::vec2 output08 = PolarToCartesian(input08);
    }
}