// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#pragma once

#include "Components/Placement.h"
#include "MeshData.h"



namespace Project001
{
    class RenderedMesh : public Placement
    {
    public:
        enum class RenderedMeshType
        {
            RENDERED_MESH_TYPE_NOT_LOADED,
            RENDERED_MESH_TYPE_LOADED_CPU_SIDE,
            RENDERED_MESH_TYPE_LOADED_GPU_SIDE //, TODO: Add stenciling
            // RENDERED_MESH_TYPE_STENCIL_LOADED_CPU_SIDE,
            // RENDERED_MESH_TYPE_STENCIL_LOADED_GPU_SIDE
        };

        RenderedMesh();

        const bool& GetVisible() const;
        void SetVisible(bool visible);

        void SetCameraMask(uint32_t cameraMask);
        const uint32_t& GetCameraMask() const;

        // RenderedMeshType is set by the following functions:
        // SetMeshDataPtr
        //     * Sets it to RENDERED_MESH_TYPE_LOADED_CPU_SIDE
        // SetMeshId
        //     * Sets it to RENDERED_MESH_TYPE_LOADED_GPU_SIDE

        const RenderedMeshType& GetRenderedMeshType() const;

        const MeshData* const& GetMeshDataPtr() const;
        void SetMeshDataPtr(const MeshData* meshData);

        // If the mesh is of type RENDERED_MESH_TYPE_LOADED_CPU_SIDE,
        // meshDataPtr_->maxBoundingRadius is used for frustum culling.
        // 
        // If the mesh is of type RENDERED_MESH_TYPE_LOADED_GPU_SIDE,
        // boundingRadius_ is used for frustum culling.
        // 
        // Frustum culling is done in RenderSystem.

        const unsigned int& GetMeshId() const;
        const float& GetMaxBoundingRadius() const;
        void SetMeshIdAndMaxBoundingRadius(unsigned int meshId, float maxBoundingRadius);

        // lower priority mesh will be rendered by a camera first, so higher
        // priorty meshes are drawn on-top
        const int& GetRenderPriorityOverride() const;
        void SetRenderPriorityOverride(int renderPriorityOverride);

        const unsigned int& GetTextureId() const;
        void SetTextureId(unsigned int textureId);

        const unsigned int& GetSpecularId() const;
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

        const float& GetShininess() const;
        void SetShininess(float shininess);

        const bool& GetTranslucent() const;
        void SetTranslucent(bool translucent);

        const bool& GetUseLighting() const;
        void SetUseLighting(bool useLighting);

    protected:
        // Inherited:
        // glm::vec3 position_;
        // glm::quat orientation_;

        bool visible_;
        uint32_t cameraMask_;
        RenderedMeshType renderedMeshType_;
        const MeshData* meshDataPtr_; // Used when RENDERED_MESH_TYPE_LOADED_CPU_SIDE
        unsigned int meshId_;         // Used when RENDERED_MESH_TYPE_LOADED_GPU_SIDE
        float maxBoundingRadius_;     // Used when RENDERED_MESH_TYPE_LOADED_GPU_SIDE (for frustum culling)
        int renderPriorityOverride_;
        unsigned int textureId_;
        unsigned int specularId_;
        glm::vec3 scale_;
        float shininess_; // 32.0f looks good
        glm::vec4 color_;
        bool translucent_;
        bool useLighting_;
    };

    inline RenderedMesh::RenderedMesh()
        : visible_(true)
        , cameraMask_(0b11111111111111111111111111111111)
        , renderedMeshType_(RenderedMeshType::RENDERED_MESH_TYPE_NOT_LOADED)
        , meshDataPtr_(nullptr)
        , meshId_(static_cast<unsigned int>(-1))
        , maxBoundingRadius_(0.0f)
        , renderPriorityOverride_(0)
        , textureId_(static_cast<unsigned int>(-1))
        , specularId_(static_cast<unsigned int>(-1))
        , scale_(1.0f, 1.0f, 1.0f)
        , color_(1.0f, 1.0f, 1.0f, 1.0f)
        , shininess_(0.0f)
        , translucent_(false)
        , useLighting_(true)
    {}

