#pragma once

#include "glm/gtc/quaternion.hpp"

#include <vector>



namespace Project001
{
    struct DirectionalLight {
        DirectionalLight()
            : direction(0.0f, 0.0f, -1.0f)
            , ambient(0.0f, 0.0f, 0.0f)
            , diffuse(0.0f, 0.0f, 0.0f)
            , specular(0.0f, 0.0f, 0.0f)
        {}

        DirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : direction(direction)
            , ambient(ambient)
            , diffuse(diffuse)
            , specular(specular)
        {}

        // Direction the light is going
        glm::vec3 direction;

        // Ambient light applied to everything from all directions evenly
        glm::vec3 ambient;

        // Light comming from a direction
        glm::vec3 diffuse;

        // The color of the shine
        glm::vec3 specular;
    };

    struct PointLight {
        PointLight()
            : position(0.0f, 0.0f, 0.0f)
            , constant(1.0f)
            , linear(0.0f)
            , quadratic(0.0f)
            , ambient(0.0f, 0.0f, 0.0f)
            , diffuse(0.0f, 0.0f, 0.0f)
            , specular(0.0f, 0.0f, 0.0f)
        {}

        PointLight(
            const glm::vec3& position,
            const float& constant,
            const float& linear,
            const float& quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : position(position)
            , constant(constant)
            , linear(linear)
            , quadratic(quadratic)
            , ambient(ambient)
            , diffuse(diffuse)
            , specular(specular)
        {}

        // Position of the light's source
        glm::vec3 position;

        // This term is usually kept at 1.0 which is maily there to make sure
        // the denominator for the attenuation calculation never gets smaller
        // than 1 since otherwise it would boost the intensity with certain
        // distances
        float constant;

        // This reduces the intesity of the light in a linear fashion
        // Higher equals faster dimming
        float linear;

        // This reduces the intesity of the light in a quadration fashin
        // Higher equals faster dimming
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct SpotLight {
        SpotLight()
            : position(0.0f, 0.0f, 0.0f)
            , direction(0.0f, 0.0f, -1.0f)
            , cutoff(0.0f)
            , outerCutoff(0.0f)
            , constant(1.0f)
            , linear(0.0f)
            , quadratic(0.0f)
            , ambient(0.0f, 0.0f, 0.0f)
            , diffuse(0.0f, 0.0f, 0.0f)
            , specular(0.0f, 0.0f, 0.0f)
        {}

        SpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            const float& cutoff,
            const float& outerCutoff,
            const float& constant,
            const float& linear,
            const float& quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular)
            : position(position)
            , direction(direction)
            , cutoff(cutoff)
            , outerCutoff(outerCutoff)
            , constant(constant)
            , linear(linear)
            , quadratic(quadratic)
            , ambient(ambient)
            , diffuse(diffuse)
            , specular(specular)
        {}

        glm::vec3 position;
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
        float cutoff;
        float outerCutoff;

        float constant;
        float linear;
        float quadratic;

        // Amblient light is cutoff as well, but will light the target from
        // behind.
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct VertexData
    {
        VertexData()
            : position(0.0f, 0.0f, 0.0f)
            , textureCoordinate(0.0f, 0.0f)
            , normal(0.0f, 0.0f, 0.0f)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , textureUnit(-1.0f)
            , specularUnit(99.0f)
            , shininess(32.0f)
            , scale(1.0f, 1.0f, 1.0f)
            , translation(0.0f, 0.0f, 0.0f)
            , orientation(0.0f, 0.0f, 0.0f, 1.0f)
            , lit(1.0f)
        {}

        glm::vec3 position;
        glm::vec2 textureCoordinate;
        glm::vec3 normal;
        glm::vec4 color;
        float textureUnit;
        float specularUnit;
        float shininess;
        glm::vec3 scale;
        glm::vec3 translation;
        glm::vec4 orientation; // x, y, z, w
        float lit;
    };
}