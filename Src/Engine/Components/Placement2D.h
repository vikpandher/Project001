#pragma once

#include "glm/gtc/constants.hpp"

#include "Engine/Components/Position2D.h"



namespace Project001
{
    class Placement2D : public Position2D
    {
    public:
        Placement2D();

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