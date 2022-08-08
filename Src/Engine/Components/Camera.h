#pragma once

#include "Engine/Components/Placement.h"
#include "Engine/Math/VectorUtilities.h"



namespace Project001
{
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

    inline glm::mat4 Camera::GetViewMatrix() const
    {
        // const glm::vec3& eye = position_;
        // const glm::vec3& r = -GetLeftVector();
        // const glm::vec3& u = GetUpVector();
        // const glm::vec3& f = GetForwardVector();
        // glm::mat4 lookAtMatrix(1);
        // lookAtMatrix[0][0] = r.x;
        // lookAtMatrix[1][0] = r.y;
        // lookAtMatrix[2][0] = r.z;
        // lookAtMatrix[0][1] = u.x;
        // lookAtMatrix[1][1] = u.y;
        // lookAtMatrix[2][1] = u.z;
        // lookAtMatrix[0][2] = -f.x;
        // lookAtMatrix[1][2] = -f.y;
        // lookAtMatrix[2][2] = -f.z;
        // lookAtMatrix[3][0] = -glm::dot(r, eye);
        // lookAtMatrix[3][1] = -glm::dot(u, eye);
        // lookAtMatrix[3][2] = glm::dot(f, eye);
        // return lookAtMatrix;

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

    inline glm::vec2 Camera::ConvertPointFromViewportToOrthoWorld(int viewportWidth, int viewportHeight, glm::vec2 windowPoint) const
    {
        float cutoffWidth = rightCutoff_ - leftCutoff_;
        float cutoffHeight = topCutoff_ - bottomCutoff_;
        glm::vec2 orthoPoint(
            cutoffWidth * (float)windowPoint.x / (float)viewportWidth - rightCutoff_,
            cutoffHeight * (float)windowPoint.y / (float)viewportHeight - topCutoff_);
        orthoPoint = Rotate2DVector(orthoPoint, glm::pi<float>() + GetRoll());
        orthoPoint.x += position_.x;
        orthoPoint.y += position_.y;
        return orthoPoint;
    }
}