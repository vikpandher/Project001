#include "GLFWWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Engine/Event.h"
#include "Engine/Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    GLFWWindow::GLFWWindow(const char* title, int width, int height)
        : aspectRatioNumerator_(GLFW_DONT_CARE)
        , aspectRatioDenominator_(GLFW_DONT_CARE)
    {
        if (s_glfwWindowCount_ == 0)
        {
            if (!glfwInit())
            {
                _LOG_ERROR("Could not initalize GLFW!");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            glfwSetErrorCallback([](int errorCode, const char* description)
            {
                _LOG_ERROR("GLFW Error: (%d) %s", errorCode, description);
            });
        }

        glfwWindowPtr_ = glfwCreateWindow(width, height, title, NULL, NULL);
        s_glfwWindowCount_++;

        glfwSetWindowUserPointer(glfwWindowPtr_, this);

        GLFWwindow* oldContext = glfwGetCurrentContext();

        glfwMakeContextCurrent(glfwWindowPtr_);

        CheckAndInitializeOpenGL();

        // Disable VSync
        glfwSwapInterval(0);

        glfwMakeContextCurrent(oldContext);

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
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                KeyEvent event((KeyCode)key, (ButtonAction)action, (KeyModifier)mods);
                sourceWindow.EventCallback(event);
            });

        glfwSetMouseButtonCallback(glfwWindowPtr_, [](GLFWwindow* window, int button, int action, int mods)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                MouseButtonEvent event((MouseButton)button, (ButtonAction)action, (KeyModifier)mods);
                sourceWindow.EventCallback(event);
            });

        glfwSetCursorPosCallback(glfwWindowPtr_, [](GLFWwindow* window, double xpos, double ypos)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                CursorPositionEvent event((float)xpos, (float)ypos);
                sourceWindow.EventCallback(event);
            });

        glfwSetCursorEnterCallback(glfwWindowPtr_, [](GLFWwindow* window, int entered)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                CursorEnterEvent event((bool)entered);
                sourceWindow.EventCallback(event);
            });

        glfwSetScrollCallback(glfwWindowPtr_, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                ScrollEvent event((float)xoffset, (float)yoffset);
                sourceWindow.EventCallback(event);
            });

        glfwSetWindowCloseCallback(glfwWindowPtr_, [](GLFWwindow* window)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                sourceWindow.EventCallback(event);
            });

        glfwSetWindowFocusCallback(glfwWindowPtr_, [](GLFWwindow* window, int focused)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                WindowFocusEvent event((bool)focused);
                sourceWindow.EventCallback(event);
            });

        glfwSetWindowSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                WindowSizeEvent event(width, height);
                sourceWindow.EventCallback(event);
            });

        glfwSetFramebufferSizeCallback(glfwWindowPtr_, [](GLFWwindow* window, int width, int height)
            {
                GLFWWindow& sourceWindow = *(GLFWWindow*)glfwGetWindowUserPointer(window);
                FrameBufferSizeEvent event(width, height);
                sourceWindow.EventCallback(event);
            });
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(glfwWindowPtr_);

        if (s_glfwWindowCount_ == 0)
        {
            glfwTerminate();
        }
    }

    void GLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFWWindow::SetAspectRatio(int numerator, int denominator)
    {
        aspectRatioNumerator_ = numerator;
        aspectRatioDenominator_ = denominator;
        glfwSetWindowAspectRatio(glfwWindowPtr_, numerator, denominator);
    }

    void GLFWWindow::GetScreenSize(int& width, int& height) const
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        width = videoMode->width;
        height = videoMode->height;
    }

    void GLFWWindow::SetWindowPosition(int x, int y)
    {
        glfwSetWindowPos(glfwWindowPtr_, x, y);
    }

    void GLFWWindow::GetWindowPosition(int& x, int& y) const
    {
        glfwGetWindowPos(glfwWindowPtr_, &x, &y);
    }

    void GLFWWindow::SetWindowSize(int width, int height)
    {
        glfwSetWindowSize(glfwWindowPtr_, width, height);
    }

    void GLFWWindow::GetWindowSize(int& width, int& height) const
    {
        glfwGetWindowSize(glfwWindowPtr_, &width, &height);
    }

    void GLFWWindow::SetTime(const double time)
    {
        glfwSetTime(time);
    }

    double GLFWWindow::GetTime() const
    {
        return glfwGetTime();
    }

    void GLFWWindow::SetVSync(bool enabled)
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

    void GLFWWindow::GetFramebufferSize(int& width, int& height) const
    {
        glfwGetFramebufferSize(glfwWindowPtr_, &width, &height);
    }

    bool GLFWWindow::GetKeyPressed(KeyCode key) const
    {
        int result = glfwGetKey(glfwWindowPtr_, (int)key);
        return result == GLFW_PRESS;
    }

    bool GLFWWindow::GetMouseButtonPressed(MouseButton mouseButton) const
    {
        int result = glfwGetMouseButton(glfwWindowPtr_, (int)mouseButton);
        return result == GLFW_PRESS;
    }

    void GLFWWindow::GetCursorPosition(float& xPosition, float& yPosition) const
    {
        double xPositionDouble, yPositionDouble;
        glfwGetCursorPos(glfwWindowPtr_, &xPositionDouble, &yPositionDouble);
        xPosition = (float)xPositionDouble;
        yPosition = (float)yPositionDouble;
    }

    bool GLFWWindow::GetJoystickPresent(unsigned int index) const
    {
        int result = glfwJoystickPresent(index);
        return result == GLFW_TRUE;
    }

    void GLFWWindow::GetJoystickAxis(unsigned int index, float*& values, unsigned int& count) const
    {
        int countInt;
        const float* axes = glfwGetJoystickAxes(index, &countInt);
        count = (unsigned int)countInt;
        for (unsigned int i = 0; i < count; ++i)
        {
            values[i] = axes[i];
        }
    }

    void GLFWWindow::GetJoystickButtonsPressed(unsigned int index, bool*& values, unsigned int& count) const
    {
        int countInt;
        const unsigned char* buttons = glfwGetJoystickButtons(index, &countInt);
        count = (unsigned int)countInt;
        for (unsigned int i = 0; i < count; ++i)
        {
            values[i] = buttons[i] == GLFW_PRESS;
        }
    }

    void GLFWWindow::MakeContextCurrent()
    {
        glfwMakeContextCurrent(glfwWindowPtr_);
    }

    void GLFWWindow::MakeContextNotCurrent()
    {
        glfwMakeContextCurrent(NULL);
    }

    void GLFWWindow::SwapBuffers()
    {
        // Uses default platform swap interval, usually 60 fps
        glfwSwapBuffers(glfwWindowPtr_);
    }

    // protected ---------------------------------------------------------------

    void GLFWWindow::CheckAndInitializeOpenGL()
    {
        if (glHint == nullptr) // OpenGL functions need to be loaded
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                _LOG_ERROR("Failed to initialize Glad!");
            }
            else
            {
                _LOG_MESSAGE("OpenGL Info:");
                _LOG_MESSAGE("    Vendor: %s", glGetString(GL_VENDOR));
                _LOG_MESSAGE("    Renderer: %s", glGetString(GL_RENDERER));
                _LOG_MESSAGE("    Version: %s", glGetString(GL_VERSION));
                _LOG_MESSAGE("    Shading Language Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
            }
        }
    }

    int GLFWWindow::s_glfwWindowCount_ = 0;
}