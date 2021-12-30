#pragma once

#include "glm/gtc/constants.hpp"
#include "glm/glm.hpp"

#include "Engine/Math/VectorAngles.h"



namespace Project001
{
    class Placement2D
    {
    public:
        Placement2D();

        void SetPosition(const glm::vec2& position);
        void SetPosition(float x, float y);
        void SetPositionX(float x);
        void SetPositionY(float y);
        const glm::vec2& GetPosition() const;
        float GetPositionX() const;
        float GetPositionY() const;

        void AddTranslation(const glm::vec2& translation);
        void AddTranslation(float x, float y);
        void AddTranslationX(float x);
        void AddTranslationY(float y);

        void RevolveAround(const glm::vec2& focalPoint, float angleInRadians);

        void SetRotation(float angleInRadians);
        float GetRotation() const;

        void AddRotation(float angleInRadians);

     protected:
        glm::vec2 position_;

        float rotation_; // (rotation >= 0) && (rotation < 2 * pi)

    private:
    };

    inline Placement2D::Placement2D()
        : position_(0.0f, 0.0f)
        , rotation_(0.0f)
    {}

    inline void Placement2D::SetPosition(const glm::vec2& position)
    {
        position_ = position;
    }

    inline void Placement2D::SetPosition(float x, float y)
    {
        position_.x = x;
        position_.y = y;
    }

    inline void Placement2D::SetPositionX(float x)
    {
        position_.x = x;
    }

    inline void Placement2D::SetPositionY(float y)
    {
        position_.y = y;
    }

    inline const glm::vec2& Placement2D::GetPosition() const
    {
        return position_;
    }

    inline float Placement2D::GetPositionX() const
    {
        return position_.x;
    }

    inline float Placement2D::GetPositionY() const
    {
        return position_.y;
    }

    inline void Placement2D::AddTranslation(const glm::vec2& translation)
    {
        position_ += translation;
    }

    inline void Placement2D::AddTranslation(float x, float y)
    {
        position_.x += x;
        position_.y += y;
    }

    inline void Placement2D::AddTranslationX(float x)
    {
        position_.x += x;
    }

    inline void Placement2D::AddTranslationY(float y)
    {
        position_.y += y;
    }

    inline void Placement2D::RevolveAround(const glm::vec2& focalPoint, float angleInRadians)
    {
        glm::vec2 focalPointToPosition = position_ - focalPoint;
        glm::vec2 focalPointToNewPosition = Rotate2DVector(focalPointToPosition, angleInRadians);
        glm::vec2 positionToNewPosition = focalPointToNewPosition - focalPointToPosition;
        position_ += positionToNewPosition;
    }

    inline void Placement2D::SetRotation(float angleInRadians)
    {
        rotation_ = angleInRadians;
        if (rotation_ < 0.0f)
        {
            rotation_ += glm::two_pi<float>();
        }
        else if (rotation_ >= glm::two_pi<float>())
        {
            rotation_ -= glm::two_pi<float>();
        }
    }

    inline float Placement2D::GetRotation() const
    {
        return rotation_;
    }

    inline void Placement2D::AddRotation(float angleInRadians)
    {
        rotation_ += angleInRadians;
        if (rotation_ < 0.0f)
        {
            rotation_ += glm::two_pi<float>();
        }
        else if (rotation_ >= glm::two_pi<float>())
        {
            rotation_ -= glm::two_pi<float>();
        }
    }
}