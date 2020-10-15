#include "Application.h"

#include <functional>

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

	Application::Application(const char* windowTitle, unsigned int windowWidth, unsigned int windowHeight)
		: windowTitle_(windowTitle)
		, windowWidth_(windowWidth)
		, windowHeight_(windowHeight)
		, running_(false)
	{
		windowPtr_ = new OpenGLWindow(windowTitle, windowWidth, windowHeight);
		windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		storesPtr_ = new Stores();

		rendererPtr_ = new OpenGLRenderer();
	}

	Application::~Application()
	{
		for (std::map<std::string, Widget*>::iterator iterator = widgetMap_.begin(); iterator != widgetMap_.end(); ++iterator)
		{
			delete iterator->second;
		}
		widgetMap_.clear();
	}

	void Application::AddWidget(std::string widgetName, Widget* widgetPtr)
	{
		widgetMap_.insert(std::make_pair(widgetName, widgetPtr));
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

			for (std::map<std::string, Widget*>::iterator widgetIterator = widgetMap_.begin(); widgetIterator != widgetMap_.end(); ++widgetIterator)
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
}