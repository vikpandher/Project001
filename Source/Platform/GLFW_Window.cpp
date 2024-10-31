// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "GLFW_Window.h"

#include "Event.h"
#include "Logger.h"

#ifndef GLFW_INCLUDE_VULKAN
#include "glad/glad.h"
#endif

#include "GLFW/glfw3.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    GLFW_Window::GLFW_Window(const char* title, int width, int height)
        : windowTitle_(title)
        , aspectRatioNumerator_(GLFW_DONT_CARE)
        , aspectRatioDenominator_(GLFW_DONT_CARE)
        , vSyncEnabled_(false)
    {
        if (s_recycledWindowIds_.empty())
        {
            windowId_ = s_glfwWindowCount_;
        }
        else
        {
            windowId_ = s_recycledWindowIds_.front();
            s_recycledWindowIds_.pop_front();
        }

        if (s_glfwWindowCount_ == 0)
        {
            if (!glfwInit())
            {
                LOG_ERROR_F("Could not initalize GLFW");
            }

#ifdef GLFW_INCLUDE_VULKAN
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#else
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
            glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
            // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

            glfwSetErrorCallback([](int errorCode, const char* description)
                {
                    LOG_ERROR_F("GLFW Error: (" << errorCode << ") " << description);
                });
        }

        glfwWindowPtr_ = glfwCreateWindow(width, height, title, NULL, NULL);

        glfwSetWindowUserPointer(glfwWindowPtr_, this);

#ifndef GLFW_INCLUDE_VULKAN
        GLFWwindow* oldContext = glfwGetCurrentContext();

        glfwMakeContextCurrent(glfwWindowPtr_);

        if (glHint == nullptr) // OpenGL functions need to be loaded
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                LOG_ERROR_F("Failed to initialize OpenGL");
            }
            else
            {
                LOG_INFO("OpenGL Info:\n" << 
                    INDENT << "    Vendor: " << glGetString(GL_VENDOR) << "\n" <<
                    INDENT << "    Renderer: " << glGetString(GL_RENDERER) << "\n" <<
                    INDENT << "    Version: " << glGetString(GL_VERSION) << "\n" <<
                    INDENT << "    Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
                );
            }
        }

        // Disable VSync
        glfwSwapInterval(0);

        glfwMakeContextCurrent(oldContext);
