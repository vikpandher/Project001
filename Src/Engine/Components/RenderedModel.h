#pragma once

#include "Engine/Components/Placement.h"
#include "Engine/MeshData.h"



namespace Project001
{
    class RenderedModel : public Placement
    {
    public:
        enum class RenderedModelType
        {
            RENDERED_MODEL_TYPE_NOT_LOADED,
            RENDERED_MODEL_TYPE_LOADED_CPU_SIDE,
            RENDERED_MODEL_TYPE_LOADED_GPU_SIDE
        };

        RenderedModel();

        // RenderedModelType is set by the following functions:
        // SetMeshDataPtr
        //     * Sets it to RENDERED_MODEL_TYPE_LOADED_CPU_SIDE
        // SetMeshId
        //     * Sets it to RENDERED_MODEL_TYPE_LOADED_GPU_SIDE

        RenderedModelType GetRenderedModelType() const;

        bool IsVisible() const;
        void SetVisibility(bool visible);

        void SetCameraMask(uint32_t cameraMask);
        uint32_t GetCameraMask() const;

        const MeshData* GetMeshDataPtr() const;
        void SetMeshDataPtr(const MeshData* meshData);

        unsigned int GetMeshId() const;
        void SetMeshId(unsigned int meshId);

        float GetMaxRadius() const;
        void SetMaxRadius(float maxRadius);

        unsigned int GetTextureId() const;
        void SetTextureId(unsigned int textureId);

        unsigned int GetSpecularId() const;
        void SetSpecularId(unsigned int specularId);

        const glm::vec3& GetScale() const;
        void SetScale(const glm::vec3& scale);
        void SetScale(float x, float y, float z);
        void SetScaleX(float x);
        void SetScaleY(float y);
        void SetScaleZ(float z);

        const glm::vec4& GetColor() const;
        void SetColor(const glm::vec4& color);
        void SetColor(float r, float g, float b, float a);
        void SetColorRGB(float r, float g, float b);

        float GetShininess() const;
        void SetShininess(float shininess);

        bool GetTranslucent() const;
        void SetTranslucent(bool translucent);

        bool GetLit() const;
        void SetLit(bool lit);

    protected:
        // Inherited:
        // glm::vec3 position_;
        // glm::quat orientation_;

        RenderedModelType renderedModelType_;

        bool visible_;

        uint32_t cameraMask_;

        const MeshData* meshDataPtr_; // Used when RENDERED_MODEL_TYPE_LOADED_CPU_SIDE

        unsigned int meshId_;         // Used when RENDERED_MODEL_TYPE_LOADED_GPU_SIDE
        float maxRadius_;             // Used when RENDERED_MODEL_TYPE_LOADED_GPU_SIDE

        unsigned int textureId_;
        unsigned int specularId_;
        glm::vec3 scale_;
        float shininess_; // 32.0f looks good
        glm::vec4 color_;
        bool translucent_;
        bool lit_;
    };

    inline RenderedModel::RenderedModel()
        : renderedModelType_(RenderedModelType::RENDERED_MODEL_TYPE_NOT_LOADED)
        , visible_(true)
        , cameraMask_(0b00000000000000000000000000000001)
        , meshDataPtr_(nullptr)
        , meshId_((unsigned int)-1)
        , maxRadius_(0.0f)
        , textureId_((unsigned int)-1)
        , specularId_((unsigned int)-1)
        , scale_(1.0f, 1.0f, 1.0f)
        , color_(1.0f, 1.0f, 1.0f, 1.0f)
        , shininess_(0.0f)
        , translucent_(false)
        , lit_(true)
    {}

    inline RenderedModel::RenderedModelType RenderedModel::GetRenderedModelType() const
    {
        return renderedModelType_;
    }

    inline bool RenderedModel::IsVisible() const
    {
        return visible_;
    }

    inline void RenderedModel::SetVisibility(bool visible)
    {
        visible_ = visible;
    }

    inline void RenderedModel::SetCameraMask(uint32_t cameraMask)
    {
        cameraMask_ = cameraMask;
    }

    inline uint32_t RenderedModel::GetCameraMask() const
    {
        return cameraMask_;
    }

    inline const MeshData* RenderedModel::GetMeshDataPtr() const
    {
        return meshDataPtr_;
    }

    inline void RenderedModel::SetMeshDataPtr(const MeshData* meshDataPtr)
    {
        renderedModelType_ = RenderedModelType::RENDERED_MODEL_TYPE_LOADED_CPU_SIDE;
        meshDataPtr_ = meshDataPtr;
    }

    inline unsigned int RenderedModel::GetMeshId() const
    {
        return meshId_;
    }

    inline void RenderedModel::SetMeshId(unsigned int meshId)
    {
        renderedModelType_ = RenderedModelType::RENDERED_MODEL_TYPE_LOADED_GPU_SIDE;
        meshId_ = meshId;
    }

    inline float RenderedModel::GetMaxRadius() const
    {
        return maxRadius_;
    }

    inline void RenderedModel::SetMaxRadius(float maxRadius)
    {
        maxRadius_ = maxRadius;
    }

    inline unsigned int RenderedModel::GetTextureId() const
    {
        return textureId_;
    }

    inline void RenderedModel::SetTextureId(unsigned int textureId)
    {
        textureId_ = textureId;
    }

    inline unsigned int RenderedModel::GetSpecularId() const
    {
        return specularId_;
    }

    inline void RenderedModel::SetSpecularId(unsigned int specularId)
    {
        specularId_ = specularId;
    }

    inline const glm::vec3& RenderedModel::GetScale() const
    {
        return scale_;
    }

    inline void RenderedModel::SetScale(const glm::vec3& scale)
    {
        scale_ = scale;
    }

    inline void RenderedModel::SetScale(float x, float y, float z)
    {
        scale_.x = x;
        scale_.y = y;
        scale_.z = z;
    }

    inline void RenderedModel::SetScaleX(float x)
    {
        scale_.x = x;
    }

    inline void RenderedModel::SetScaleY(float y)
    {
        scale_.y = y;
    }

    inline void RenderedModel::SetScaleZ(float z)
    {
        scale_.z = z;
    }

    inline const glm::vec4& RenderedModel::GetColor() const
    {
        return color_;
    }

    inline void RenderedModel::SetColor(const glm::vec4& color)
    {
        color_ = color;
    }

    inline void RenderedModel::SetColor(float r, float g, float b, float a)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
        color_.a = a;
    }

    inline void RenderedModel::SetColorRGB(float r, float g, float b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

    inline float RenderedModel::GetShininess() const
    {
        return shininess_;
    }

    inline void RenderedModel::SetShininess(float shininess)
    {
        shininess_ = shininess;
    }

    inline bool RenderedModel::GetTranslucent() const
    {
        return translucent_;
    }

    inline void RenderedModel::SetTranslucent(bool translucent)
    {
        translucent_ = translucent;
    }

    inline bool RenderedModel::GetLit() const
    {
        return lit_;
    }

    inline void RenderedModel::SetLit(bool lit)
    {
        lit_ = lit;
    }
}