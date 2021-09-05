#include "Engine/Renderer.h"

#include "Engine/Platform/OpenGLRenderer.h"



namespace Project001
{
    Renderer* Renderer::Create()
    {
        return new OpenGLRenderer();
    }

    Renderer::~Renderer()
    {}
}