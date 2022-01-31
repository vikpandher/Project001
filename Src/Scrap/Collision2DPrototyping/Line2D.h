#pragma once

#include "Engine/Collision2D/Position2D.h"



namespace Project001
{
    class Line2D : public Position2D
    {
    public:
        Line2D();

        void SetSlope(float slope);
        float GetSlope() const;

    protected:
        // Inherited:
        // glm::vec3 position_;

        float slope_;
    };

    inline Line2D::Line2D()
        : slope_(0.0f)
    {}

    inline void Line2D::SetSlope(float slope)
    {
        slope_ = slope;
    }

    inline float Line2D::GetSlope() const
    {
        return slope_;
    }
}