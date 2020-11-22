#pragma once

#include <functional>

#include "EventUtilities.h"



namespace Project001
{
	class Window
	{
	public:
		virtual void OnUpdate() = 0;

		virtual void SetViewportSize(int lowerLeftX, int lowerLeftY, int width, int height) = 0;

		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;

		virtual void SetAspectRatio(int numerator, int denominator) = 0;

		virtual void SetSize(int width, int height) = 0;
		
		virtual void SetTime(const double time) = 0;
		virtual double GetTime() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

	protected:

	private:
		
	};
}