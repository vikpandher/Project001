// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-12

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

            transformedRenderedMesh.SetPosition(renderedMesh.GetPosition());
            transformedRenderedMesh.SetOrientation(renderedMesh.GetOrientation());
            size_t childMeshIndex = i;
            size_t parentMeshIndex = renderedMesh.GetParentMeshIndex();
            while (parentMeshIndex < childMeshIndex) // parent meshes must be added to the model before their child meshes
            {
                const RenderedMesh& parentMesh = renderedMeshes_[parentMeshIndex];
                const glm::vec3& parentPosition = parentMesh.GetPosition();
                const glm::quat& parentOrientation = parentMesh.GetOrientation();
                transformedRenderedMesh.SetPosition(parentPosition + parentOrientation * transformedRenderedMesh.GetPosition());
                transformedRenderedMesh.SetOrientation(parentOrientation * transformedRenderedMesh.GetOrientation());
                childMeshIndex = parentMeshIndex;
                parentMeshIndex = parentMesh.GetParentMeshIndex();
            }
            transformedRenderedMesh.SetPosition(position_ + orientation_ * transformedRenderedMesh.GetPosition());
            transformedRenderedMesh.SetOrientation(orientation_ * transformedRenderedMesh.GetOrientation());

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