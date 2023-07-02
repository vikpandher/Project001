#pragma once

#include "Engine/Components/Placement.h"



namespace Project001
{
    // plane normals face into the center of the frustum
    // normals are unit vectors
    // distances are from the origin
    struct FrustumPlanes
    {
        glm::vec3 leftPlaneNormal_;
        float leftPlaneDistance_;

        glm::vec3 rightPlaneNormal_;
        float rightPlaneDistance_;

        glm::vec3 bottomPlaneNormal_;
        float bottomPlaneDistance_;

        glm::vec3 topPlaneNormal_;
        float topPlaneDistance_;

        glm::vec3 nearPlaneNormal_;
        float nearPlaneDistance_;

        glm::vec3 farPlaneNormal_;
        float farPlaneDistance_;
    };

    class Camera : public Placement
    {
    public:
        Camera();

        bool IsTurnedOn() const;
        void TurnOn();
        void TurnOff();

        // Other View Controls
        // ---------------------------------------------------------------------

        bool IsProjectionOrthographic() const;
        void SetProjectionToOrthographic();

        bool IsProjectionPerspective() const;
        void SetProjectionToPerspective();

        float GetFieldOfVision() const;
        void SetFieldOfVision(float angleInRadians);

        float GetAspectRatio() const;
        void SetAspectRatio(float aspectRatio);

        float GetLeftCutoff() const;
        void SetLeftCutoff(float leftCutoff);

        float GetRightCutoff() const;
        void SetRightCutoff(float rightCutoff);

        float GetBottomCutoff() const;
        void SetBottomCutoff(float bottomCutoff);

        float GetTopCutoff() const;
        void SetTopCutoff(float topCutoff);

        float GetNearCutoff() const;
        void SetNearCutoff(float nearCutoff);

        float GetFarCutoff() const;
        void SetFarCutoff(float farCutoff);

        // 
        // ---------------------------------------------------------------------

        glm::mat4 GetViewMatrix() const;

        glm::mat4 GetProjectionMatrix() const;

        void GetProjectionFrustumPlanes(FrustumPlanes& frustumPlanes) const;

        glm::vec2 ConvertPointFromViewportToOrthoWorld(int viewportWidth, int viewportHeight, glm::vec2 windowPoint) const;

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
    };

    inline bool Camera::IsTurnedOn() const
    {
        return turnedOn_;
    }

    inline void Camera::TurnOn()
    {
        turnedOn_ = true;
    }

    inline void Camera::TurnOff()
    {
        turnedOn_ = false;
    }

    inline bool Camera::IsProjectionOrthographic() const
    {
        return cameraProjection_ == CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC;
    }

    inline void Camera::SetProjectionToOrthographic()
    {
        cameraProjection_ = CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC;
    }

    inline bool Camera::IsProjectionPerspective() const
    {
        return cameraProjection_ == CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;
    }

    inline void Camera::SetProjectionToPerspective()
    {
        cameraProjection_ = CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;
    }

    inline float Camera::GetFieldOfVision() const
    {
        return fieldOfVision_;
    }

    inline void Camera::SetFieldOfVision(float angleInRadians)
    {
        fieldOfVision_ = angleInRadians;
    }

    inline float Camera::GetAspectRatio() const
    {
        return aspectRatio_;
    }

    inline void Camera::SetAspectRatio(float aspectRatio)
    {
        aspectRatio_ = aspectRatio;
    }

    inline float Camera::GetLeftCutoff() const
    {
        return leftCutoff_;
    }

    inline void Camera::SetLeftCutoff(float leftCutoff)
    {
        leftCutoff_ = leftCutoff;
    }

    inline float Camera::GetRightCutoff() const
    {
        return rightCutoff_;
    }

    inline void Camera::SetRightCutoff(float rightCutoff)
    {
        rightCutoff_ = rightCutoff;
    }

    inline float Camera::GetBottomCutoff() const
    {
        return bottomCutoff_;
    }

    inline void Camera::SetBottomCutoff(float bottomCutoff)
    {
        bottomCutoff_ = bottomCutoff;
    }

    inline float Camera::GetTopCutoff() const
    {
        return topCutoff_;
    }

    inline void Camera::SetTopCutoff(float topCutoff)
    {
        topCutoff_ = topCutoff;
    }

    inline float Camera::GetNearCutoff() const
    {
        return nearCutoff_;
    }

    inline void Camera::SetNearCutoff(float nearCutoff)
    {
        nearCutoff_ = nearCutoff;
    }

    inline float Camera::GetFarCutoff() const
    {
        return farCutoff_;
    }

    inline void Camera::SetFarCutoff(float farCutoff)
    {
        farCutoff_ = farCutoff;
    }
}