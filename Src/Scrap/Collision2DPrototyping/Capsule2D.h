#pragma once

#include "Engine/Collision2D/LineSegment2D.h"



namespace Project001
{
    class Capsule2D : public LineSegment2D
    {
    public:
        Capsule2D();

        void SetRadius(float radius);
        float GetRadius() const;

    protected:
        // Inherited
        // glm::vec2 start_;
        // glm::vec2 end_;

        float radius_;
    };

    inline Capsule2D::Capsule2D()
        : radius_(0.0f)
    {}

    inline void Capsule2D::SetRadius(float radius)
    {
        radius_ = radius;
    }

    inline float Capsule2D::GetRadius() const
    {
        return radius_;
    }
}