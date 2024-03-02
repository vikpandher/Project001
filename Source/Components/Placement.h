#pragma once

#include "glm/gtc/quaternion.hpp"

#include "Components/Position.h"



namespace Project001
{
    class Placement : public Position
    {
    public:
        Placement();

        // Position Controls
        // ---------------------------------------------------------------------

        void MoveForward(float translation);
        void MoveBack(float translation);
        void MoveRight(float translation);
        void MoveLeft(float translation);
        void MoveUp(float translation);
        void MoveDown(float translation);

        void RevolveAround(const glm::vec3& focalPoint, float angleInRadians, const glm::vec3& normal);
        void RevolveAroundHorizontally(const glm::vec3& focalPoint, float angleInRadians);

        // Orientation Controls
        // ---------------------------------------------------------------------
        // Rotation follows the right hand rule.
        // 
        // The world is a Right-Handed Coordinate system.
        // If x is positive to the right and y is positive to the top,
        // z is positive sticking out of the screen.
        // 
        // By default the camera's forward vector is (0.0f, 0.0f, 1.0f).

        void ResetOrientation();

        const glm::quat& GetOrientation() const;
        void SetOrientation(const glm::quat& orientation);
        void SetOrientation(float w, float x, float y, float z);

        void AddRotation(const glm::quat& rotation);

        void AddRelativeRotation(float rotationInRadians, const glm::vec3 axis);

        void AddRelativeRotationX(float rotationInRadians);
        void AddRelativeRotationY(float rotationInRadians);
        void AddRelativeRotationZ(float rotationInRadians);

        float GetPitch() const;
        void AddPitch(float rotationInRadians);

        float GetYaw() const;
        void AddYaw(float rotationInRadians);

        float GetRoll() const;
        void AddRoll(float rotationInRadians);

        void AddWorldRotation(float rotationInRadians, const glm::vec3 axis);

        void AddWorldRotationX(float rotationInRadians);
        void AddWorldRotationY(float rotationInRadians);
        void AddWorldRotationZ(float rotationInRadians);

        void LookAt(const glm::vec3& direction, const glm::vec3& up);
        void LookAt(const glm::vec3& direction);

        glm::vec3 GetForwardVector() const;
        glm::vec3 GetLeftVector() const;
        glm::vec3 GetUpVector() const;

    protected:
        // Inherited:
        // glm::vec3 position_;

        glm::quat orientation_; // w, x, y, z
    };

    inline Placement::Placement()
        : orientation_(1.0f, 0.0f, 0.0f, 0.0f)
    {}

    inline void Placement::MoveForward(float translation)
    {
        AddTranslation(translation * GetForwardVector());
    }

    inline void Placement::MoveBack(float translation)
    {
        AddTranslation(-1.0f * translation * GetForwardVector());
    }

    inline void Placement::MoveRight(float translation)
    {
        AddTranslation(-1.0f * translation * GetLeftVector());
    }

    inline void Placement::MoveLeft(float translation)
    {
        AddTranslation(translation * GetLeftVector());
    }

    inline void Placement::MoveUp(float translation)
    {
        AddTranslation(translation * GetUpVector());
    }

    inline void Placement::MoveDown(float translation)
    {
        AddTranslation(-1.0f * translation * GetUpVector());
    }

    inline void Placement::RevolveAround(const glm::vec3& focalPoint, float angleInRadians, const glm::vec3& normal)
    {
        glm::vec3 focalPointToPosition = position_ - focalPoint;
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), angleInRadians, normal);
        glm::vec3 focalPointToNewPosition = rotation * focalPointToPosition;
        glm::vec3 positionToNewPosition = focalPointToNewPosition - focalPointToPosition;
        position_ += positionToNewPosition;
    }

    inline void Placement::RevolveAroundHorizontally(const glm::vec3& focalPoint, float angleInRadians)
    {
        RevolveAround(focalPoint, angleInRadians, GetUpVector());
    }

    inline void Placement::ResetOrientation()
    {
        orientation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    inline const glm::quat& Placement::GetOrientation() const
    {
        return orientation_;
    }

    inline void Placement::SetOrientation(const glm::quat& orientation)
    {
        orientation_ = orientation;
    }

    inline void Placement::SetOrientation(float w, float x, float y, float z)
    {
        orientation_.w = w;
        orientation_.x = x;
        orientation_.y = y;
        orientation_.z = z;
    }

    inline void Placement::AddRotation(const glm::quat& rotation)
    {
        orientation_ = rotation * orientation_;
    }

    inline void Placement::AddRelativeRotation(float rotationInRadians, const glm::vec3 axis)
    {
        orientation_ = glm::rotate(orientation_, rotationInRadians, axis);
    }

    inline void Placement::AddRelativeRotationX(float rotationInRadians)
    {
        AddRelativeRotation(rotationInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    inline void Placement::AddRelativeRotationY(float rotationInRadians)
    {
        AddRelativeRotation(rotationInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    inline void Placement::AddRelativeRotationZ(float rotationInRadians)
    {
        AddRelativeRotation(rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    inline float Placement::GetPitch() const
    {
        return glm::pitch(orientation_);
    }

    inline void Placement::AddPitch(float rotationInRadians)
    {
        AddRelativeRotationX(rotationInRadians);
    }

    inline float Placement::GetYaw() const
    {
        return glm::yaw(orientation_);
    }

    inline void Placement::AddYaw(float rotationInRadians)
    {
        AddRelativeRotationY(rotationInRadians);
    }

    inline float Placement::GetRoll() const
    {
        return glm::roll(orientation_);
    }

    inline void Placement::AddRoll(float rotationInRadians)
    {
        AddRelativeRotationZ(rotationInRadians);
    }

    inline void Placement::AddWorldRotation(float rotationInRadians, const glm::vec3 axis)
    {
        orientation_ = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, axis) * orientation_;
    }

    inline void Placement::AddWorldRotationX(float rotationInRadians)
    {
        orientation_ = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(1.0f, 0.0f, 0.0f)) * orientation_;
    }

    inline void Placement::AddWorldRotationY(float rotationInRadians)
    {
        orientation_ = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(0.0f, 1.0f, 0.0f)) * orientation_;
    }

    inline void Placement::AddWorldRotationZ(float rotationInRadians)
    {
        orientation_ = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f)) * orientation_;
    }

    inline void Placement::LookAt(const glm::vec3& direction, const glm::vec3& up)
    {
        glm::mat3 orientation;
        orientation[2] = direction;
        orientation[0] = glm::normalize(glm::cross(up, orientation[2]));
        orientation[1] = glm::cross(orientation[2], orientation[0]);
        orientation_ = glm::quat_cast(orientation);
    }

    inline void Placement::LookAt(const glm::vec3& direction)
    {
        LookAt(glm::normalize(direction), GetUpVector());
    }

    inline glm::vec3 Placement::GetForwardVector() const
    {
        return orientation_ * glm::vec3(0.0f, 0.0f, 1.0f);
    }

    inline glm::vec3 Placement::GetLeftVector() const
    {
        return orientation_ * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    inline glm::vec3 Placement::GetUpVector() const
    {
        return orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);
    }
}