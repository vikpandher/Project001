// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-17

#pragma once

#include "Renderer.h"

#include <unordered_map>
#include <vector>



namespace Project001
{
    class Camera;
    class ComponentStores;
    struct FrustumPlanes;
    class RenderedMesh;

    class RenderSystem
    {
    public:
        RenderSystem();

        void SetComponentStoresPtr(ComponentStores* componentStoresPtr);
        void SetRendererPtr(Renderer* rendererPtr);

        void Render();

        // Excludes meshes culled by the camera view frustum
        bool GetNumberOfMeshesRendered(unsigned int cameraEntityId, size_t& number);

    protected:
        void GroupMeshPtr(
            const RenderedMesh* renderedMeshPtr,
            const Camera* cameraPtr,
            const FrustumPlanes* frustumPlanesPtr);

        ComponentStores* componentStoresPtr_;
        Renderer* rendererPtr_;

        std::vector<Camera*> cameraPtrs_;

        std::vector<const RenderedMesh*> renderedMeshPtrs_;

        std::vector<MeshInstanceData> meshInstanceDataArray_;

        std::unordered_map<unsigned int, size_t> cameraEntityIdToRenderedMeshCount_;
    };

    // public ------------------------------------------------------------------

    inline void RenderSystem::SetComponentStoresPtr(ComponentStores* componentStoresPtr)
    {
        componentStoresPtr_ = componentStoresPtr;
    }

    inline void RenderSystem::SetRendererPtr(Renderer* rendererPtr)
    {
        rendererPtr_ = rendererPtr;
    }

    inline bool RenderSystem::GetNumberOfMeshesRendered(unsigned int cameraEntityId, size_t& number)
    {
        if (cameraEntityIdToRenderedMeshCount_.find(cameraEntityId) != cameraEntityIdToRenderedMeshCount_.end())
        {
            number = cameraEntityIdToRenderedMeshCount_[cameraEntityId];
            return true;
        }
        return false;
    }
}