// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-24

#pragma once

#include "Components/Position.h"



namespace Project001
{
    class LightSource : public Position
    {
    public:
        LightSource();

        bool IsLightTypeDirectional() const;
        bool IsLightTypePoint() const;
        bool IsLightTypeSpot() const;
        void SetLightTypeDirectional();
        void SetLightTypePoint();
        void SetLightTypeSpot();

        bool IsTurnedOn() const;
        void TurnOn();
        void TurnOff();

        void SetCameraMask(uint32_t cameraMask);
        uint32_t GetCameraMask() const;

        const glm::vec3& GetDirection() const;
        const float& GetDirectionX() const;
        const float& GetDirectionY() const;
        const float& GetDirectionZ() const;
        void SetDirection(const glm::vec3& direction);
        void SetDirection(float x, float y, float z);
        void SetDirectionX(float x);
        void SetDirectionY(float y);
        void SetDirectionZ(float z);

        const float& GetCutoff() const;
        void SetCutoff(float cutoff);

        const float& GetOuterCutoff() const;
        void SetOuterCutoff(float outerCutoff);

        const float& GetAttenuationConstant() const;
        void SetAttenuationConstant(float constant);

        const float& GetLinearAttenuation() const;
        void SetLinearAttenuation(float linear);

        const float& GetQuadraticAttenuation() const;
        void SetQuadraticAttenuation(float quadratic);

        const glm::vec3& GetAmbientColor() const;
        void SetAmbientColor(const glm::vec3& ambient);
        void SetAmbientColor(float r, float g, float b);

        const glm::vec3& GetDiffuseColor() const;
        void SetDiffuseColor(const glm::vec3& diffuse);
        void SetDiffuseColor(float r, float g, float b);

        const glm::vec3& GetSpecularColor() const;
        void SetSpecularColor(const glm::vec3& specular);
        void SetSpecularColor(float r, float g, float b);

    protected:
        // Inherited:
        // glm::vec3 position_;

        enum class LightType
        {
            LIGHT_TYPE_DIRECTIONAL,
            LIGHT_TYPE_POINT,
            LIGHT_TYPE_SPOT
        };

        LightType lightType_;

        bool turnedOn_;

        uint32_t cameraMask_;

        // Direction the light is going
        // 
        // For: LIGHT_TYPE_DIRECTIONAL_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        glm::vec3 direction_;

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
        float cutoff_;
        float outerCutoff_;

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
        float constant_;

        // This reduces the intesity of the light in a linear fashion
        // Higher equals faster dimming
        // 
        // For: LIGHT_TYPE_POINT_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        float linear_;

        // This reduces the intesity of the light in a quadration fashin
        // Higher equals faster dimming
        // 
        // For: LIGHT_TYPE_POINT_LIGHT && LIGHT_TYPE_SPOT_LIGHT
        float quadratic_;

        // Ambient light applied to everything from all directions evenly.
        // 
        // In the case of LIGHT_TYPE_SPOT_LIGHT, amblient light is cutoff using
        // the cutoffs, but abient light will light the target from behind.
        glm::vec3 ambient_;

        // Light comming from a direction
        glm::vec3 diffuse_;

        // The color of the shine
        glm::vec3 specular_;
    };

    inline LightSource::LightSource()
        : lightType_(LightType::LIGHT_TYPE_POINT)
        , turnedOn_(false)
        , cameraMask_(0b00000000000000000000000000000001)
        , direction_(0.0f, 0.0f, 0.0f)
        , cutoff_(0.99f)
        , outerCutoff_(0.97f)
        , constant_(1.0f)
        , linear_(0.0f)
        , quadratic_(0.0f)
        , ambient_(0.0f, 0.0f, 0.0f)
        , diffuse_(1.0f, 1.0f, 1.0f)
        , specular_(0.0f, 0.0f, 0.0f)
    {}

    inline bool LightSource::IsLightTypeDirectional() const
    {
        return lightType_ == LightType::LIGHT_TYPE_DIRECTIONAL;
    }

    inline bool LightSource::IsLightTypePoint() const
    {
        return lightType_ == LightType::LIGHT_TYPE_POINT;
    }

