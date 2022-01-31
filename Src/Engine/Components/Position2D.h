#pragma once

#include "glm/glm.hpp"

#include "Engine/Math/VectorAngles.h"



namespace Project001
{
    class Position2D
    {
    public:
        Position2D();

        void SetPosition(const glm::vec2& position);
        void SetPosition(float x, float y);
        void SetPositionX(float x);
        void SetPositionY(float y);
        const glm::vec2& GetPosition() const;

        void AddTranslation(const glm::vec2& translation);
        void AddTranslation(float x, float y);
        void AddTranslationX(float x);
        void AddTranslationY(float y);

        void RevolveAround(const glm::vec2& focalPoint, float angleInRadians);

     protected:
        glm::vec2 position_;
    };

    inline Position2D::Position2D()
        : position_(0.0f, 0.0f)
    {}

    inline void Position2D::SetPosition(const glm::vec2& position)
    {
        position_ = position;
    }

    inline void Position2D::SetPosition(float x, float y)
    {
        position_.x = x;
        position_.y = y;
    }

    inline void Position2D::SetPositionX(float x)
    {
        position_.x = x;
    }

    inline void Position2D::SetPositionY(float y)
    {
        position_.y = y;
    }

    inline const glm::vec2& Position2D::GetPosition() const
    {
        return position_;
    }

    inline void Position2D::AddTranslation(const glm::vec2& translation)
    {
        position_ += translation;
    }

    inline void Position2D::AddTranslation(float x, float y)
    {
        position_.x += x;
        position_.y += y;
    }

    inline void Position2D::AddTranslationX(float x)
    {
        position_.x += x;
    }

    inline void Position2D::AddTranslationY(float y)
    {
        position_.y += y;
    }

    inline void Position2D::RevolveAround(const glm::vec2& focalPoint, float angleInRadians)
    {
        glm::vec2 focalPointToPosition = position_ - focalPoint;
        glm::vec2 focalPointToNewPosition = Rotate2DVector(focalPointToPosition, angleInRadians);
        glm::vec2 positionToNewPosition = focalPointToNewPosition - focalPointToPosition;
        position_ += positionToNewPosition;
    }
}