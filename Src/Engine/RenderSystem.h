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
        static void Render(
            ComponentStores* componentStoresPtr,
            Renderer* rendererPtr);

        // Excludes meshes culled by the camera view frustum
        static bool GetNumberOfMeshesRendered(unsigned int cameraEntityId, size_t& number);

    protected:
        static void GroupMeshPtr(
            const RenderedMesh* renderedMeshPtr,
            const Camera* cameraPtr,
            const FrustumPlanes* frustumPlanesPtr);

        static std::vector<Camera*> s_cameraPtrs_;

        static std::vector<const RenderedMesh*> s_renderedMeshPtrs_;

        static std::vector<MeshInstanceData> s_meshInstanceDataArray_;

        static std::unordered_map<unsigned int, size_t> s_cameraEntityIdToRenderedMeshCount_;
    };

    inline bool RenderSystem::GetNumberOfMeshesRendered(unsigned int cameraEntityId, size_t& number)
    {
        if (s_cameraEntityIdToRenderedMeshCount_.find(cameraEntityId) != s_cameraEntityIdToRenderedMeshCount_.end())
        {
            number = s_cameraEntityIdToRenderedMeshCount_[cameraEntityId];
            return true;
        }
        return false;
    }
}