    inline bool LightSource::IsLightTypeSpot() const
    {
        return lightType_ == LightType::LIGHT_TYPE_SPOT;
    }

    inline void LightSource::SetLightTypeDirectional()
    {
        lightType_ = LightType::LIGHT_TYPE_DIRECTIONAL;
    }

    inline void LightSource::SetLightTypePoint()
    {
        lightType_ = LightType::LIGHT_TYPE_POINT;
    }

    inline void LightSource::SetLightTypeSpot()
    {
        lightType_ = LightType::LIGHT_TYPE_SPOT;
    }

    inline bool LightSource::IsTurnedOn() const
    {
        return turnedOn_;
    }

    inline void LightSource::TurnOn()
    {
        turnedOn_ = true;
    }

    inline void LightSource::TurnOff()
    {
        turnedOn_ = false;
    }

    inline void LightSource::SetCameraMask(uint32_t cameraMask)
    {
        cameraMask_ = cameraMask;
    }

    inline uint32_t LightSource::GetCameraMask() const
    {
        return cameraMask_;
    }

    inline const glm::vec3& LightSource::GetDirection() const
    {
        return direction_;
    }

    inline const float& LightSource::GetDirectionX() const
    {
        return direction_.x;
    }

    inline const float& LightSource::GetDirectionY() const
    {
        return direction_.y;
    }

    inline const float& LightSource::GetDirectionZ() const
    {
        return direction_.z;
    }

    inline void LightSource::SetDirection(const glm::vec3& direction)
    {
        direction_ = direction;
    }
    inline void LightSource::SetDirection(float x, float y, float z)
    {
        direction_.x = x;
        direction_.y = y;
        direction_.z = z;
    }

    inline void LightSource::SetDirectionX(float x)
    {
        direction_.x = x;
    }

    inline void LightSource::SetDirectionY(float y)
    {
        direction_.y = y;
    }

    inline void LightSource::SetDirectionZ(float z)
    {
        direction_.z = z;
    }

    inline const float& LightSource::GetCutoff() const
    {
        return cutoff_;
    }

    inline void LightSource::SetCutoff(float cutoff)
    {
        cutoff_ = cutoff;
    }

    inline const float& LightSource::GetOuterCutoff() const
    {
        return outerCutoff_;
    }

    inline void LightSource::SetOuterCutoff(float outerCutoff)
    {
        outerCutoff_ = outerCutoff;
    }

    inline const float& LightSource::GetAttenuationConstant() const
    {
        return constant_;
    }

    inline void LightSource::SetAttenuationConstant(float constant)
    {
        constant_ = constant;
    }

    inline const float& LightSource::GetLinearAttenuation() const
    {
        return linear_;
    }

    inline void LightSource::SetLinearAttenuation(float linear)
    {
        linear_ = linear;
    }

    inline const float& LightSource::GetQuadraticAttenuation() const
    {
        return quadratic_;
    }

    inline void LightSource::SetQuadraticAttenuation(float quadratic)
    {
        quadratic_ = quadratic;
    }

    inline const glm::vec3& LightSource::GetAmbientColor() const
    {
        return ambient_;
    }

    inline void LightSource::SetAmbientColor(const glm::vec3& ambient)
    {
        ambient_ = ambient;
    }

    inline void LightSource::SetAmbientColor(float r, float g, float b)
    {
        ambient_.r = r;
        ambient_.g = g;
        ambient_.b = b;
    }

    inline const glm::vec3& LightSource::GetDiffuseColor() const
    {
        return diffuse_;
    }

    inline void LightSource::SetDiffuseColor(const glm::vec3& diffuse)
    {
        diffuse_ = diffuse;
    }

    inline void LightSource::SetDiffuseColor(float r, float g, float b)
    {
        diffuse_.r = r;
        diffuse_.g = g;
        diffuse_.b = b;
    }

    inline const glm::vec3& LightSource::GetSpecularColor() const
    {
        return specular_;
    }

    inline void LightSource::SetSpecularColor(const glm::vec3& specular)
    {
        specular_ = specular;
    }

    inline void LightSource::SetSpecularColor(float r, float g, float b)
    {
        specular_.r = r;
        specular_.g = g;
        specular_.b = b;
    }
}