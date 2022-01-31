#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    static const float g_floattMarginOfError = 0.000001f;
    
    inline bool FloatsEqual(float a, float b, float epsilon = g_floattMarginOfError)
    {
        return glm::abs(a - b) < epsilon;
    }
}