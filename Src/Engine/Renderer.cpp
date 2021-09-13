#include "Engine/Renderer.h"

#include "Engine/Platform/OpenGLRenderer.h"



namespace Project001
{
    Renderer* Renderer::Create(unsigned int width, unsigned int height)
    {
        return new OpenGLRenderer(width, height);
    }

    Renderer::~Renderer()
    {}
}