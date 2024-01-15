#pragma once

#include "Components/RenderedMesh.h"



namespace Project001
{
    class RenderedModel : public Placement
    {
    public:
        RenderedModel();

        // Gets applied to RenderedMeshes
        bool GetVisible() const;
        void SetVisible(bool visible);

        // Gets applied to the RenderedMeshes
        const glm::vec3& GetScale() const;
        void SetScale(glm::vec3 scale);

        std::vector<RenderedMesh>& GetRenderedMeshes();
        const std::vector<RenderedMesh>& GetRenderedMeshes() const;
        const std::vector<RenderedMesh>& GetTransformedRenderedMeshes() const;

        bool TransformedMeshesUpToDate() const;

        void CalculateTransformedMeshes();

        // Overwriten:

        void SetPosition(const glm::vec3& position);
        void SetPosition(float x, float y, float z);
        void SetPositionX(float x);
        void SetPositionY(float y);
        void SetPositionZ(float z);

        void AddTranslation(const glm::vec3& translation);
        void AddTranslation(float x, float y, float z);
        void AddTranslationX(float x);
        void AddTranslationY(float y);
        void AddTranslationZ(float z);

        void MoveForward(float translation);
        void MoveBack(float translation);
        void MoveRight(float translation);
        void MoveLeft(float translation);
        void MoveUp(float translation);
        void MoveDown(float translation);

        void RevolveAround(const glm::vec3& focalPoint, float angleInRadians, const glm::vec3& normal);
        void RevolveAroundHorizontally(const glm::vec3& focalPoint, float angleInRadians);

        void ResetOrientation();

        void SetOrientation(const glm::quat& orientation);
        void SetOrientation(float w, float x, float y, float z);

        void AddRotation(const glm::quat& rotation);

        void AddRelativeRotation(float rotationInRadians, const glm::vec3 axis);

        void AddRelativeRotationX(float rotationInRadians);
        void AddRelativeRotationY(float rotationInRadians);
        void AddRelativeRotationZ(float rotationInRadians);

        void AddPitch(float rotationInRadians);
        void AddYaw(float rotationInRadians);
        void AddRoll(float rotationInRadians);

        void AddWorldRotation(float rotationInRadians, const glm::vec3 axis);

        void AddWorldRotationX(float rotationInRadians);
        void AddWorldRotationY(float rotationInRadians);
        void AddWorldRotationZ(float rotationInRadians);

        void LookAt(const glm::vec3& direction, const glm::vec3& up);
        void LookAt(const glm::vec3& direction);

    protected:
        // Inherited:
        // glm::vec3 position_;
        // glm::quat orientation_;

        bool visible_;
        glm::vec3 scale_;

        std::vector<RenderedMesh> renderedMeshes_;

        std::vector<RenderedMesh> transformedRenderedMeshes_;

        bool transformedMeshesUpToDate_;
    };

    inline RenderedModel::RenderedModel()
        : visible_(true)
        , scale_(1.0f, 1.0f, 1.0f)
        , transformedMeshesUpToDate_(false)
    {}

    inline bool RenderedModel::GetVisible() const
    {
        return visible_;
    }

    inline void RenderedModel::SetVisible(bool visible)
    {
        visible_ = visible;
    }

    inline const glm::vec3& RenderedModel::GetScale() const
    {
        return scale_;
    }

    inline void RenderedModel::SetScale(glm::vec3 scale)
    {
        scale_ = scale;
    }

    inline std::vector<RenderedMesh>& RenderedModel::GetRenderedMeshes()
    {
        transformedMeshesUpToDate_ = false;
        return renderedMeshes_;
    }

    inline const std::vector<RenderedMesh>& RenderedModel::GetRenderedMeshes() const
    {
        return renderedMeshes_;
    }

    inline const std::vector<RenderedMesh>& RenderedModel::GetTransformedRenderedMeshes() const
    {
        return transformedRenderedMeshes_;
    }

    inline bool RenderedModel::TransformedMeshesUpToDate() const
    {
        return transformedMeshesUpToDate_;
    }

