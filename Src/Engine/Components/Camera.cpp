#include "Camera.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    Camera::Camera()
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

    glm::mat4 Camera::GetViewMatrix() const
    {
        // glm::quat flippedOrientation = orientation_;
        // flippedOrientation.z *= -1.0f;
        // glm::mat4 rotationMatrix = glm::mat4_cast(flippedOrientation);
        // glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position_);
        // glm::mat4 inverseViewMatrix = translationMatrix * rotationMatrix;
        // return glm::inverse(inverseViewMatrix);

        glm::vec3 eye = position_;
        glm::vec3 center = position_ + GetForwardVector();
        glm::vec3 up = GetUpVector();
        return glm::lookAt(eye, center, up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const
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