#include "Engine/Renderer.h"

#include "Platform/OpenGLRenderer.h"



namespace Project001
{
    Renderer* Renderer::Create(ModelStores* modelStoresPtr, TextureStores* textureStoresPtr)
    {
        return new OpenGLRenderer(modelStoresPtr, textureStoresPtr);
    }
}