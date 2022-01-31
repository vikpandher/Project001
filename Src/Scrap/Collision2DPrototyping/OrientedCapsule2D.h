#pragma once

#include "Engine/Collision2D/OrientedLineSegment2D.h"



namespace Project001
{
    class OrientedCapsule2D : public OrientedLineSegment2D
    {
    public:
        OrientedCapsule2D();

        void SetRadius(float radius);
        float GetRadius() const;

    protected:
        // Inherited:
        // glm::vec2 position_;
        // float rotation_;
        // glm::vec2 start_;
        // glm::vec2 end_;

        float radius_;
    };

    inline OrientedCapsule2D::OrientedCapsule2D()
        : radius_(0.0f)
    {}

    inline void OrientedCapsule2D::SetRadius(float radius)
    {
        radius_ = radius;
    }

    inline float OrientedCapsule2D::GetRadius() const
    {
        return radius_;
    }
}