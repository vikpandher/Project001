#include "Application.h"

#include <functional>
#include <thread> 

#include "EventUtilities.h"
#include "Logger.h"
#include "Renderer.h"
//^includes "Mesh.h"
#include "Stores.h"
//^includes "Mesh.h"
#include "Widget.h"
#include "Window.h"
//^includes "EventUtilities.h"

#include "../Platform/OpenGLRenderer.h"
#include "../Platform/OpenGLWindow.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	void Application::AddWidget(std::string widgetName, Widget* widgetPtr)
	{
		Application* instance = GetInstance();
		instance->widgetMap_.insert(std::make_pair(widgetName, widgetPtr));
		widgetPtr->Initialize(instance);
	}

	void Application::Run()
	{
		Application* instance = GetInstance();
		std::chrono::system_clock::time_point timeStampA = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point timeStampB = std::chrono::system_clock::now();
		double timeStampDifference = 0.0;
		double sleepTime = 0.0;

		instance->running_ = true;
		while (instance->running_)
		{
			timeStampA = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> workTime_ms = timeStampA - timeStampB;

			if (workTime_ms.count() < instance->secondsPerFrame_)
			{
				std::chrono::duration<double, std::milli> delta_ms(200.0 - workTime_ms.count());
				std::chrono::duration<long long, std::milli> deltaDuration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
				std::this_thread::sleep_for(std::chrono::milliseconds(deltaDuration_ms.count()));
			}

			timeStampB = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> sleepTime_ms = timeStampB - timeStampA;
			std::chrono::duration<double, std::milli> totalTime_ms = workTime_ms + sleepTime_ms;

			for (std::map<std::string, Widget*>::iterator widgetIterator = instance->widgetMap_.begin(); widgetIterator != instance->widgetMap_.end(); ++widgetIterator)
			{
				Widget* currentWidget = widgetIterator->second;
				currentWidget->OnUpdate(totalTime_ms.count() / 1000.0);
			}

			instance->windowPtr_->OnUpdate();
			instance->rendererPtr_->Render();
		}
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

		if (rendererPtr_ != nullptr)
		{
			delete rendererPtr_;
		}
		
		for (std::map<std::string, Widget*>::iterator iterator = widgetMap_.begin(); iterator != widgetMap_.end(); ++iterator)
		{
			delete iterator->second;
		}
		widgetMap_.clear();
	}

	// private --------------------------------------------------------------------

	Application* Application::GetInstance()
	{
		if (s_instance_ == nullptr)
		{
			s_instance_ = new Application("Project001", 800, 600);
		}
		return s_instance_;
	}

	Application::Application(const char* windowTitle, unsigned int windowWidth, unsigned int windowHeight)
		: windowTitle_(windowTitle)
		, windowWidth_(windowWidth)
		, windowHeight_(windowHeight)
		, running_(false)
		, secondsPerFrame_(1.0 / 60.0)
	{
		windowPtr_ = new OpenGLWindow(windowTitle, windowWidth, windowHeight);
		windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		storesPtr_ = new Stores();

		rendererPtr_ = new OpenGLRenderer();
	}

	void Application::OnEvent(Event& event)
	{		
		for (std::map<std::string, Widget*>::reverse_iterator widgetIterator = widgetMap_.rbegin(); widgetIterator != widgetMap_.rend(); ++widgetIterator)
		{
			Widget* currentWidget = widgetIterator->second;
			currentWidget->OnEvent(event);

			if (event.handled)
			{
				break;
			}
		}

		if (event.GetEventType() == EventType::EVENT_TYPE_WINDOW_CLOSE)
		{
			WindowCloseEvent windowFocusEvent = dynamic_cast<WindowCloseEvent&>(event);
			running_ = false;
			windowFocusEvent.handled = true;
		}
	}

	Application* Application::s_instance_ = nullptr;
}