#endif

        InitializeCallbacks();

        s_glfwWindowCount_++;
    }

    GLFW_Window::~GLFW_Window()
    {
        s_glfwWindowCount_--;

        s_recycledWindowIds_.push_back(windowId_);

        glfwDestroyWindow(glfwWindowPtr_);

        if (s_glfwWindowCount_ == 0)
        {
            glfwTerminate();
            s_recycledWindowIds_.clear();
        }
    }

    void GLFW_Window::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFW_Window::SetWindowTitle(const char*& title)
    {
        windowTitle_ = title;
        glfwSetWindowTitle(glfwWindowPtr_, title);
    }

    void GLFW_Window::SetAspectRatio(int numerator, int denominator)
    {
        aspectRatioNumerator_ = numerator;
        aspectRatioDenominator_ = denominator;
        glfwSetWindowAspectRatio(glfwWindowPtr_, numerator, denominator);
    }

    void GLFW_Window::GetScreenSize(int& width, int& height) const
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        width = videoMode->width;
        height = videoMode->height;
    }

    void GLFW_Window::SetWindowPosition(int x, int y)
    {
        glfwSetWindowPos(glfwWindowPtr_, x, y);
    }

    void GLFW_Window::GetWindowPosition(int& x, int& y) const
    {
        glfwGetWindowPos(glfwWindowPtr_, &x, &y);
    }

    void GLFW_Window::SetWindowSize(int width, int height)
    {
        glfwSetWindowSize(glfwWindowPtr_, width, height);
    }

    void GLFW_Window::GetWindowSize(int& width, int& height) const
    {
        glfwGetWindowSize(glfwWindowPtr_, &width, &height);
    }

    void GLFW_Window::SetTime(const double time)
    {
        glfwSetTime(time);
    }

    double GLFW_Window::GetTime() const
    {
        return glfwGetTime();
    }

    void GLFW_Window::SetVSync(bool enabled)
    {
        GLFWwindow* oldContext = glfwGetCurrentContext();

        glfwMakeContextCurrent(glfwWindowPtr_);

        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        glfwMakeContextCurrent(oldContext);

        vSyncEnabled_ = enabled;
    }

    void GLFW_Window::GetFramebufferSize(int& width, int& height) const
    {
        glfwGetFramebufferSize(glfwWindowPtr_, &width, &height);
    }

    bool GLFW_Window::GetKeyPressed(KeyCode key) const
    {
        int result = glfwGetKey(glfwWindowPtr_, (int)key);
        return result == GLFW_PRESS;
    }

    bool GLFW_Window::GetMouseButtonPressed(MouseButton mouseButton) const
    {
        int result = glfwGetMouseButton(glfwWindowPtr_, (int)mouseButton);
        return result == GLFW_PRESS;
    }

    void GLFW_Window::GetCursorPosition(float& xPosition, float& yPosition) const
    {
        double xPositionDouble, yPositionDouble;
        glfwGetCursorPos(glfwWindowPtr_, &xPositionDouble, &yPositionDouble);
        xPosition = (float)xPositionDouble;
        yPosition = (float)yPositionDouble;
    }

    bool GLFW_Window::GetJoystickPresent(unsigned int index) const
    {
        int result = glfwJoystickPresent(index);
        return result == GLFW_TRUE;
    }

    void GLFW_Window::GetJoystickAxis(unsigned int index, std::vector<float>& values) const
    {
        int count;
        const float* axes = glfwGetJoystickAxes(index, &count);

        values.clear();
        for (int i = 0; i < count; ++i)
        {
            values.push_back(axes[i]);
        }
    }

    void GLFW_Window::GetJoystickButtons(unsigned int index, std::vector<bool>& values) const
    {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(index, &count);

        values.clear();
        for (int i = 0; i < count; ++i)
        {
            values.push_back(buttons[i] == GLFW_PRESS);
        }
    }

    void GLFW_Window::MakeContextCurrent()
    {
        if (glfwWindowPtr_ != glfwGetCurrentContext())
        {
            glfwMakeContextCurrent(glfwWindowPtr_);
        }
    }

    void GLFW_Window::MakeContextNotCurrent()
    {
        glfwMakeContextCurrent(NULL);
    }

    void GLFW_Window::SwapBuffers()
    {
        // Uses default platform swap interval, usually 60 fps
        glfwSwapBuffers(glfwWindowPtr_);
    }

    void GLFW_Window::CreateWindowSurface(VkInstance& instance, VkSurfaceKHR& surface)
    {
#ifdef GLFW_INCLUDE_VULKAN
        if (glfwCreateWindowSurface(instance, glfwWindowPtr_, NULL, &surface) != VK_SUCCESS)
        {
            LOG_ERROR_F("Failed to create window surface");
        }
#endif
    }

    const char** GLFW_Window::GetRequiredInstanceExtensions(uint32_t* count)
    {
#ifdef GLFW_INCLUDE_VULKAN
        return glfwGetRequiredInstanceExtensions(count);
#else
        return NULL;
#endif
    }

    // protected ---------------------------------------------------------------

    void GLFW_Window::InitializeCallbacks()
    {
        // NOTES:
        // GLFW Callback Functions:
        // ---------------------------------------------------------------------
        // ERROR CALLBACKS: https://www.glfw.org/docs/3.3/group__init.html
        // GLFWerrorfun glfwSetErrorCallback (GLFWerrorfun callback) ---implemented---
        //
        // INPUT CALLBACKS: https://www.glfw.org/docs/3.3/group__input.html
        // GLFWkeyfun glfwSetKeyCallback (GLFWwindow *window, GLFWkeyfun callback) ---implemented---
        // GLFWcharfun glfwSetCharCallback (GLFWwindow *window, GLFWcharfun callback)
        // GLFWcharmodsfun glfwSetCharModsCallback (GLFWwindow *window, GLFWcharmodsfun callback)
        // GLFWmousebuttonfun glfwSetMouseButtonCallback (GLFWwindow *window, GLFWmousebuttonfun callback) ---implemented---
        // GLFWcursorposfun glfwSetCursorPosCallback (GLFWwindow *window, GLFWcursorposfun callback) ---implemented---
        // GLFWcursorenterfun glfwSetCursorEnterCallback (GLFWwindow *window, GLFWcursorenterfun callback) ---implemented---
        // GLFWscrollfun glfwSetScrollCallback (GLFWwindow *window, GLFWscrollfun callback) ---implemented---
        // GLFWdropfun glfwSetDropCallback (GLFWwindow *window, GLFWdropfun callback)
        // GLFWjoystickfun glfwSetJoystickCallback (GLFWjoystickfun callback)
        //
        // MONITOR CALLBACKS: https://www.glfw.org/docs/3.3/group__monitor.html
        // GLFWmonitorfun glfwSetMonitorCallback (GLFWmonitorfun callback)
        //
        // WINDOW CALLBACKS: https://www.glfw.org/docs/3.3/group__window.html
        // GLFWwindowposfun glfwSetWindowPosCallback (GLFWwindow *window, GLFWwindowposfun callback)
        // GLFWwindowsizefun glfwSetWindowSizeCallback (GLFWwindow *window, GLFWwindowsizefun callback)
        // GLFWwindowclosefun glfwSetWindowCloseCallback (GLFWwindow *window, GLFWwindowclosefun callback) ---implemented---
        // GLFWwindowrefreshfun glfwSetWindowRefreshCallback (GLFWwindow *window, GLFWwindowrefreshfun callback)
        // GLFWwindowfocusfun glfwSetWindowFocusCallback (GLFWwindow *window, GLFWwindowfocusfun callback) ---implemented---
        // GLFWwindowiconifyfun glfwSetWindowIconifyCallback (GLFWwindow *window, GLFWwindowiconifyfun callback)
        // GLFWwindowmaximizefun glfwSetWindowMaximizeCallback (GLFWwindow *window, GLFWwindowmaximizefun callback)
        // GLFWframebuffersizefun glfwSetFramebufferSizeCallback (GLFWwindow *window, GLFWframebuffersizefun callback) ---implemented---
        // GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback (GLFWwindow *window, GLFWwindowcontentscalefun callback)

        glfwSetKeyCallback(glfwWindowPtr_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                KeyEvent event(sourceWindow.GetWindowId(), (KeyCode)key, (ButtonAction)action, (KeyModifier)mods);
                sourceWindow.EventCallback(event);
            });

        glfwSetMouseButtonCallback(glfwWindowPtr_, [](GLFWwindow* window, int button, int action, int mods)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                MouseButtonEvent event(sourceWindow.GetWindowId(), (MouseButton)button, (ButtonAction)action, (KeyModifier)mods);
                sourceWindow.EventCallback(event);
            });

        glfwSetCursorPosCallback(glfwWindowPtr_, [](GLFWwindow* window, double xpos, double ypos)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                CursorPositionEvent event(sourceWindow.GetWindowId(), (float)xpos, (float)ypos);
                sourceWindow.EventCallback(event);
            });

        glfwSetCursorEnterCallback(glfwWindowPtr_, [](GLFWwindow* window, int entered)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                CursorEnterEvent event(sourceWindow.GetWindowId(), (bool)entered);
                sourceWindow.EventCallback(event);
            });

        glfwSetScrollCallback(glfwWindowPtr_, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                ScrollEvent event(sourceWindow.GetWindowId(), (float)xoffset, (float)yoffset);
                sourceWindow.EventCallback(event);
            });

        glfwSetWindowCloseCallback(glfwWindowPtr_, [](GLFWwindow* window)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event(sourceWindow.GetWindowId());
                sourceWindow.EventCallback(event);
            });

        glfwSetWindowFocusCallback(glfwWindowPtr_, [](GLFWwindow* window, int focused)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                WindowFocusEvent event(sourceWindow.GetWindowId(), (bool)focused);
                sourceWindow.EventCallback(event);
            });

        glfwSetWindowSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                WindowSizeEvent event(sourceWindow.GetWindowId(), width, height);
                sourceWindow.EventCallback(event);
            });

        // The framebuffer size might change if the window is resized or if the
        // pixel density (DPI scaling) of the window changes, resulting in a
        // different framebuffer size but the same window size.
        glfwSetFramebufferSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                GLFW_Window& sourceWindow = *(GLFW_Window*)glfwGetWindowUserPointer(window);
                FrameBufferSizeEvent event(sourceWindow.GetWindowId(), width, height);
                sourceWindow.EventCallback(event);
            });
    }

    int GLFW_Window::s_glfwWindowCount_ = 0;

    std::deque<unsigned int> GLFW_Window::s_recycledWindowIds_;
}