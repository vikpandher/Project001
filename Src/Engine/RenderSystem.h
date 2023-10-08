#pragma once

#include "Renderer.h"

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

    protected:
        static void GroupMeshPtr(
            const RenderedMesh* renderedMeshPtr,
            const Camera* cameraPtr,
            const FrustumPlanes* frustumPlanesPtr);

        static std::vector<Camera*> s_cameraPtrs_;

        static std::vector<const RenderedMesh*> s_batchedRenderedMeshPtrs_;
        static std::vector<const RenderedMesh*> s_instancedRenderedMeshPtrs_;
        static std::vector<const RenderedMesh*> s_translucentInstancedRenderedMeshPtrs_;

        static std::vector<MeshInstanceData> s_meshInstanceDataArray_;
    };
}