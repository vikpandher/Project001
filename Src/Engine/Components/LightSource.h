#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    struct LightSource
    {
        enum class LightType
        {
            LIGHT_TYPE_DIRECTIONAL,
            LIGHT_TYPE_POINT,
            LIGHT_TYPE_SPOT
        };

        LightSource();

        bool turnedOn;

        LightType lightType;

        // The origin of the light
        glm::vec3 position;

        // Direction the light is going
        // 
        // For: LIGHT_TYPE_DIRECTIONAL_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        glm::vec3 direction;

        // The cut-offs determine the angle the cone of light spreads from the
        // source:
        // A value of 0.0 equals a 180 degree angle of light, a completely
        // flattened cone.
        // A value of 1.0 equals a 0 degree angle of light, an infinitly skinny
        // cone.
        // Light will dim from the cutoff to the outerCutoff. Valid values are
        // from 0.0 to 1.0.
        //
        // An example of some good values are:
        // cutoff = 0.99 & outterCutoff = 0.97
        // 
        // For: LIGHT_TYPE_SPOT_LIGHT
        float cutoff;
        float outerCutoff;

        // This term is usually kept at 1.0 which is maily there to make sure
        // the denominator for the attenuation calculation never gets smaller
        // than 1 since otherwise it would boost the intensity with certain
        // distances
        // 
        // a = light attenuation
        // c = constant
        // l = linear
        // q = quadratic
        // d = distance from the light to the fragment being lit
        // a = 1 / (c + (l * d) + (q * d * d));
        // 
        // For: LIGHT_TYPE_POINT_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        float constant;

        // This reduces the intesity of the light in a linear fashion
        // Higher equals faster dimming
        // 
        // For: LIGHT_TYPE_POINT_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        float linear;

        // This reduces the intesity of the light in a quadration fashin
        // Higher equals faster dimming
        // 
        // For: LIGHT_TYPE_POINT_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        float quadratic;

        // Ambient light applied to everything from all directions evenly.
        // 
        // In the case of LIGHT_TYPE_SPOT_LIGHT, amblient light is cutoff using
        // the cutoffs, but abient light will light the target from behind.
        glm::vec3 ambient;

        // Light comming from a direction
        glm::vec3 diffuse;

        // The color of the shine
        glm::vec3 specular;
    };

    LightSource::LightSource()
        : turnedOn(false)
        , lightType(LightType::LIGHT_TYPE_POINT)
        , position(0.0f, 0.0f, 0.0f)
        , direction(0.0f, 0.0f, 0.0f)
        , cutoff(0.99f)
        , outerCutoff(0.97f)
        , constant(1.0f)
        , linear(0.0f)
        , quadratic(0.0f)
        , ambient(0.0f, 0.0f, 0.0f)
        , diffuse(1.0f, 1.0f, 1.0f)
        , specular(0.0f, 0.0f, 0.0f)
    {}
}