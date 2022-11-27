#include "Engine/Renderer.h"

#ifdef VULKAN_MODE
#include "Engine/Platform/VulkanTutorial_Renderer.h"
#else
#include "Engine/Platform/OpenGL_Renderer.h"
#endif


namespace Project001
{
    // public: -----------------------------------------------------------------

    Renderer* Renderer::Create(Window* windowPtr, unsigned int width, unsigned int height)
    {
#ifdef VULKAN_MODE
        return new VulkanTutorial_Renderer(windowPtr, width, height);
#else
        return new OpenGL_Renderer(windowPtr, width, height, true);
#endif
    }

    Renderer::~Renderer()
    {}
}