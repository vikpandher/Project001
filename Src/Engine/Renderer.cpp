#include "Engine/Renderer.h"

#define SELECTED_RENDERER 2

#if SELECTED_RENDERER == 1
#include "Engine/Platform/OpenGLRenderer.h"
#else
#include "Engine/Platform/OpenGLRendererAlt.h"
#endif



namespace Project001
{
    // public: -----------------------------------------------------------------

    Renderer* Renderer::Create(unsigned int width, unsigned int height)
    {
#if SELECTED_RENDERER == 1
        return new OpenGLRenderer(width, height);
#else
        return new OpenGLRendererAlt(width, height);
#endif
    }

    Renderer::~Renderer()
    {}
}