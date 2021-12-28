#pragma once

#include "Engine/Components/Placement.h"



namespace Project001
{
    struct Camera : Placement
    {
        enum class CameraProjection
        {
            CAMERA_PROJECTION_PERSPECTIVE,
            CAMERA_PROJECTION_ORTHOGRAPHIC
        };

        Camera();

        glm::mat4 GetViewMatrix() const;

        glm::mat4 GetProjectionMatrix() const;

        // Inherited:
        // glm::vec3 position;
        // glm::quat orientation;

        bool turnedOn;

        CameraProjection cameraProjection;

        float fieldOfVision; // only used in perspective projection
        float aspectRatio; // (width/height) only used in perspective projection
        float leftCutoff;  // only used in othographic projection
        float rightCutoff; // only used in othographic projection
        float bottomCutoff; // only used in othographic projection
        float topCutoff; // only used in othographic projection
        float nearCutoff;
        float farCutoff;
    };

    inline Camera::Camera()
        : turnedOn(false)
        , cameraProjection(CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        , fieldOfVision(45.0f)
        , aspectRatio(1.0f)
        , leftCutoff(-5.0f)
        , rightCutoff(5.0f)
        , bottomCutoff(-5.0f)
        , topCutoff(5.0f)
        , nearCutoff(0.1f)
        , farCutoff(50.0f)
    {}

    inline glm::mat4 Camera::GetViewMatrix() const
    {
        glm::vec3 eye = position;
        glm::vec3 center = position + GetForwardVector();
        glm::vec3 up = GetUpVector();
        return glm::lookAt(eye, center, up);
    }

    inline glm::mat4 Camera::GetProjectionMatrix() const
    {
        if (cameraProjection == CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
        {
            return glm::perspective(fieldOfVision, aspectRatio, nearCutoff, farCutoff);
        }
        else
        {
            return glm::ortho(leftCutoff, rightCutoff, bottomCutoff, topCutoff, nearCutoff, farCutoff);
        }
    }
}