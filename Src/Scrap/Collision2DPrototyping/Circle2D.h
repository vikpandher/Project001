#pragma once

#include "Engine/Collision2D/Position2D.h"



namespace Project001
{
    class Circle2D : public Position2D
    {
    public:
        Circle2D();

        void SetRadius(float radius);
        float GetRadius() const;

    protected:
        // Inherited
        // glm::vec2 position_;

        float radius_;
    };

    inline Circle2D::Circle2D()
        : radius_(0.0f)
    {}

    inline void Circle2D::SetRadius(float radius)
    {
        radius_ = radius;
    }

    inline float Circle2D::GetRadius() const
    {
        return radius_;
    }
}