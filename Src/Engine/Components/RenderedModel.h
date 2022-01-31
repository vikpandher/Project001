#pragma once

#include "Engine/Components/Placement.h"



namespace Project001
{
    class RenderedModel : public Placement
    {
    public:
        RenderedModel();

        void SetVisibility(bool visible);
        bool IsVisible() const;

        void SetMeshIndex(unsigned int meshIndex);
        unsigned int GetMeshIndex() const;

        void SetTextureIndex(unsigned int textureIndex);
        unsigned int GetTextureIndex() const;

        void SetSpecularIndex(unsigned int specularIndex);
        unsigned int GetSpecularIndex() const;

        void SetScale(const glm::vec3& scale);
        void SetScale(float x, float y, float z);
        void SetScaleX(float x);
        void SetScaleY(float y);
        void SetScaleZ(float z);
        const glm::vec3& GetScale() const;

        void SetColor(const glm::vec4& color);
        void SetColor(float r, float g, float b, float a);
        void SetColorRGB(float r, float g, float b);
        const glm::vec4& GetColor() const;

        void SetShininess(float shininess);
        float GetShininess() const;

        void SetTranslucent(bool translucent);
        bool GetTranslucent() const;

        void SetLit(bool lit);
        bool GetLit() const;

    protected:
        // Inherited:
        // glm::vec3 position_;
        // glm::quat orientation_;

        bool visible_;
        unsigned int meshIndex_;
        unsigned int textureIndex_;
        unsigned int specularIndex_;
        glm::vec3 scale_;
        float shininess_; // 32.0f looks good
        glm::vec4 color_;
        bool translucent_;
        bool lit_;
    };

    inline RenderedModel::RenderedModel()
        : visible_(true)
        , meshIndex_((unsigned int)-1)
        , textureIndex_((unsigned int)-1)
        , specularIndex_((unsigned int)-1)
        , scale_(1.0f, 1.0f, 1.0f)
        , color_(1.0f, 1.0f, 1.0f, 1.0f)
        , shininess_(0.0f)
        , translucent_(false)
        , lit_(true)
    {}

    inline void RenderedModel::SetVisibility(bool visible)
    {
        visible_ = visible;
    }

    inline bool RenderedModel::IsVisible() const
    {
        return visible_;
    }

    inline void RenderedModel::SetMeshIndex(unsigned int meshIndex)
    {
        meshIndex_ = meshIndex;
    }

    inline unsigned int RenderedModel::GetMeshIndex() const
    {
        return meshIndex_;
    }

    inline void RenderedModel::SetTextureIndex(unsigned int textureIndex)
    {
        textureIndex_ = textureIndex;
    }

    inline unsigned int RenderedModel::GetTextureIndex() const
    {
        return textureIndex_;
    }

    inline void RenderedModel::SetSpecularIndex(unsigned int specularIndex)
    {
        specularIndex_ = specularIndex;
    }

    inline unsigned int RenderedModel::GetSpecularIndex() const
    {
        return specularIndex_;
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

    inline const glm::vec3& RenderedModel::GetScale() const
    {
        return scale_;
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

    inline const glm::vec4& RenderedModel::GetColor() const
    {
        return color_;
    }

    inline void RenderedModel::SetShininess(float shininess)
    {
        shininess_ = shininess;
    }

    inline float RenderedModel::GetShininess() const
    {
        return shininess_;
    }

    inline void RenderedModel::SetTranslucent(bool translucent)
    {
        translucent_ = translucent;
    }

    inline bool RenderedModel::GetTranslucent() const
    {
        return translucent_;
    }

    inline void RenderedModel::SetLit(bool lit)
    {
        lit_ = lit;
    }

    inline bool RenderedModel::GetLit() const
    {
        return lit_;
    }
}