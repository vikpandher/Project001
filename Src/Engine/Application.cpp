#include <functional>

#include "Application.h"
#include "EventUtilities.h"
#include "Layer.h"
#include "Logger.h"
#include "Window.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	Application::Application(Window* windowPtr)
		: windowPtr_(windowPtr)
		, windowTitle_("Project001")
		, windowWidth_(800)
		, windowHeight_(600)
		, running_(false)
	{
		windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		for (std::vector<Layer*>::iterator iterator = layerStack_.begin(); iterator != layerStack_.end(); ++iterator)
		{
			delete *iterator;
		}
		layerStack_.clear();
	}

	void Application::AddLayer(Layer* layer)
	{
		layerStack_.push_back(layer);
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

			for (std::vector<Layer*>::iterator layerIterator = layerStack_.begin(); layerIterator != layerStack_.end(); ++layerIterator)
			{
				Layer* currentLayer = *layerIterator;
				currentLayer->OnUpdate(frameTimestep);
			}

			windowPtr_->OnUpdate();
		}
	}

	// protected ---------------------------------------------------------------

	void Application::OnEvent(Event& event)
	{
		for (std::vector<Layer*>::reverse_iterator layerIterator = layerStack_.rbegin(); layerIterator != layerStack_.rend(); ++layerIterator)
		{
			if (event.handled)
			{
				break;
			}

			Layer* currentLayer = *layerIterator;
			currentLayer->OnEvent(event);
		}
	}
}