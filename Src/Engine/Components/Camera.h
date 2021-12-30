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

    inline Camera::Camera()
        : turnedOn_(false)
        , cameraProjection_(CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        , fieldOfVision_(45.0f)
        , aspectRatio_(1.0f)
        , leftCutoff_(-5.0f)
        , rightCutoff_(5.0f)
        , bottomCutoff_(-5.0f)
        , topCutoff_(5.0f)
        , nearCutoff_(0.1f)
        , farCutoff_(50.0f)
    {}

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

    inline glm::mat4 Camera::GetViewMatrix() const
    {
        glm::vec3 eye = position_;
        glm::vec3 center = position_ + GetForwardVector();
        glm::vec3 up = GetUpVector();
        return glm::lookAt(eye, center, up);
    }

    inline glm::mat4 Camera::GetProjectionMatrix() const
    {
        if (cameraProjection_ == CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        {
            return glm::perspective(fieldOfVision_, aspectRatio_, nearCutoff_, farCutoff_);
        }
        else
        {
            return glm::ortho(leftCutoff_, rightCutoff_, bottomCutoff_, topCutoff_, nearCutoff_, farCutoff_);
        }
    }
}