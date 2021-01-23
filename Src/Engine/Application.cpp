#include "Application.h"

#include <functional>
#include <thread> 

#include "Engine/Event.h"
#include "Engine/ResourceStores.h"
#include "Engine/Window.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	Application::Application(const char* windowTitle, unsigned int windowWidth, unsigned int windowHeight)
		: windowTitle_(windowTitle)
		, windowWidth_(windowWidth)
		, windowHeight_(windowHeight)
		, running_(false)
		, secondsPerFrame_(1.0 / 60.0)
	{
		windowPtr_ = Window::Create(windowTitle, windowWidth, windowHeight);
		windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		storesPtr_ = new ResourceStores();
	}

	Application::~Application()
	{
		if (windowPtr_ != nullptr)
		{
			delete windowPtr_;
		}

		if (storesPtr_ != nullptr)
		{
			delete storesPtr_;
		}
	}

	void Application::Run()
	{
		std::chrono::system_clock::time_point timeStampA = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point timeStampB = std::chrono::system_clock::now();
		double timeStampDifference = 0.0;
		double sleepTime = 0.0;

		running_ = true;
		while (running_)
		{			
			timeStampA = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> workTime_ms = timeStampA - timeStampB;

			if (workTime_ms.count() < secondsPerFrame_)
			{
				std::chrono::duration<double, std::milli> delta_ms(200.0 - workTime_ms.count());
				std::chrono::duration<long long, std::milli> deltaDuration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
				std::this_thread::sleep_for(std::chrono::milliseconds(deltaDuration_ms.count()));
			}

			timeStampB = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> sleepTime_ms = timeStampB - timeStampA;
			std::chrono::duration<double, std::milli> totalTime_ms = workTime_ms + sleepTime_ms;

			windowPtr_->PollEvents();
			OnEvent(UpdateEvent(totalTime_ms.count() / 1000.0));
		}
	}

	// private --------------------------------------------------------------------

	void Application::OnEvent(Event& event)
	{		
		if (event.GetEventType() == EventType::EVENT_TYPE_WINDOW_CLOSE)
		{
			WindowCloseEvent windowFocusEvent = dynamic_cast<WindowCloseEvent&>(event);
			running_ = false;
			windowFocusEvent.handled = true;
		}
	}
}