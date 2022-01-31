#pragma once

#include "Engine/Collision2D/Position2D.h"



namespace Project001
{
    class Rectangle2D : public Position2D
    {
    public:
        Rectangle2D();

        void SetHalfSize(const glm::vec2& halfSize);
        void SetHalfSize(float x, float y);
        void SetHalfSizeX(float x);
        void SetHalfSizeY(float y);
        const glm::vec2& GetHalfSize() const;

        // float GetMaxX() const;
        // float GetMaxY() const;
        // float GetMinX() const;
        // float GetMinY() const;

    protected:
        // Inherited:
        // glm::vec3 position_;

        glm::vec2 halfSize_;
    };

    inline Rectangle2D::Rectangle2D()
        : halfSize_(0.0f, 0.0f)
    {}

    inline void Rectangle2D::SetHalfSize(const glm::vec2& halfSize)
    {
        halfSize_ = halfSize;
    }

    inline void Rectangle2D::SetHalfSize(float x, float y)
    {
        halfSize_.x = x;
        halfSize_.y = y;
    }

    inline void Rectangle2D::SetHalfSizeX(float x)
    {
        halfSize_.x = x;
    }

    inline void Rectangle2D::SetHalfSizeY(float y)
    {
        halfSize_.y = y;
    }

    inline const glm::vec2& Rectangle2D::GetHalfSize() const
    {
        return halfSize_;
    }

    // inline float Rectangle2D::GetMaxX() const
    // {
    //     return position_.x + halfSize_.x;
    // }

    // inline float Rectangle2D::GetMaxY() const
    // {
    //     return position_.y + halfSize_.y;
    // }

    // inline float Rectangle2D::GetMinX() const
    // {
    //     return position_.x - halfSize_.x;
    // }

    // inline float Rectangle2D::GetMinY() const
    // {
    //     return position_.y - halfSize_.y;
    // }
}