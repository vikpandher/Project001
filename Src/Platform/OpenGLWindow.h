#pragma once

#include "Engine/Window.h"



struct GLFWwindow;

namespace Project001
{
    class OpenGLShader;
    class OpenGLTexture;

    class OpenGLWindow : public Window
    {
    public:
        OpenGLWindow(const char* title, int width, int height);
        ~OpenGLWindow();

        OpenGLWindow(OpenGLWindow& other) = delete;
        void operator=(const OpenGLWindow&) = delete;

        void Render(const RenderData* renderData) override;

        void AddTexture(unsigned int textureSlot, unsigned char* data, int width, int height, int numberOfComponents) override;

        void PollEvents() override;

        void SetEventCallback(const std::function<void(Event&)>& callback) override;

        // Setting numerator and denominator to -1 unlocks the aspect ratio.
        void SetAspectRatio(int numerator, int denominator);

        void SetViewportSize(int x, int y, int width, int height) override;

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

        // determines the size of the index and vertex buffers
        // static const unsigned int s_bufferCapacity_ = 36 * 10;
        static const unsigned int s_indexBufferCapacity_ = 36 * 10;
        static const unsigned int s_vertexBufferCapacity_ = 36 * 8;

        static const unsigned int s_numberOfTextureSlots_ = 16;

        static const unsigned int s_numberOfPointLights_ = 8;
        static const unsigned int s_numberOfSpotLights_ = 4;

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

        OpenGLShader* shaderPtr_;

        // this holds the buffer's id
        // the buffer holds the blob of data that will be displayed
        unsigned int vertexBufferId_;

        // this holds the index buffer's id
        unsigned int indexBufferId_;

        // this holds the vertex array's id
        // the vertex array holds information about the size, shape, and type of array
        unsigned int vertexArrayId_;

        OpenGLTexture* texturePtrs_[s_numberOfTextureSlots_];

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