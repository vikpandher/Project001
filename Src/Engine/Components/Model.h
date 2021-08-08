#pragma once

#include "Engine/Components/Placement.h"



namespace Project001
{
    class Model : public Placement
    {
    public:
        Model();
        ~Model();

        void SetModelIndex(unsigned int modelIndex);
        unsigned int GetModelIndex() const;

        void SetScale(const glm::vec3& scale);
        void SetScale(float x, float y, float z);
        void SetScaleX(float x);
        void SetScaleY(float y);
        void SetScaleZ(float z);
        const glm::vec3& GetScale() const;

    protected:
        unsigned int modelIndex_;

        glm::vec3 scale_;

    private:
    };

    inline void Model::SetModelIndex(unsigned int modelIndex)
    {
        modelIndex_ = modelIndex;
    }

    inline unsigned int Model::GetModelIndex() const
    {
        return modelIndex_;
    }

    inline void Model::SetScale(const glm::vec3& scale)
    {
        scale_ = scale;
    }

    inline void Model::SetScale(float x, float y, float z)
    {
        scale_.x = x;
        scale_.y = y;
        scale_.z = z;
    }

    inline void Model::SetScaleX(float x)
    {
        scale_.x = x;
    }

    inline void Model::SetScaleY(float y)
    {
        scale_.y = y;
    }

    inline void Model::SetScaleZ(float z)
    {
        scale_.z = z;
    }

    inline const glm::vec3& Model::GetScale() const
    {
        return scale_;
    }
}