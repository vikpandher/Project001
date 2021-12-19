#include "Engine/Window.h"

#include "Engine/Platform/GLFWWindow.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    Window* Window::Create(const char* title, int width, int height)
    {
        return new GLFWWindow(title, width, height);
    }

    Window::~Window()
    {}
}