#pragma once

#include <string>

#include "../Engine/Window.h"
//^includes "EventUtilities.h"



struct GLFWwindow;

namespace Project001
{
	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const char* title, int width, int height);
		~OpenGLWindow();

		OpenGLWindow(OpenGLWindow& other) = delete;
		void operator=(const OpenGLWindow&) = delete;

		void OnUpdate() override;

		void SetViewportSize(int lowerLeftX, int lowerLeftY, int width, int height) override;

		void SetEventCallback(const std::function<void(Event&)>& callback) override;

		// Setting numerator and denominator to -1 unlocks the aspect ratio.
		void SetAspectRatio(int numerator, int denominator);

		void SetSize(int width, int height) override;

		// Time measurements are in seconds
		void SetTime(const double time) override;
		double GetTime() const override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	protected:
		GLFWwindow* glfwWindowPtr_;

		struct WindowData
		{
			WindowData()
				: width(0)
				, height(0)
				, vSyncEnabled(false)
			{}

			std::string title;
			unsigned int width;
			unsigned int height;

			std::function<void(Event&)> EventCallback;

			bool vSyncEnabled;

		} windowData_;

	private:
		static int s_glfwWindowCount_;
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