#pragma once

#include "Engine/Components/Placement.h"



namespace Project001
{
    class Camera : public Placement
    {
    public:
        Camera();

        void TurnOn();
        void TurnOff();
        bool IsTurnedOn() const;

        // Position Controls
        // ---------------------------------------------------------------------

        void MoveForward(float translation);
        void MoveBack(float translation);
        void MoveRight(float translation);
        void MoveLeft(float translation);
        void MoveUp(float translation);
        void MoveDown(float translation);

        void RevolveAroundHorizontally(const glm::vec3& focalPoint, float angleInRadians);

        // Orientation Controls
        // ---------------------------------------------------------------------
        // Camera is oriented facing -z, so "Roll" parent functions are hidden

        void AddRoll(float rotationInRadians);
        float GetRoll() const;

        void LookAt(const glm::vec3& direction);

        glm::vec3 GetForwardVector() const;
        glm::vec3 GetRightVector() const;
        glm::vec3 GetUpVector() const;

        // Other View Controls
        // ---------------------------------------------------------------------

        void SetProjectionToOrthographic();
        void SetProjectionToPerspective();
        bool IsProjectionOrthographic() const;
        bool IsProjectionPerspective() const;

        void SetFieldOfVision(float angleInRadians);
        float GetFieldOfVision() const;

        void SetAspectRatio(float aspectRatio);
        float GetAspectRatio() const;

        void SetLeftCutoff(float leftCutoff);
        float GetLeftCutoff() const;

        void SetRightCutoff(float rightCutoff);
        float GetRightCutoff() const;

        void SetBottomCutoff(float bottomCutoff);
        float GetBottomCutoff() const;

        void SetTopCutoff(float topCutoff);
        float GetTopCutoff() const;

        void SetNearCutoff(float nearCutoff);
        float GetNearCutoff() const;

        void SetFarCutoff(float farCutoff);
        float GetFarCutoff() const;

        // 
        // ---------------------------------------------------------------------

        glm::mat4 GetViewMatrix() const;

        glm::mat4 GetProjectionMatrix() const;

    protected:
        // Inherited:
        // glm::vec3 position_;
        // glm::quat orientation_;

        enum class CameraProjection
        {
            CAMERA_PROJECTION_PERSPECTIVE,
            CAMERA_PROJECTION_ORTHOGRAPHIC
        };

        bool turnedOn_;

        CameraProjection cameraProjection_;

        float fieldOfVision_; // only used in perspective projection
        float aspectRatio_; // (width/height) only used in perspective projection
        float leftCutoff_;  // only used in othographic projection
        float rightCutoff_; // only used in othographic projection
        float bottomCutoff_; // only used in othographic projection
        float topCutoff_; // only used in othographic projection
        float nearCutoff_;
        float farCutoff_;

    private:
    };

    inline void Camera::TurnOn()
    {
        turnedOn_ = true;
    }

    inline void Camera::TurnOff()
    {
        turnedOn_ = false;
    }

    inline bool Camera::IsTurnedOn() const
    {
        return turnedOn_;
    }

    inline void Camera::MoveForward(float translation)
    {
        AddTranslation(translation * GetForwardVector());
    }

    inline void Camera::MoveBack(float translation)
    {
        AddTranslation(-1.0f * translation * GetForwardVector());
    }

    inline void Camera::MoveRight(float translation)
    {
        AddTranslation(translation * GetRightVector());
    }

    inline void Camera::MoveLeft(float translation)
    {
        AddTranslation(-1.0f * translation * GetRightVector());
    }

    inline void Camera::MoveUp(float translation)
    {
        AddTranslation(translation * GetUpVector());
    }

    inline void Camera::MoveDown(float translation)
    {
        AddTranslation(-1.0f * translation * GetUpVector());
    }

    inline void Camera::RevolveAroundHorizontally(const glm::vec3& focalPoint, float angleInRadians)
    {
        RevolveAround(focalPoint, angleInRadians, GetUpVector());
    }

    inline void Camera::AddRoll(float rotationInRadians)
    {
        orientation_ = glm::rotate(orientation_, rotationInRadians, glm::vec3(0.0f, 0.0f, -1.0f)); // camera looks towards -z
    }

    inline float Camera::GetRoll() const
    {
        return -1.0f * glm::roll(orientation_); // camera looks towards -z
    }

    inline void Camera::LookAt(const glm::vec3& direction)
    {
        orientation_ = glm::quatLookAt(glm::normalize(direction), GetUpVector());
    }

    inline glm::vec3 Camera::GetForwardVector() const
    {
        return orientation_ * glm::vec3(0.0f, 0.0f, -1.0f); // camera looks toward -z
    }

    inline glm::vec3 Camera::GetRightVector() const
    {
        return orientation_ * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    inline glm::vec3 Camera::GetUpVector() const
    {
        return orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    inline void Camera::SetProjectionToOrthographic()
    {
        cameraProjection_ = CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC;
    }

    inline void Camera::SetProjectionToPerspective()
    {
        cameraProjection_ = CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;
    }

    inline bool Camera::IsProjectionOrthographic() const
    {
        return cameraProjection_ == CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC;
    }

    inline bool Camera::IsProjectionPerspective() const
    {
        return cameraProjection_ == CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;
    }

    inline void Camera::SetFieldOfVision(float angleInRadians)
    {
        fieldOfVision_ = angleInRadians;
    }

    inline float Camera::GetFieldOfVision() const
    {
        return fieldOfVision_;
    }

    inline void Camera::SetAspectRatio(float aspectRatio)
    {
        aspectRatio_ = aspectRatio;
    }

    inline float Camera::GetAspectRatio() const
    {
        return aspectRatio_;
    }

    inline void Camera::SetLeftCutoff(float leftCutoff)
    {
        leftCutoff_ = leftCutoff;
    }

    inline float Camera::GetLeftCutoff() const
    {
        return leftCutoff_;
    }

    inline void Camera::SetRightCutoff(float rightCutoff)
    {
        rightCutoff_ = rightCutoff;
    }

    inline float Camera::GetRightCutoff() const
    {
        return rightCutoff_;
    }

    inline void Camera::SetBottomCutoff(float bottomCutoff)
    {
        bottomCutoff_ = bottomCutoff;
    }

    inline float Camera::GetBottomCutoff() const
    {
        return bottomCutoff_;
    }

    inline void Camera::SetTopCutoff(float topCutoff)
    {
        topCutoff_ = topCutoff;
    }

    inline float Camera::GetTopCutoff() const
    {
        return topCutoff_;
    }

    inline void Camera::SetNearCutoff(float nearCutoff)
    {
        nearCutoff_ = nearCutoff;
    }

    inline float Camera::GetNearCutoff() const
    {
        return nearCutoff_;
    }

    inline void Camera::SetFarCutoff(float farCutoff)
    {
        farCutoff_ = farCutoff;
    }

    inline float Camera::GetFarCutoff() const
    {
        return farCutoff_;
    }
}