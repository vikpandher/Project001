// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "Window.h"

#include "Platform/GLFW_Window.h"


namespace Project001
{
    // public ------------------------------------------------------------------

    Window* Window::Create(const char* title, int width, int height)
    {
        return new GLFW_Window(title, width, height);
    }

    Window::~Window()
    {}
}