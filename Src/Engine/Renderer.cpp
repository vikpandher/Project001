#include "Engine/Renderer.h"

#ifdef VULKAN_MODE
#include "Engine/Platform/Vulkan_Renderer.h"
#else
#ifdef USE_ALT_OPENGL_RENDERER
#include "Engine/Platform/OpenGL_RendererAlt.h"
#else
#include "Engine/Platform/OpenGL_Renderer.h"
#endif
#endif


namespace Project001
{
    // public: -----------------------------------------------------------------

    Renderer* Renderer::Create(Window* windowPtr, unsigned int width, unsigned int height)
    {
#ifdef VULKAN_MODE
        return new Vulkan_Renderer(windowPtr, width, height);
#else
#ifdef USE_ALT_OPENGL_RENDERER
        return new OpenGL_RendererAlt(windowPtr, width, height);
#else
        return new OpenGL_Renderer(windowPtr, width, height);
#endif
#endif
    }

    Renderer::~Renderer()
    {}
}