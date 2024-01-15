#pragma once

#include "Window.h"

#include <deque>



struct GLFWwindow;

namespace Project001
{
    class GLFW_Window : public Window
    {
    public:
        GLFW_Window(const char* title, int width, int height);
        virtual ~GLFW_Window() override;

        GLFW_Window(GLFW_Window& other) = delete;
        void operator=(const GLFW_Window&) = delete;

        void PollEvents() override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

        unsigned int GetWindowId() const override;

        void SetWindowTitle(const char*& title) override;
        void GetWindowTitle(const char*& title) const override;

        // Setting numerator and denominator to -1 unlocks the aspect ratio.
        void SetAspectRatio(int numerator, int denominator) override;
        void GetAspectRatio(int& numerator, int& denominator) const override;

        void GetScreenSize(int& width, int& height) const override;

        void SetWindowPosition(int x, int y) override;
        void GetWindowPosition(int& x, int& y) const override;

        void SetWindowSize(int width, int height) override;
        void GetWindowSize(int& width, int& height) const override;

        // Time measurements are in seconds
        void SetTime(const double time) override;
        double GetTime() const override;

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void GetFramebufferSize(int& width, int& height) const override;

        bool GetKeyPressed(KeyCode key) const override;
        bool GetMouseButtonPressed(MouseButton mouseButton) const override;
        void GetCursorPosition(float& xPosition, float& yPosition) const override;
        bool GetJoystickPresent(unsigned int index) const override;
        void GetJoystickAxis(unsigned int index, float*& values, unsigned int& count) const override;
        void GetJoystickButtonsPressed(unsigned int index, bool*& values, unsigned int& count) const override;

        void MakeContextCurrent() override;
        void MakeContextNotCurrent() override;
        void SwapBuffers() override;

        void CreateWindowSurface(VkInstance& instance, VkSurfaceKHR& surface) override;
        const char** GetRequiredInstanceExtensions(uint32_t* count) override;

        // NOTE:
        // GLFW threading, timer, and joystick input are not window dependent.

    protected:
        void InitializeCallbacks();

        static int s_glfwWindowCount_;

        static std::deque<unsigned int> s_recycledWindowIds_;

        unsigned int windowId_;

        const char* windowTitle_;

        GLFWwindow* glfwWindowPtr_;

        std::function<void(Event&)> EventCallback;

        int aspectRatioNumerator_;
        int aspectRatioDenominator_;

        bool vSyncEnabled_;
    };

    // public ------------------------------------------------------------------

    inline void GLFW_Window::SetEventCallback(const std::function<void(Event&)>& callback)
    {
        EventCallback = callback;
    }

    inline unsigned int GLFW_Window::GetWindowId() const
    {
        return windowId_;
    }

    inline void GLFW_Window::GetWindowTitle(const char*& title) const
    {
        title = windowTitle_;
    }

    inline void GLFW_Window::GetAspectRatio(int& numerator, int& denominator) const
    {
        numerator = aspectRatioNumerator_;
        denominator = aspectRatioDenominator_;
    }

    inline bool GLFW_Window::IsVSync() const
    {
        return vSyncEnabled_;
    }
}