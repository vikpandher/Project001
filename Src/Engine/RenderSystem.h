#pragma once



namespace Project001
{
    class ComponentStores;
    class Renderer;

    class RenderSystem
    {
    public:
        static void Render(ComponentStores* componentStoresPtr, Renderer* rendererPtr);
    };
}