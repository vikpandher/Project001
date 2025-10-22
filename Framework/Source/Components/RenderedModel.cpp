// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-22

#include "Components/RenderedModel.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    void RenderedModel::CalculateTransformedMeshes()
    {
        transformedRenderedMeshes_.clear();

        for (size_t i = 0; i < renderedMeshes_.size(); ++i)
        {
            const RenderedMesh& renderedMesh = renderedMeshes_[i];
            transformedRenderedMeshes_.emplace_back();
            RenderedMesh& transformedRenderedMesh = transformedRenderedMeshes_.back();
            transformedRenderedMesh.SetPosition(position_ + orientation_ * renderedMesh.GetPosition());
            transformedRenderedMesh.SetOrientation(orientation_ * renderedMesh.GetOrientation());
            transformedRenderedMesh.SetVisible(visible_ && renderedMesh.GetVisible());
            transformedRenderedMesh.SetCameraMask(cameraMask_ & renderedMesh.GetCameraMask());
            if (renderedMesh.GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_CPU_SIDE)
            {
                transformedRenderedMesh.SetMeshDataPtr(renderedMesh.GetMeshDataPtr());
            }
            else if (renderedMesh.GetRenderedMeshType() == RenderedMesh::RenderedMeshType::RENDERED_MESH_TYPE_LOADED_GPU_SIDE)
            {
                transformedRenderedMesh.SetMeshIdAndMaxBoundingRadius(renderedMesh.GetMeshId(), renderedMesh.GetMaxBoundingRadius());
            }
            transformedRenderedMesh.SetRenderPriorityOverride(renderedMesh.GetRenderPriorityOverride());
            transformedRenderedMesh.SetTextureId(renderedMesh.GetTextureId());
            transformedRenderedMesh.SetSpecularId(renderedMesh.GetSpecularId());
            transformedRenderedMesh.SetScale(scale_ * renderedMesh.GetScale());
            transformedRenderedMesh.SetColor(renderedMesh.GetColor());
            transformedRenderedMesh.SetShininess(renderedMesh.GetShininess());
            transformedRenderedMesh.SetTranslucent(renderedMesh.GetTranslucent());
            transformedRenderedMesh.SetUseLighting(renderedMesh.GetUseLighting());
        }
    }
}