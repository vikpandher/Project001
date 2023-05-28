#include "Engine/Renderer.h"

#ifdef VULKAN_MODE
#include "Engine/Platform/Vulkan_Renderer.h"
#else
#include "Engine/Platform/OpenGL_Renderer.h"
#endif


namespace Project001
{
    // public ------------------------------------------------------------------

    Renderer* Renderer::Create(
        Window* windowPtr,
        unsigned int frameBufferWidth,
        unsigned int frameBufferHeight,
        unsigned int indexBufferCapacity,
        unsigned int vertexBufferCapacity)
    {
#ifdef VULKAN_MODE
        return new Vulkan_Renderer(windowPtr, frameBufferWidth, frameBufferHeight, indexBufferCapacity, vertexBufferCapacity, true);
#else
        return new OpenGL_Renderer(windowPtr, frameBufferWidth, frameBufferWidth, indexBufferCapacity, vertexBufferCapacity, true);
#endif
    }

    Renderer::~Renderer()
    {}
}