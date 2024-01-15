#pragma once

#include "glm/gtc/constants.hpp"

#include "Components/Position2D.h"



namespace Project001
{
    class Placement2D : public Position2D
    {
    public:
        Placement2D();

        void TranslateUp(float d);
        void TranslateRight(float d);

        void AddRotation(float angleInRadians);
        float GetRotation() const;
        void SetRotation(float angleInRadians);

    protected:
        // Inherited:
        // glm::vec2 position_;

        float rotation_; // (rotation >= 0) && (rotation < 2 * pi)
    };

    inline Placement2D::Placement2D()
        : rotation_(0.0f)
    {}

    inline void Placement2D::TranslateUp(float d)
    {
        position_.x += d * std::sin(-1.0f * rotation_);
        position_.y += d * std::cos(-1.0f * rotation_);
    }
    inline void Placement2D::TranslateRight(float d)
    {
        position_.x += d * std::cos(rotation_);
        position_.y += d * std::sin(rotation_);
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

    inline float Placement2D::GetRotation() const
    {
        return rotation_;
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
}