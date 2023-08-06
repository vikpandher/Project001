#pragma once

#include "Renderer.h"

#include <vector>



namespace Project001
{
    class ComponentStores;
    class RenderedModel;

    class RenderSystem
    {
    public:
        static void Render(ComponentStores* componentStoresPtr, Renderer* rendererPtr);

    protected:
        static std::vector<RenderedModel*> s_batchedRenderedModelPtrs_;
        static std::vector<RenderedModel*> s_instancedRenderedModelPtrs_;
        static std::vector<RenderedModel*> s_translucentInstancedRenderedModelPtrs_;

        static std::vector<MeshInstanceData> s_meshInstanceDataArray_;
    };
}