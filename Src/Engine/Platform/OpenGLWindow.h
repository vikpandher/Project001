#pragma once

#include "Engine/Window.h"



struct GLFWwindow;

namespace Project001
{
    class OpenGLWindow : public Window
    {
    public:
        OpenGLWindow(const char* title, int width, int height);
        virtual ~OpenGLWindow() override;

        OpenGLWindow(OpenGLWindow& other) = delete;
        void operator=(const OpenGLWindow&) = delete;

        void PollEvents() override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

        // Setting numerator and denominator to -1 unlocks the aspect ratio.
        void SetAspectRatio(int numerator, int denominator);

        void SetWindowSize(int width, int height) override;

        // Time measurements are in seconds
        void SetTime(const double time) override;
        double GetTime() const override;

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void GetFramebufferSize(int& width, int& height) const override;
        void GetWindowSize(int& width, int& height) const override;

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

        struct WindowData
        {
            WindowData()
                : vSyncEnabled(false)
            {}

            std::function<void(Event&)> EventCallback;

            bool vSyncEnabled;
        } windowData_;

    private:
    };

    // public: -----------------------------------------------------------------

    inline void OpenGLWindow::SetEventCallback(const std::function<void(Event&)>& callback)
    {
        windowData_.EventCallback = callback;
    }

    inline bool OpenGLWindow::IsVSync() const
    {
        return windowData_.vSyncEnabled;
    }
}