#include "OpenGLWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Engine/Event.h"
#include "Engine/Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGLWindow::OpenGLWindow(const char* title, int width, int height)
        : isCurrentContext_(true)
        , aspectRatioNumerator_(GLFW_DONT_CARE)
        , aspectRatioDenominator_(GLFW_DONT_CARE)
    {
        if (s_glfwWindowCount_ == 0)
        {
            if (!glfwInit())
            {
                _LOG_ERROR("Could not initalize GLFW!");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwSetErrorCallback([](int errorCode, const char* description)
            {
                _LOG_ERROR("GLFW Error: (%d) %s", errorCode, description);
            });
        }

        glfwWindowPtr_ = glfwCreateWindow(width, height, title, NULL, NULL);
        s_glfwWindowCount_++;

        glfwSetWindowUserPointer(glfwWindowPtr_, &windowData_);

        glfwMakeContextCurrent(glfwWindowPtr_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            _LOG_ERROR("Failed to initialize Glad!");
        }

        _LOG_MESSAGE("OpenGL Info:");
        _LOG_MESSAGE("    Vendor: %s", glGetString(GL_VENDOR));
        _LOG_MESSAGE("    Renderer: %s", glGetString(GL_RENDERER));
        _LOG_MESSAGE("    Version: %s", glGetString(GL_VERSION));
        _LOG_MESSAGE("    Shading Language Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

        SetVSync(false);

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
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                KeyEvent event((KeyCode)key, (ButtonAction)action, (KeyModifier)mods);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(glfwWindowPtr_, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseButtonEvent event((MouseButton)button, (ButtonAction)action, (KeyModifier)mods);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(glfwWindowPtr_, [](GLFWwindow* window, double xpos, double ypos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                CursorPositionEvent event((float)xpos, (float)ypos);
                data.EventCallback(event);
            });

        glfwSetCursorEnterCallback(glfwWindowPtr_, [](GLFWwindow* window, int entered)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                CursorEnterEvent event((bool)entered);
                data.EventCallback(event);
            });

        glfwSetScrollCallback(glfwWindowPtr_, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                ScrollEvent event((float)xoffset, (float)yoffset);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(glfwWindowPtr_, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetWindowFocusCallback(glfwWindowPtr_, [](GLFWwindow* window, int focused)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowFocusEvent event((bool)focused);
                data.EventCallback(event);
            });

        glfwSetWindowSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowSizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetFramebufferSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                FrameBufferSizeEvent event(width, height);
                data.EventCallback(event);
            });
    }

    OpenGLWindow::~OpenGLWindow()
    {
        glfwDestroyWindow(glfwWindowPtr_);

        if (s_glfwWindowCount_ == 0)
        {
            glfwTerminate();
        }
    }

    void OpenGLWindow::PollEvents()
    {
        CheckAndMakeContextCurrent();

        glfwPollEvents();
    }

    void OpenGLWindow::SetAspectRatio(int numerator, int denominator)
    {
        aspectRatioNumerator_ = numerator;
        aspectRatioDenominator_ = denominator;
        glfwSetWindowAspectRatio(glfwWindowPtr_, numerator, denominator);
    }

    void OpenGLWindow::GetScreenSize(int& width, int& height) const
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        width = videoMode->width;
        height = videoMode->height;
    }

    void OpenGLWindow::SetWindowPosition(int x, int y)
    {
        glfwSetWindowPos(glfwWindowPtr_, x, y);
    }

    void OpenGLWindow::GetWindowPosition(int& x, int& y) const
    {
        glfwGetWindowPos(glfwWindowPtr_, &x, &y);
    }

    void OpenGLWindow::SetWindowSize(int width, int height)
    {
        glfwSetWindowSize(glfwWindowPtr_, width, height);
    }

    void OpenGLWindow::GetWindowSize(int& width, int& height) const
    {
        glfwGetWindowSize(glfwWindowPtr_, &width, &height);
    }

    void OpenGLWindow::SetTime(const double time)
    {
        glfwSetTime(time);
    }

    double OpenGLWindow::GetTime() const
    {
        return glfwGetTime();
    }

    void OpenGLWindow::SetVSync(bool enabled)
    {
        CheckAndMakeContextCurrent();

        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        vSyncEnabled_ = enabled;
    }

    void OpenGLWindow::GetFramebufferSize(int& width, int& height) const
    {
        glfwGetFramebufferSize(glfwWindowPtr_, &width, &height);
    }

    bool OpenGLWindow::GetKeyPressed(KeyCode key) const
    {
        int result = glfwGetKey(glfwWindowPtr_, (int)key);
        return result == GLFW_PRESS;
    }

    bool OpenGLWindow::GetMouseButtonPressed(MouseButton mouseButton) const
    {
        int result = glfwGetMouseButton(glfwWindowPtr_, (int)mouseButton);
        return result == GLFW_PRESS;
    }

    void OpenGLWindow::GetCursorPosition(float& xPosition, float& yPosition) const
    {
        double xPositionDouble, yPositionDouble;
        glfwGetCursorPos(glfwWindowPtr_, &xPositionDouble, &yPositionDouble);
        xPosition = (float)xPositionDouble;
        yPosition = (float)yPositionDouble;
    }

    bool OpenGLWindow::GetJoystickPresent(unsigned int index) const
    {
        int result = glfwJoystickPresent(index);
        return result == GL_TRUE;
    }

    void OpenGLWindow::GetJoystickAxis(unsigned int index, float*& values, unsigned int& count) const
    {
        int countInt;
        const float* axes = glfwGetJoystickAxes(index, &countInt);
        count = (unsigned int)countInt;
        for (unsigned int i = 0; i < count; ++i)
        {
            values[i] = axes[i];
        }
    }

    void OpenGLWindow::GetJoystickButtonsPressed(unsigned int index, bool*& values, unsigned int& count) const
    {
        int countInt;
        const unsigned char* buttons = glfwGetJoystickButtons(index, &countInt);
        count = (unsigned int)countInt;
        for (unsigned int i = 0; i < count; ++i)
        {
            values[i] = buttons[i] == GLFW_PRESS;
        }
    }

    // protected ---------------------------------------------------------------

    void OpenGLWindow::CheckAndMakeContextCurrent()
    {
        if (!isCurrentContext_)
        {
            glfwMakeContextCurrent(glfwWindowPtr_);
            isCurrentContext_ = true;
        }
    }

    int OpenGLWindow::s_glfwWindowCount_ = 0;
}