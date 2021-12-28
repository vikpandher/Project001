#pragma once

#include "Engine/Components/Position.h"



namespace Project001
{
    class LightSource : public Position
    {
    public:
        LightSource();

        void TurnOn();
        void TurnOff();
        bool IsTurnedOn() const;

        void SetLightTypeDirectional();
        void SetLightTypePoint();
        void SetLightTypeSpot();
        bool IsLightTypeDirectional() const;
        bool IsLightTypePoint() const;
        bool IsLightTypeSpot() const;

        void SetDirection(const glm::vec3& direction);
        void SetDirection(float x, float y, float z);
        void SetDirectionX(float x);
        void SetDirectionY(float y);
        void SetDirectionZ(float z);
        const glm::vec3& GetDirection() const;
        float GetDirectionX() const;
        float GetDirectionY() const;
        float GetDirectionZ() const;

        void SetCutoff(float cutoff);
        float GetCutoff() const;

        void SetOuterCutoff(float outerCutoff);
        float GetOuterCutoff() const;

        void SetAttenuationConstant(float constant);
        float GetAttenuationConstant() const;

        void SetLinearAttenuation(float linear);
        float GetLinearAttenuation() const;

        void SetQuadraticAttenuation(float quadratic);
        float GetQuadraticAttenuation() const;

        void SetAmbientColor(const glm::vec3& ambient);
        void SetAmbientColor(float r, float g, float b);
        const glm::vec3& GetAmbientColor() const;

        void SetDiffuseColor(const glm::vec3& diffuse);
        void SetDiffuseColor(float r, float g, float b);
        const glm::vec3& GetDiffuseColor() const;

        void SetSpecularColor(const glm::vec3& specular);
        void SetSpecularColor(float r, float g, float b);
        const glm::vec3& GetSpecularColor() const;

    protected:
        // Inherited:
        // glm::vec3 position_;

        enum class LightType
        {
            LIGHT_TYPE_DIRECTIONAL,
            LIGHT_TYPE_POINT,
            LIGHT_TYPE_SPOT
        };

        bool turnedOn_;

        LightType lightType_;

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

    private:
    };

    inline void LightSource::TurnOn()
    {
        turnedOn_ = true;
    }

    inline void LightSource::TurnOff()
    {
        turnedOn_ = false;
    }

    inline bool LightSource::IsTurnedOn() const
    {
        return turnedOn_;
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

    inline const glm::vec3& LightSource::GetDirection() const
    {
        return direction_;
    }

    inline float LightSource::GetDirectionX() const
    {
        return direction_.x;
    }

    inline float LightSource::GetDirectionY() const
    {
        return direction_.y;
    }

    inline float LightSource::GetDirectionZ() const
    {
        return direction_.z;
    }

    inline void LightSource::SetCutoff(float cutoff)
    {
        cutoff_ = cutoff;
    }

    inline float LightSource::GetCutoff() const
    {
        return cutoff_;
    }

    inline void LightSource::SetOuterCutoff(float outerCutoff)
    {
        outerCutoff_ = outerCutoff;
    }

    inline float LightSource::GetOuterCutoff() const
    {
        return outerCutoff_;
    }

    inline void LightSource::SetAttenuationConstant(float constant)
    {
        constant_ = constant;
    }

    inline float LightSource::GetAttenuationConstant() const
    {
        return constant_;
    }

    inline void LightSource::SetLinearAttenuation(float linear)
    {
        linear_ = linear;
    }

    inline float LightSource::GetLinearAttenuation() const
    {
        return linear_;
    }

    inline void LightSource::SetQuadraticAttenuation(float quadratic)
    {
        quadratic_ = quadratic;
    }

    inline float LightSource::GetQuadraticAttenuation() const
    {
        return quadratic_;
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

    inline const glm::vec3& LightSource::GetAmbientColor() const
    {
        return ambient_;
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

    inline const glm::vec3& LightSource::GetDiffuseColor() const
    {
        return diffuse_;
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

    inline const glm::vec3& LightSource::GetSpecularColor() const
    {
        return specular_;
    }

}