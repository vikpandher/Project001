// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-20

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
        void Render(
            ComponentStores* componentStoresPtr,
            Renderer* rendererPtr);

        // Excludes meshes culled by the camera view frustum
        bool GetNumberOfMeshesRendered(unsigned int cameraEntityId, size_t& number);

    protected:
        void GroupMeshPtr(
            const RenderedMesh* renderedMeshPtr,
            const Camera* cameraPtr,
            const FrustumPlanes* frustumPlanesPtr);

        std::vector<Camera*> cameraPtrs_;

        std::vector<const RenderedMesh*> renderedMeshPtrs_;

        std::vector<MeshInstanceData> meshInstanceDataArray_;

        std::unordered_map<unsigned int, size_t> cameraEntityIdToRenderedMeshCount_;
    };

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