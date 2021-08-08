#pragma once

#include "glm/gtc/quaternion.hpp"



namespace Project001
{
    struct TestComponent00
    {
        TestComponent00(int a, int b, int c);

        inline int GetSum()
        {
            return a + b + c;
        }

        int a;
        int b;
        int c;
    };

    struct TestComponent01
    {
        TestComponent01(float x, float y, float z);

        float x;
        float y;
        float z;
    };

    struct TestComponent02
    {
        TestComponent02(unsigned int r, unsigned int g, unsigned int b);

        unsigned int r;
        unsigned int g;
        unsigned int b;
    };

    struct SceneDataComponent
    {
        SceneDataComponent();

        glm::vec2 previousCursorDownPosition;
    };

    struct DeathFlagComponent
    {
        DeathFlagComponent();

        bool dead;
    };

    struct CameraComponent
    {
        CameraComponent();

        inline glm::mat4 GetViewMatrix() const
        {
            if (orbitMode)
            {
                glm::mat4 focalPointTranslation = glm::translate(glm::mat4(1.0f), focalPoint);
                glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
                glm::mat4 radiusTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, radius));
                glm::mat4 inverseViewMatrix = focalPointTranslation * rotationMatrix * radiusTranslation;
                return glm::inverse(inverseViewMatrix);
            }
            else
            {
                glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
                glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
                glm::mat4 inverseViewMatrix = translationMatrix * rotationMatrix;
                return glm::inverse(inverseViewMatrix);
            }
        }

        inline glm::mat4 GetProjectionMatrix() const
        {
            if (orthographicProjection)
            {
                return glm::ortho(leftCutOff, rightCutOff, bottomCutOff, topCutOff, nearCutOff, farCutOff);
            }
            else
            {
                return glm::perspective(glm::radians(fieldOfVisionDegrees), aspectRatio, nearCutOff, farCutOff);
            }
        }

        inline glm::vec3 GetForwardVector() const
        {
            return orientation * s_worldForward;
        }

        inline glm::vec3 GetRightVector() const
        {
            return orientation * s_worldRight;
        }

        inline glm::vec3 GetUpVector() const
        {
            return orientation * s_worldUp;
        }

        bool turnedOn;

        bool orbitMode;

        glm::vec3 position; // only used in free flight mode
        glm::vec3 focalPoint; // only used in orbit mode
        float radius; // only used in orbit mode

        // TODO:
        // float pitch;
        // float yaw;
        // float roll;

        glm::quat orientation; // w, x, y, z

        bool orthographicProjection;

        float fieldOfVisionDegrees; // only used in perspective projection
        float aspectRatio; // (width/height) only used in perspective projection
        float leftCutOff;  // only used in othographic projection
        float rightCutOff; // only used in othographic projection
        float bottomCutOff; // only used in othographic projection
        float topCutOff; // only used in othographic projection
        float nearCutOff;
        float farCutOff;

        // World Coordinate System
        // ---------------------------------------------------------------------

        // NOTE:
        // positive x is to the right, positive y is up, and positive z is out of the screen

        static glm::vec3 s_worldForward;
        static glm::vec3 s_worldUp;
        static glm::vec3 s_worldRight;
    };

    struct LightComponent
    {
        enum class LightType
        {
            LIGHT_TYPE_DIRECTIONAL_LIGHT,
            LIGHT_TYPE_POINT_LIGHT,
            LIGHT_TYPE_SPOT_LIGHT
        };
        
        LightComponent();

        bool turnedOn;

        LightType lightType;

        // Position of the light's source
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

    struct RenderedModelComponent
    {
        RenderedModelComponent();

        unsigned int modelIndex;
        unsigned int textureIndex;
        unsigned int specularIndex;
        float shininess;
        glm::vec4 color;
        glm::vec3 scale;
        glm::vec3 position;
        glm::quat orientation;

        // TODO:
        //bool transparent;
    };

    struct PhysicsBodyComponent
    {
        PhysicsBodyComponent();

        unsigned int modelIndex;
        glm::vec3 position;
        glm::quat orientation;
        glm::vec3 velocity;
    };

}