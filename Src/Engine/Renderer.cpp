#include "Engine/Renderer.h"

#include "Engine/Platform/OpenGLRenderer.h"
// #include "Engine/Platform/OpenGLRendererAlt.h"



namespace Project001
{
    Renderer* Renderer::Create(unsigned int width, unsigned int height)
    {
        return new OpenGLRenderer(width, height);
        // return new OpenGLRendererAlt(width, height);
    }

    Renderer::~Renderer()
    {}
}