    inline const bool& RenderedMesh::GetVisible() const
    {
        return visible_;
    }

    inline void RenderedMesh::SetVisible(bool visible)
    {
        visible_ = visible;
    }

    inline void RenderedMesh::SetCameraMask(uint32_t cameraMask)
    {
        cameraMask_ = cameraMask;
    }

    inline const uint32_t& RenderedMesh::GetCameraMask() const
    {
        return cameraMask_;
    }

    inline const RenderedMesh::RenderedMeshType& RenderedMesh::GetRenderedMeshType() const
    {
        return renderedMeshType_;
    }

    inline const MeshData* const& RenderedMesh::GetMeshDataPtr() const
    {
        return meshDataPtr_;
    }

    inline void RenderedMesh::SetMeshDataPtr(const MeshData* meshDataPtr)
    {
        renderedMeshType_ = RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE;
        meshDataPtr_ = meshDataPtr;
    }

    inline const unsigned int& RenderedMesh::GetMeshId() const
    {
        return meshId_;
    }

    inline const float& RenderedMesh::GetMaxBoundingRadius() const
    {
        return maxBoundingRadius_;
    }

    inline void RenderedMesh::SetMeshIdAndMaxBoundingRadius(unsigned int meshId, float maxBoundingRadius)
    {
        renderedMeshType_ = RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE;
        meshId_ = meshId;
        maxBoundingRadius_ = maxBoundingRadius;
    }

    inline const int& RenderedMesh::GetRenderPriorityOverride() const
    {
        return renderPriorityOverride_;
    }

    inline void RenderedMesh::SetRenderPriorityOverride(int renderPriorityOverride)
    {
        renderPriorityOverride_ = renderPriorityOverride;
    }

    inline const unsigned int& RenderedMesh::GetTextureId() const
    {
        return textureId_;
    }

    inline void RenderedMesh::SetTextureId(unsigned int textureId)
    {
        textureId_ = textureId;
    }

    inline const unsigned int& RenderedMesh::GetSpecularId() const
    {
        return specularId_;
    }

    inline void RenderedMesh::SetSpecularId(unsigned int specularId)
    {
        specularId_ = specularId;
    }

    inline const glm::vec3& RenderedMesh::GetScale() const
    {
        return scale_;
    }

    inline void RenderedMesh::SetScale(const glm::vec3& scale)
    {
        scale_ = scale;
    }

    inline void RenderedMesh::SetScale(float x, float y, float z)
    {
        scale_.x = x;
        scale_.y = y;
        scale_.z = z;
    }

    inline void RenderedMesh::SetScaleX(float x)
    {
        scale_.x = x;
    }

    inline void RenderedMesh::SetScaleY(float y)
    {
        scale_.y = y;
    }

    inline void RenderedMesh::SetScaleZ(float z)
    {
        scale_.z = z;
    }

    inline const glm::vec4& RenderedMesh::GetColor() const
    {
        return color_;
    }

    inline void RenderedMesh::SetColor(const glm::vec4& color)
    {
        color_ = color;
    }

    inline void RenderedMesh::SetColor(float r, float g, float b, float a)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
        color_.a = a;
    }

    inline void RenderedMesh::SetColorRGB(float r, float g, float b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

    inline const float& RenderedMesh::GetShininess() const
    {
        return shininess_;
    }

    inline void RenderedMesh::SetShininess(float shininess)
    {
        shininess_ = shininess;
    }

    inline const bool& RenderedMesh::GetTranslucent() const
    {
        return translucent_;
    }

    inline void RenderedMesh::SetTranslucent(bool translucent)
    {
        translucent_ = translucent;
    }

    inline const bool& RenderedMesh::GetUseLighting() const
    {
        return useLighting_;
    }

    inline void RenderedMesh::SetUseLighting(bool useLighting)
    {
        useLighting_ = useLighting;
    }
}