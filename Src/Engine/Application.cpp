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
	}

	void Application::OnEvent(Event& event)
	{
		Logger::Message("%s", EventToString(event).c_str());

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

	void Application::Run()
	{
		float lastFrameTime = 0.0f; //(float)glfwGetTime();

		running_ = true;
		while (running_)
		{
			float currentFrameTime = 0.0f; //(float)glfwGetTime();
			float frameTimestep = currentFrameTime - lastFrameTime;
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

	std::string Application::EventToString(Event& event) const
	{
		std::string messageString;
		messageString += EventTypeToString(event.GetEventType());
		if (event.GetEventType() == EventType::EVENT_TYPE_KEY)
		{
			KeyEvent keyEvent = dynamic_cast<KeyEvent&>(event);
			messageString += " : KeyCode = ";
			messageString += KeyCodeToString(keyEvent.keyCode);
			messageString += ", ButtonAction = ";
			messageString += ButtonActionToString(keyEvent.buttonAction);
			messageString += ", KeyModifier = ";
			messageString += KeyModifierToString(keyEvent.keyModifier);
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_MOUSE_BUTTON)
		{
			MouseButtonEvent mouseButtonEvent = dynamic_cast<MouseButtonEvent&>(event);
			messageString += " : MouseButton = ";
			messageString += MouseButtonToString(mouseButtonEvent.mouseButton);
			messageString += ", ButtonAction = ";
			messageString += ButtonActionToString(mouseButtonEvent.buttonAction);
			messageString += ", KeyModifier = ";
			messageString += KeyModifierToString(mouseButtonEvent.keyModifier);
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_CURSOR_POS)
		{
			CursorPosEvent cursorPosEvent = dynamic_cast<CursorPosEvent&>(event);
			messageString += " : xPos = ";
			messageString += std::to_string(cursorPosEvent.xPos);
			messageString += " : yPos = ";
			messageString += std::to_string(cursorPosEvent.yPos);
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_CURSOR_ENTER)
		{
			CursorEnterEvent cursorEnterEvent = dynamic_cast<CursorEnterEvent&>(event);
			messageString += " : entered = ";
			messageString += std::to_string(cursorEnterEvent.entered);
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_SCROLL)
		{
			ScrollEvent scrollEvent = dynamic_cast<ScrollEvent&>(event);
			messageString += " : xOffset = ";
			messageString += std::to_string(scrollEvent.xOffset);
			messageString += " : yOffset = ";
			messageString += std::to_string(scrollEvent.yOffset);
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_WINDOW_FOCUS)
		{
			WindowFocusEvent windowFocusEvent = dynamic_cast<WindowFocusEvent&>(event);
			messageString += " : focused = ";
			messageString += std::to_string(windowFocusEvent.focused);
		}
		else if (event.GetEventType() == EventType::EVENT_TYPE_FRAMEBUFFER_SIZE)
		{
			FrameBufferSizeEvent frameBufferSizeEvent = dynamic_cast<FrameBufferSizeEvent&>(event);
			messageString += " : width = ";
			messageString += std::to_string(frameBufferSizeEvent.width);
			messageString += " : height = ";
			messageString += std::to_string(frameBufferSizeEvent.height);
		}
		return messageString;
	}
}