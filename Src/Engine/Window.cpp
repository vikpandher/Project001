#include "Engine/Window.h"

#include "Engine/Platform/GLFW_Window.h"


namespace Project001
{
    // public: -----------------------------------------------------------------

    Window* Window::Create(const char* title, int width, int height)
    {
        return new GLFW_Window(title, width, height);
    }

    Window::~Window()
    {}
}