// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "Renderer.h"

#ifdef VULKAN_BUILD
#include "Platform/Vulkan_Renderer.h"
#else
#include "Platform/OpenGL_Renderer.h"
#endif



namespace Project001
{
    // public ------------------------------------------------------------------

    Renderer* Renderer::Create(const RendererInfo& rendererInfo)
    {
#ifdef VULKAN_BUILD
        return new Vulkan_Renderer(rendererInfo);
#else
        return new OpenGL_Renderer(rendererInfo);
#endif
    }

    Renderer::~Renderer()
    {}
}