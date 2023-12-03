#include "Engine/Renderer.h"

#ifdef VULKAN_MODE
#include "Engine/Platform/Vulkan_Renderer.h"
#else
#include "Engine/Platform/OpenGL_Renderer.h"
#endif



namespace Project001
{
    // public ------------------------------------------------------------------

    Renderer* Renderer::Create(const RendererInfo& rendererInfo)
    {
#ifdef VULKAN_MODE
        return new Vulkan_Renderer(rendererInfo);
#else
        return new OpenGL_Renderer(rendererInfo);
#endif
    }

    Renderer::~Renderer()
    {}
}