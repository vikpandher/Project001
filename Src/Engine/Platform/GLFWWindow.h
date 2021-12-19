#pragma once

#include "Engine/Window.h"



struct GLFWwindow;

namespace Project001
{
    class GLFWWindow : public Window
    {
    public:
        GLFWWindow(const char* title, int width, int height);
        virtual ~GLFWWindow() override;

        GLFWWindow(GLFWWindow& other) = delete;
        void operator=(const GLFWWindow&) = delete;

        void PollEvents() override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

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

        // NOTE:
        // GLFW threading, timer, and joystick input are not window dependent.

    protected:
        void CheckAndMakeContextCurrent();

        static int s_glfwWindowCount_;

        GLFWwindow* glfwWindowPtr_;

        bool isCurrentContext_;

        int aspectRatioNumerator_;
        int aspectRatioDenominator_;

        bool vSyncEnabled_;

        struct WindowData
        {
            std::function<void(Event&)> EventCallback;
        } windowData_;

    private:
    };

    // public: -----------------------------------------------------------------

    inline void GLFWWindow::SetEventCallback(const std::function<void(Event&)>& callback)
    {
        windowData_.EventCallback = callback;
    }

    inline void GLFWWindow::GetAspectRatio(int& numerator, int& denominator) const
    {
        numerator = aspectRatioNumerator_;
        denominator = aspectRatioDenominator_;
    }

    inline bool GLFWWindow::IsVSync() const
    {
        return vSyncEnabled_;
    }
}