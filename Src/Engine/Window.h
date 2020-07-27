#pragma once

#include <functional>

#include "EventUtilities.h"



namespace Project001
{
	class Window
	{
	public:
		Window();
		virtual ~Window();

		Window(Window& other) = delete;
		void operator=(const Window&) = delete;

		virtual void OnUpdate() = 0;

		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

	protected:

	private:
		
	};
}