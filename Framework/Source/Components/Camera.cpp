// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-21

#include "Components/Camera.h"

#include "Math/MathUtilities.h"
#include "Math/Overlap3D.h"



namespace Project001
{
    Camera::Camera()
        : turnedOn_(false)
        , cameraMask_(0b00000000000000000000000000000001)
        , priorityValue_(0)
        , cameraProjection_(CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        , depthTestEnabled_(true)
        , fieldOfView_(glm::radians(45.0f))
        , aspectRatio_(1.0f)
        , leftCutoff_(-5.0f)
        , rightCutoff_(5.0f)
        , bottomCutoff_(-5.0f)
        , topCutoff_(5.0f)
        , nearCutoff_(0.1f)
        , farCutoff_(50.0f)
        , cameraViewportX_(0.0f)
        , cameraViewportY_(0.0f)
        , cameraViewportWidth_(1.0f)
        , cameraViewportHeight_(1.0f)
    {}

    glm::mat4 Camera::GetViewMatrix() const
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

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        if (cameraProjection_ == CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        {
            return glm::perspective(fieldOfView_, aspectRatio_, nearCutoff_, farCutoff_);
        }
        else
        {
            return glm::ortho(leftCutoff_, rightCutoff_, bottomCutoff_, topCutoff_, nearCutoff_, farCutoff_);
        }
    }

    void Camera::GetProjectionFrustumCorners(glm::vec3(&corners)[8]) const
    {
        float tan_half = std::tan(fieldOfView_ * 0.5f);

        float nh = tan_half * nearCutoff_;
        float nw = nh * aspectRatio_;
        float fh = tan_half * farCutoff_;
        float fw = fh * aspectRatio_;

        corners[0] = glm::vec3(-nw, nh, nearCutoff_);  // top left
        corners[1] = glm::vec3(nw, nh, nearCutoff_);   // top right
        corners[2] = glm::vec3(nw, -nh, nearCutoff_);  // bottom right
        corners[3] = glm::vec3(-nw, -nh, nearCutoff_); // bottom left
        corners[4] = glm::vec3(-fw, fh, farCutoff_);   // top left
        corners[5] = glm::vec3(fw, fh, farCutoff_);    // top right
        corners[6] = glm::vec3(fw, -fh, farCutoff_);   // bottom right
        corners[7] = glm::vec3(-fw, -fh, farCutoff_);  // bottom left
    }

    void Camera::GetProjectionFrustumPlanes(FrustumPlanes& frustumPlanes) const
    {
        if (cameraProjection_ == CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        {
            float tanHalfFov = glm::tan(fieldOfView_ / 2.0f);
            float nearHalfHeight = tanHalfFov * nearCutoff_;
            float nearHalfWidth = nearHalfHeight * aspectRatio_;
            float farHalfHeight = tanHalfFov * farCutoff_;
            float farHalfWidth = farHalfHeight * aspectRatio_;

            glm::vec3 forwardVector = GetForwardVector();;
            glm::vec3 leftVector = GetLeftVector();
            glm::vec3 upVector = GetUpVector();

            glm::vec3 nearCenter = position_ + forwardVector * nearCutoff_;
            glm::vec3 farCenter = position_ + forwardVector * farCutoff_;

            glm::vec3 nearTopLeft = nearCenter + leftVector * nearHalfWidth + upVector * nearHalfHeight;
            glm::vec3 nearTopRight = nearCenter - leftVector * nearHalfWidth + upVector * nearHalfHeight;
            glm::vec3 nearBottomLeft = nearCenter + leftVector * nearHalfWidth - upVector * nearHalfHeight;
            glm::vec3 nearBottomRight = nearCenter - leftVector * nearHalfWidth - upVector * nearHalfHeight;

            frustumPlanes.nearPlaneNormal_ = forwardVector;
            frustumPlanes.nearPlaneDistance_ = glm::dot(frustumPlanes.nearPlaneNormal_, nearCenter);

            frustumPlanes.farPlaneNormal_ = -forwardVector;
            frustumPlanes.farPlaneDistance_ = glm::dot(frustumPlanes.farPlaneNormal_, farCenter);

            frustumPlanes.leftPlaneNormal_ = glm::normalize(glm::cross(nearBottomLeft - position_, nearTopLeft - nearBottomLeft));
            frustumPlanes.leftPlaneDistance_ = glm::dot(frustumPlanes.leftPlaneNormal_, position_);

            frustumPlanes.rightPlaneNormal_ = glm::normalize(glm::cross(nearTopRight - position_, nearBottomRight - nearTopRight));
            frustumPlanes.rightPlaneDistance_ = glm::dot(frustumPlanes.rightPlaneNormal_, position_);

            frustumPlanes.bottomPlaneNormal_ = glm::normalize(glm::cross(nearBottomRight - position_, nearBottomLeft - nearBottomRight));
            frustumPlanes.bottomPlaneDistance_ = glm::dot(frustumPlanes.bottomPlaneNormal_, position_);

            frustumPlanes.topPlaneNormal_ = glm::normalize(glm::cross(nearTopLeft - position_, nearTopRight - nearTopLeft));
            frustumPlanes.topPlaneDistance_ = glm::dot(frustumPlanes.topPlaneNormal_, position_);
        }
        else if (cameraProjection_ == CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC)
        {
            glm::vec3 forwardVector = GetForwardVector();;
            glm::vec3 leftVector = GetLeftVector();
            glm::vec3 upVector = GetUpVector();

            glm::vec3 nearCenter = position_ + forwardVector * nearCutoff_;
            glm::vec3 farCenter = position_ + forwardVector * farCutoff_;
            glm::vec3 leftCenter = position_ - leftVector * leftCutoff_;
            glm::vec3 rightCenter = position_ - leftVector * rightCutoff_;
            glm::vec3 bottomCenter = position_ + upVector * bottomCutoff_;
            glm::vec3 topCenter = position_ + upVector * topCutoff_;

            frustumPlanes.nearPlaneNormal_ = forwardVector;
            frustumPlanes.nearPlaneDistance_ = glm::dot(frustumPlanes.nearPlaneNormal_, nearCenter);

            frustumPlanes.farPlaneNormal_ = -forwardVector;
            frustumPlanes.farPlaneDistance_ = glm::dot(frustumPlanes.farPlaneNormal_, farCenter);

            frustumPlanes.leftPlaneNormal_ = -leftVector;
            frustumPlanes.leftPlaneDistance_ = glm::dot(frustumPlanes.leftPlaneNormal_, leftCenter);

            frustumPlanes.rightPlaneNormal_ = leftVector;
            frustumPlanes.rightPlaneDistance_ = glm::dot(frustumPlanes.rightPlaneNormal_, rightCenter);

            frustumPlanes.bottomPlaneNormal_ = upVector;
            frustumPlanes.bottomPlaneDistance_ = glm::dot(frustumPlanes.bottomPlaneNormal_, bottomCenter);

            frustumPlanes.topPlaneNormal_ = -upVector;
            frustumPlanes.topPlaneDistance_ = glm::dot(frustumPlanes.topPlaneNormal_, topCenter);
        }
    }

    glm::vec3 Camera::ConvertPointFromNormalizedViewportToNearPlane(const glm::vec2& normalizedViewportPoint) const
    {
        glm::vec2 normalizedCameraViewportPoint;
        normalizedCameraViewportPoint.x = (normalizedViewportPoint.x - cameraViewportX_) / cameraViewportWidth_;
        normalizedCameraViewportPoint.y = (normalizedViewportPoint.y - cameraViewportY_) / cameraViewportHeight_;

        // This bounds the point inside the camera viewport; it's commented out
        // normalizedCameraViewportPoint = glm::clamp(normalizedCameraViewportPoint, glm::vec2(0.0f), glm::vec2(1.0f));

        glm::vec2 ndc;
        ndc.x = 1.0f - normalizedCameraViewportPoint.x * 2.0f;
        ndc.y = normalizedCameraViewportPoint.y * 2.0f - 1.0f;

        glm::vec3 cameraPoint;

        if (cameraProjection_ == CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC)
        {
            // glm::mix does linear interpolation
            cameraPoint.x = glm::mix(leftCutoff_, rightCutoff_, (ndc.x + 1.0f) * 0.5f);
            cameraPoint.y = glm::mix(bottomCutoff_, topCutoff_, (ndc.y + 1.0f) * 0.5f);
            cameraPoint.z = nearCutoff_;
        }
        else // Perspective Projection
        {
            float tanHalfFov = tanf(fieldOfView_ * 0.5f);
            float nh = nearCutoff_ * tanHalfFov; // half height at near plane
            float nw = nh * aspectRatio_;        // half width at near plane

            cameraPoint.x = ndc.x * nw;
            cameraPoint.y = ndc.y * nh;
            cameraPoint.z = nearCutoff_;
        }

        return position_ + orientation_ * cameraPoint;
    }

    bool Camera::RaycastPointFromNormalizedViewportToPane(
        const glm::vec2& normalizedViewportPoint,
        const glm::vec3& planeNormal,
        const float& planeDistance,
        glm::vec3& hitPoint,
        glm::vec3& hitNormal) const
    {
        glm::vec3 nearPlaneCursorPosition = ConvertPointFromNormalizedViewportToNearPlane(normalizedViewportPoint);

        glm::vec3 cameraPosition;
        glm::vec3 cursorDirection;
        if (cameraProjection_ == Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        {
            cameraPosition = position_;
            cursorDirection = nearPlaneCursorPosition - cameraPosition;
        }
        else
        {
            cameraPosition = nearPlaneCursorPosition;
            cursorDirection = GetForwardVector();
        }

        float hitScalar;
        bool intersected = Project001::Raycast3D_Plane(
            cameraPosition,
            cursorDirection,
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            hitScalar,
            hitNormal);

        hitPoint = cameraPosition + cursorDirection * hitScalar;

        return intersected;
    }

    glm::vec2 Camera::ConvertPointFromViewportToOrthographicTopDownPoint(
        int viewportWidth,
        int viewportHeight,
        const glm::vec2& viewportPoint) const
    {
        float cutoffWidth = rightCutoff_ - leftCutoff_;
        float cutoffHeight = topCutoff_ - bottomCutoff_;
        glm::vec2 orthoPoint(
            cutoffWidth * (float)viewportPoint.x / (float)viewportWidth - rightCutoff_,
            cutoffHeight * (float)viewportPoint.y / (float)viewportHeight - topCutoff_);
        orthoPoint = Rotate2DVector(orthoPoint, glm::pi<float>() + GetRoll());
        orthoPoint.x += position_.x;
        orthoPoint.y += position_.y;
        return orthoPoint;
    }
}