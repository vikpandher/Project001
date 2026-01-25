// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-24

#pragma once

#include "Components/Position2D.h"

#include "glm/gtc/constants.hpp"



namespace Project001
{
    class Placement2D : public Position2D
    {
    public:
        Placement2D();

        void TranslateUp(float d);
        void TranslateRight(float d);

        void AddRotation(float angleInRadians);
        const float& GetRotation() const;
        void SetRotation(float angleInRadians);

        glm::vec2 GetForwardVector() const;
        glm::vec2 GetRightVector() const;

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
        position_.x += d * glm::sin(-1.0f * rotation_);
        position_.y += d * glm::cos(-1.0f * rotation_);
    }
    inline void Placement2D::TranslateRight(float d)
    {
        position_.x += d * glm::cos(rotation_);
        position_.y += d * glm::sin(rotation_);
    }

    inline void Placement2D::AddRotation(float angleInRadians)
    {
        rotation_ = glm::mod(rotation_ + angleInRadians, glm::two_pi<float>());
    }

    inline const float& Placement2D::GetRotation() const
    {
        return rotation_;
    }

    inline void Placement2D::SetRotation(float angleInRadians)
    {
        rotation_ = glm::mod(angleInRadians, glm::two_pi<float>());
    }

    inline glm::vec2 Placement2D::GetForwardVector() const
    {
        return Math::Rotate2DVector(glm::vec2(0.0f, 1.0f), rotation_);
    }

    inline glm::vec2 Placement2D::GetRightVector() const
    {
        return Math::Rotate2DVector(glm::vec2(1.0f, 0.0f), rotation_);
    }
}