#include "Engine/Renderer.h"

#include "Engine/Platform/OpenGLRenderer.h"



namespace Project001
{
    Renderer* Renderer::Create(MeshStores* meshStoresPtr, TextureStores* textureStoresPtr)
    {
        return new OpenGLRenderer(meshStoresPtr, textureStoresPtr);
    }
}