#include "Engine/Window.h"

#include "Platform/OpenGLWindow.h"



namespace Project001
{
    Window* Window::Create(const char* title, int width, int height)
    {
        return new OpenGLWindow(title, width, height);
    }
}