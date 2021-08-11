#pragma once

#include "Engine/Components/Model.h"



namespace Project001
{
    class RenderedModel : public Model
    {
    public:
        RenderedModel();
        ~RenderedModel();

        void SetTextureIndex(unsigned int textureIndex);
        unsigned int GetTextureIndex() const;

        void SetSpecularIndex(unsigned int specularIndex);
        unsigned int GetSpecularIndex() const;

        void SetShininess(float shininess);
        float GetShininess() const;

        void SetColor(const glm::vec4& color);
        void SetColor(float r, float g, float b, float a);
        void SetColorRGB(float r, float g, float b);
        const glm::vec4& GetColor() const;

        void SetTranslucent(bool translucent);
        bool GetTranslucent() const;

    protected:
        unsigned int textureIndex_;
        unsigned int specularIndex_;
        float shininess_; // 32.0f looks good
        glm::vec4 color_;
        bool translucent_;

    private:
    };

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

    inline void RenderedModel::SetShininess(float shininess)
    {
        shininess_ = shininess;
    }

    inline float RenderedModel::GetShininess() const
    {
        return shininess_;
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

    inline void RenderedModel::SetTranslucent(bool translucent)
    {
        translucent_ = translucent;
    }

    inline bool RenderedModel::GetTranslucent() const
    {
        return translucent_;
    }
}