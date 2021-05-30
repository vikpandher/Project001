#pragma once

#include <functional>



namespace Project001
{
    struct Event;
    enum class KeyCode;
    enum class MouseButton;

    class Window
    {
    public:
        static Window* Create(const char* title, int width, int height);

        virtual void PollEvents() = 0;

        virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;

        virtual void SetAspectRatio(int numerator, int denominator) = 0;

        virtual void SetViewportSize(int x, int y, int width, int height) = 0;

        virtual void SetWindowSize(int width, int height) = 0;

        virtual void SetTime(const double time) = 0;
        virtual double GetTime() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void GetFramebufferSize(int& width, int& height) const = 0;
        virtual void GetWindowSize(int& width, int& height) const = 0;

        virtual bool GetKeyPressed(KeyCode key) const = 0;
        virtual bool GetMouseButtonPressed(MouseButton mouseButton) const = 0;
        virtual void GetCursorPosition(float& xPosition, float& yPosition) const = 0;
        virtual bool GetJoystickPresent(unsigned int index) const = 0;
        virtual void GetJoystickAxis(unsigned int index, float*& values, unsigned int& count) const = 0;
        virtual void GetJoystickButtonsPressed(unsigned int index, bool*& values, unsigned int& count) const = 0;

    protected:

    private:
    };
}