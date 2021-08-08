#include "Placement.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    Placement::Placement()
        : orientation_(1.0f, 0.0f, 0.0f, 0.0f)
    {}

    Placement::~Placement()
    {}

    void Placement::RevolveAround(const glm::vec3& focalPoint, float angleInRadians, const glm::vec3& normal)
    {
        glm::vec3 focalPointToPosition = position_ - focalPoint;
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), angleInRadians, normal);
        glm::vec3 focalPointToNewPosition = rotation * focalPointToPosition;
        glm::vec3 positionToNewPosition = focalPointToNewPosition - focalPointToPosition;
        position_ += positionToNewPosition;
    }
}