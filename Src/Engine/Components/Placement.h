#pragma once

#include "glm/gtc/quaternion.hpp"

#include "Engine/Components/Position.h"



namespace Project001
{
    class Placement : public Position
    {
    public:
        Placement();

        // Position Controls
        // ---------------------------------------------------------------------

        void RevolveAround(const glm::vec3& focalPoint, float angleInRadians, const glm::vec3& normal);

        // Orientation Controls
        // ---------------------------------------------------------------------
        // Rotation follows the right hand rule.

        void SetOrientation(const glm::quat& orientation);
        void SetOrientation(float w, float x, float y, float z);
        const glm::quat& GetOrientation() const;

        void AddRotation(const glm::quat& rotation);

        void AddRelativeRotation(float rotationInRadians, const glm::vec3 axis);

        void AddPitch(float rotationInRadians);
        float GetPitch() const;

        void AddYaw(float rotationInRadians);
        float GetYaw() const;

        void AddRoll(float rotationInRadians);
        float GetRoll() const;

        void AddWorldRotation(float rotationInRadians, const glm::vec3 axis);

        void AddWorldRotationX(float rotationInRadians);
        void AddWorldRotationY(float rotationInRadians);
        void AddWorldRotationZ(float rotationInRadians);

        void LookAt(const glm::vec3& direction, const glm::vec3& up);

    protected:
        // Inherited:
        // glm::vec3 position_;

        glm::quat orientation_; // w, x, y, z

    private:
    };

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

    inline const glm::quat& Placement::GetOrientation() const
    {
        return orientation_;
    }

    inline void Placement::AddRotation(const glm::quat& rotation)
    {
        orientation_ = rotation * orientation_;
    }

    inline void Placement::AddRelativeRotation(float rotationInRadians, const glm::vec3 axis)
    {
        orientation_ = glm::rotate(orientation_, rotationInRadians, axis);
    }

    inline void Placement::AddPitch(float rotationInRadians)
    {
        AddRelativeRotation(rotationInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    inline float Placement::GetPitch() const
    {
        return glm::pitch(orientation_);
    }

    inline void Placement::AddYaw(float rotationInRadians)
    {
        AddRelativeRotation(rotationInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    inline float Placement::GetYaw() const
    {
        return glm::yaw(orientation_);
    }

    inline void Placement::AddRoll(float rotationInRadians)
    {
        AddRelativeRotation(rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    inline float Placement::GetRoll() const
    {
        return glm::roll(orientation_);
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
        orientation_ = glm::quatLookAt(glm::normalize(direction), glm::normalize(up));
    }
}