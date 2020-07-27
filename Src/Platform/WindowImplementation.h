#pragma once

#include <string>

#include "../Engine/Window.h"



struct GLFWwindow;

namespace Project001
{
	class WindowImplementation : public Window
	{
	public:
		WindowImplementation(const char* title, int width, int height);
		~WindowImplementation() override;

		WindowImplementation(WindowImplementation& other) = delete;
		void operator=(const WindowImplementation&) = delete;

		void OnUpdate() override;

		void SetEventCallback(const std::function<void(Event&)>& callback) override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	protected:
		GLFWwindow* glfwWindow_;

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

	inline void WindowImplementation::SetEventCallback(const std::function<void(Event&)>& callback)
	{
		windowData_.EventCallback = callback;
	}

	inline bool WindowImplementation::IsVSync() const
	{
		return windowData_.vSyncEnabled;
	}
}