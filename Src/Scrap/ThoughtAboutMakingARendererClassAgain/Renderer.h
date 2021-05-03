#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    struct DirectionalLight {
        DirectionalLight()
            : direction(0.0f, 0.0f, 0.0f)
            , ambient(0.0f, 0.0f, 0.0f)
            , diffuse(0.0f, 0.0f, 0.0f)
            , specular(0.0f, 0.0f, 0.0f)
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
            , direction(0.0f, 0.0f, 0.0f)
            , cutoff(0.0f)
            , outerCutoff(0.0f)
            , constant(1.0f)
            , linear(0.0f)
            , quadratic(0.0f)
            , ambient(0.0f, 0.0f, 0.0f)
            , diffuse(0.0f, 0.0f, 0.0f)
            , specular(0.0f, 0.0f, 0.0f)
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
            , normal(0.0f, 0.0f, 0.0f)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , textureCoordinte(0.0f, 0.0f)
            , textureIndex(-1.0f)
            , specularIndex(-1.0f)
            , shininess(32.0f)
            , translation(0.0f, 0.0f, 0.0f)
            , orientation(1.0f, 0.0f, 0.0f, 0.0f)
        {}

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
        glm::vec2 textureCoordinte;
        float textureIndex;
        float specularIndex;
        float shininess;
        glm::vec3 translation;
        glm::vec4 orientation;
    };
 
    class Renderer
    {
    public:
        virtual void Render() const = 0;

        virtual bool AddTexture(unsigned int textureSlot, unsigned char* data, int width, int height, int numberOfComponents) = 0;

        virtual void AddVerticies(const VertexData* verticies, unsigned int count) = 0;
        
        virtual void ClearVerticies() = 0;

        virtual void SetProjectionMatrix(const glm::mat4& projectionMatrix) const = 0;

        virtual void SetViewMatrix(const glm::mat4& viewMatrix) const = 0;

        virtual void SetViewPosition(const glm::vec3& viewPosition) const = 0;

        virtual void SetDirectionalLight(const DirectionalLight& directionalLight) const = 0;

        virtual bool SetPointLight(const PointLight& pointLight, unsigned int index) const = 0;

        virtual bool SetSpotLight(const SpotLight& spotLight, unsigned int index) const = 0;

    protected:

    private:
    };
}