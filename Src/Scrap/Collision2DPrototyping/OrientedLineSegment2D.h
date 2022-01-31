#pragma once

#include "Engine/Collision2D/Placement2D.h"



namespace Project001
{
    class OrientedLineSegment2D : public Placement2D
    {
    public:
        OrientedLineSegment2D();

        void SetStart(const glm::vec2& start);
        void SetStart(float x, float y);
        void SetStartX(float x);
        void SetStartY(float y);
        const glm::vec2& GetStart() const;

        void SetEnd(const glm::vec2& end);
        void SetEnd(float x, float y);
        void SetEndX(float x);
        void SetEndY(float y);
        const glm::vec2& GetEnd() const;

        float GetLength() const;
        float GetLengthSqared() const;

        float GetMaxX() const;
        float GetMaxY() const;

        float GetMinX() const;
        float GetMinY() const;

        float GetSlope() const;

    protected:
        // Inherited:
        // glm::vec2 position_;
        // float rotation_;

        glm::vec2 start_;
        glm::vec2 end_;

    };

    inline OrientedLineSegment2D::OrientedLineSegment2D()
        : start_(0.0f, 0.0f)
        , end_(0.0f, 0.0f)
    {}

    inline void OrientedLineSegment2D::SetStart(const glm::vec2& start)
    {
        start_ = start;
    }

    inline void OrientedLineSegment2D::SetStart(float x, float y)
    {
        start_.x = x;
        start_.y = y;
    }

    inline void OrientedLineSegment2D::SetStartX(float x)
    {
        start_.x = x;
    }

    inline void OrientedLineSegment2D::SetStartY(float y)
    {
        start_.y = y;
    }

    inline const glm::vec2& OrientedLineSegment2D::GetStart() const
    {
        return start_;
    }

    inline void OrientedLineSegment2D::SetEnd(const glm::vec2& end)
    {
        end_ = end;
    }

    inline void OrientedLineSegment2D::SetEnd(float x, float y)
    {
        end_.x = x;
        end_.y = y;
    }

    inline void OrientedLineSegment2D::SetEndX(float x)
    {
        end_.x = x;
    }

    inline void OrientedLineSegment2D::SetEndY(float y)
    {
        end_.y = y;
    }

    inline const glm::vec2& OrientedLineSegment2D::GetEnd() const
    {
        return end_;
    }

    inline float OrientedLineSegment2D::GetLength() const
    {
        return std::sqrtf(glm::dot(start_, end_));
    }

    inline float OrientedLineSegment2D::GetLengthSqared() const
    {
        return glm::dot(start_, end_);
    }

    inline float OrientedLineSegment2D::GetMaxX() const
    {
        if (start_.x > end_.x)
        {
            return start_.x;
        }
        else
        {
            return end_.x;
        }
    }

    inline float OrientedLineSegment2D::GetMaxY() const
    {
        if (start_.y > end_.y)
        {
            return start_.y;
        }
        else
        {
            return end_.y;
        }
    }

    inline float OrientedLineSegment2D::GetMinX() const
    {
        if (start_.x < end_.x)
        {
            return start_.x;
        }
        else
        {
            return end_.x;
        }
    }

    inline float OrientedLineSegment2D::GetMinY() const
    {
        if (start_.y < end_.y)
        {
            return start_.y;
        }
        else
        {
            return end_.y;
        }
    }

    inline float OrientedLineSegment2D::GetSlope() const
    {
        return (end_.y - start_.y) / (end_.x - start_.x);
    }
}