    inline void RenderedModel::SetPosition(const glm::vec3& position)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetPosition(position);
    }

    inline void RenderedModel::SetPosition(float x, float y, float z)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetPosition(x, y, z);
    }

    inline void RenderedModel::SetPositionX(float x)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetPositionX(x);
    }

    inline void RenderedModel::SetPositionY(float y)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetPositionY(y);
    }

    inline void RenderedModel::SetPositionZ(float z)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetPositionZ(z);
    }

    inline void RenderedModel::AddTranslation(const glm::vec3& translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddTranslation(translation);
    }

    inline void RenderedModel::AddTranslation(float x, float y, float z)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddTranslation(x, y, z);
    }

    inline void RenderedModel::AddTranslationX(float x)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddTranslationX(x);
    }

    inline void RenderedModel::AddTranslationY(float y)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddTranslationY(y);
    }

    inline void RenderedModel::AddTranslationZ(float z)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddTranslationZ(z);
    }

    inline void RenderedModel::MoveForward(float translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::MoveForward(translation);
    }

    inline void RenderedModel::MoveBack(float translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::MoveBack(translation);
    }

    inline void RenderedModel::MoveRight(float translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::MoveRight(translation);
    }

    inline void RenderedModel::MoveLeft(float translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::MoveLeft(translation);
    }

    inline void RenderedModel::MoveUp(float translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::MoveUp(translation);
    }

    inline void RenderedModel::MoveDown(float translation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::MoveDown(translation);
    }

    inline void RenderedModel::RevolveAround(const glm::vec3& focalPoint, float angleInRadians, const glm::vec3& normal)
    {
        transformedMeshesUpToDate_ = false;
        Placement::RevolveAround(focalPoint, angleInRadians, normal);
    }

    inline void RenderedModel::RevolveAroundHorizontally(const glm::vec3& focalPoint, float angleInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::RevolveAroundHorizontally(focalPoint, angleInRadians);
    }

    inline void RenderedModel::ResetOrientation()
    {
        transformedMeshesUpToDate_ = false;
        Placement::ResetOrientation();
    }

    inline void RenderedModel::SetOrientation(const glm::quat& orientation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetOrientation(orientation);
    }

    inline void RenderedModel::SetOrientation(float w, float x, float y, float z)
    {
        transformedMeshesUpToDate_ = false;
        Placement::SetOrientation(w, x, y, z);
    }

    inline void RenderedModel::AddRotation(const glm::quat& rotation)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddRotation(rotation);
    }

    inline void RenderedModel::AddRelativeRotation(float rotationInRadians, const glm::vec3 axis)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddRelativeRotation(rotationInRadians, axis);
    }

    inline void RenderedModel::AddRelativeRotationX(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddRelativeRotationX(rotationInRadians);
    }

    inline void RenderedModel::AddRelativeRotationY(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddRelativeRotationY(rotationInRadians);
    }

    inline void RenderedModel::AddRelativeRotationZ(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddRelativeRotationZ(rotationInRadians);
    }

    inline void RenderedModel::AddPitch(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddPitch(rotationInRadians);
    }

    inline void RenderedModel::AddYaw(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddYaw(rotationInRadians);
    }

    inline void RenderedModel::AddRoll(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddRoll(rotationInRadians);
    }

    inline void RenderedModel::AddWorldRotation(float rotationInRadians, const glm::vec3 axis)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddWorldRotation(rotationInRadians, axis);
    }

    inline void RenderedModel::AddWorldRotationX(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddWorldRotationX(rotationInRadians);
    }

    inline void RenderedModel::AddWorldRotationY(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddWorldRotationY(rotationInRadians);
    }

    inline void RenderedModel::AddWorldRotationZ(float rotationInRadians)
    {
        transformedMeshesUpToDate_ = false;
        Placement::AddWorldRotationZ(rotationInRadians);
    }

    inline void RenderedModel::LookAt(const glm::vec3& direction, const glm::vec3& up)
    {
        transformedMeshesUpToDate_ = false;
        Placement::LookAt(direction, up);
    }

    inline void RenderedModel::LookAt(const glm::vec3& direction)
    {
        transformedMeshesUpToDate_ = false;
        Placement::LookAt(direction);
    }
}