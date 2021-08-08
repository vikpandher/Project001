#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    class Position
    {
    public:
        Position();
        ~Position();

        void SetPosition(const glm::vec3& position);
        void SetPosition(float x, float y, float z);
        void SetPositionX(float x);
        void SetPositionY(float y);
        void SetPositionZ(float z);
        const glm::vec3& GetPosition() const;
        float GetPositionX() const;
        float GetPositionY() const;
        float GetPositionZ() const;

        void AddTranslation(const glm::vec3& translation);
        void AddTranslation(float x, float y, float z);
        void AddTranslationX(float x);
        void AddTranslationY(float y);
        void AddTranslationZ(float z);

     protected:
        glm::vec3 position_;

    private:
    };

    inline void Position::SetPosition(const glm::vec3& position)
    {
        position_ = position;
    }

    inline void Position::SetPosition(float x, float y, float z)
    {
        position_.x = x;
        position_.y = y;
        position_.z = z;
    }

    inline void Position::SetPositionX(float x)
    {
        position_.x = x;
    }

    inline void Position::SetPositionY(float y)
    {
        position_.y = y;
    }

    inline void Position::SetPositionZ(float z)
    {
        position_.z = z;
    }

    inline const glm::vec3& Position::GetPosition() const
    {
        return position_;
    }

    inline float Position::GetPositionX() const
    {
        return position_.x;
    }

    inline float Position::GetPositionY() const
    {
        return position_.y;
    }

    inline float Position::GetPositionZ() const
    {
        return position_.z;
    }

    inline void Position::AddTranslation(const glm::vec3& translation)
    {
        position_ += translation;
    }

    inline void Position::AddTranslation(float x, float y, float z)
    {
        position_.x += x;
        position_.y += y;
        position_.z += z;
    }

    inline void Position::AddTranslationX(float x)
    {
        position_.x += x;
    }

    inline void Position::AddTranslationY(float y)
    {
        position_.y += y;
    }

    inline void Position::AddTranslationZ(float z)
    {
        position_.z += z;
    }
}