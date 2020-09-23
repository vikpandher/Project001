#include <functional>

#include "Application.h"
#include "EventUtilities.h"
#include "Widget.h"
#include "Logger.h"
#include "Renderer.h"
#include "Window.h"

#include "../Platform/OpenGLRenderer.h"
#include "../Platform/OpenGLWindow.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	Application::Application(const char* windowTitle, unsigned int windowWidth, unsigned int windowHeight)
		: windowTitle_(windowTitle)
		, windowWidth_(windowWidth)
		, windowHeight_(windowHeight)
		, running_(false)
	{
		windowPtr_ = new OpenGLWindow(windowTitle, windowWidth, windowHeight);
		windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		rendererPtr_ = new OpenGLRenderer();
	}

	Application::~Application()
	{
		for (std::map<std::string, Widget*>::iterator iterator = widgetContainer_.begin(); iterator != widgetContainer_.end(); ++iterator)
		{
			delete iterator->second;
		}
		widgetContainer_.clear();
	}

	void Application::AddWidget(std::string widgetName, Widget* widgetPtr)
	{
		widgetContainer_.insert(std::make_pair(widgetName, widgetPtr));
		widgetPtr->Initialize(this);
	}

	void Application::Run()
	{
		double lastFrameTime = windowPtr_->GetTime();

		running_ = true;
		while (running_)
		{
			double currentFrameTime = windowPtr_->GetTime();
			double frameTimestep = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			for (std::map<std::string, Widget*>::iterator widgetIterator = widgetContainer_.begin(); widgetIterator != widgetContainer_.end(); ++widgetIterator)
			{
				Widget* currentWidget = widgetIterator->second;
				currentWidget->OnUpdate(frameTimestep);
			}

			windowPtr_->OnUpdate();
		}
	}

	// private --------------------------------------------------------------------

	void Application::OnEvent(Event& event)
	{		
		for (std::map<std::string, Widget*>::reverse_iterator widgetIterator = widgetContainer_.rbegin(); widgetIterator != widgetContainer_.rend(); ++widgetIterator)
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
}