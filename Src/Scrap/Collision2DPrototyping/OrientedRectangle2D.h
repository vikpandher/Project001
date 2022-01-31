#pragma once

#include "Engine/Collision2D/Placement2D.h"



namespace Project001
{
    class OrientedRectangle2D : public Placement2D
    {
    public:
        OrientedRectangle2D();

        void SetHalfSize(const glm::vec2& halfSize);
        void SetHalfSize(float x, float y);
        void SetHalfSizeX(float x);
        void SetHalfSizeY(float y);
        const glm::vec2& GetHalfSize() const;

    protected:
        // Inherited:
        // glm::vec2 position_;
        // float rotation_;

        glm::vec2 halfSize_;
    };

    inline OrientedRectangle2D::OrientedRectangle2D()
        : halfSize_(0.0f, 0.0f)
    {}

    inline void OrientedRectangle2D::SetHalfSize(const glm::vec2& halfSize)
    {
        halfSize_ = halfSize;
    }

    inline void OrientedRectangle2D::SetHalfSize(float x, float y)
    {
        halfSize_.x = x;
        halfSize_.y = y;
    }

    inline void OrientedRectangle2D::SetHalfSizeX(float x)
    {
        halfSize_.x = x;
    }

    inline void OrientedRectangle2D::SetHalfSizeY(float y)
    {
        halfSize_.y = y;
    }

    inline const glm::vec2& OrientedRectangle2D::GetHalfSize() const
    {
        return halfSize_;
    }
}