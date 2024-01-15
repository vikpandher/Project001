#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    static const float g_floatMarginOfError = 0.00001f;
    
    bool FloatEqualToFloat(float a, float b, float epsilon = g_floatMarginOfError);

    bool FloatGreaterThanOrEqualToFloat(float a, float b, float epsilon = g_floatMarginOfError);

    bool FloatLessThanOrEqualToFloat(float a, float b, float epsilon = g_floatMarginOfError);

    // Unused
    float GetMax(float a, float b);

    // Unused
    float GetMin(float a, float b);

    void GetMinMax(float a, float b, float& min, float& max);

    void Swap(float& a, float& b);
}

#include "FloatComparators.inl"