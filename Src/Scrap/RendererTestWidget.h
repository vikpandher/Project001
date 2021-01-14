#pragma once

#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

#include "Engine/Widget.h"

#include "Engine/EventUtilities.h"

#include <string>
#include <map>


namespace Project001
{
	class Application;
	struct Event;
	class Stores;
	class Window;

	class RendererTestWidget : public Widget
	{
	public:
		RendererTestWidget();
		virtual ~RendererTestWidget();

		RendererTestWidget(RendererTestWidget& other) = delete;
		void operator=(const RendererTestWidget&) = delete;

		void Initialize(Application* applicationPtr) override;

		void OnEvent(Event& event) override;
		void OnUpdate(double frameTimestep) override;

	protected:
		void OutputTestValues();

		Application* applicationPtr_;

		Window* windowPtr_;

		Stores* storesPtr_;

		std::map<std::string, Widget*>* widgetContainerPtr_;

		// Camera
		// ---------------------------------------------------------------------
		glm::vec3 cameraPosition_;

		glm::quat cameraOrientation_;

		float fieldOfVisionDegrees_;
		float aspectRatio_;
		float nearCutOff_;
		float farCutOff_;

		// Input
		// ---------------------------------------------------------------------
		KeyCode moveLeftKeyCode_;
		KeyCode moveRightKeyCode_;
		KeyCode moveUpKeyCode_;
		KeyCode moveDownKeyCode_;
		KeyCode rollLeftKeyCode_;
		KeyCode rollRightKeyCode_;

		bool movingLeft_;
		bool movingRight_;
		bool movingUp_;
		bool movingDown_;
		bool rollingLeft_;
		bool rollingRight_;

		MouseButton mouseButton1_;
		MouseButton mouseButton2_;

		bool mouseButton1Down_;
		bool mouseButton2Down_;

		float lastCursorPositionX_;
		float lastCursorPositionY_;
		float cursorPositionX_;
		float cursorPositionY_;

		float testValue00_;
		float testValue01_;

		// World Coordinate System
		// ---------------------------------------------------------------------
		static glm::vec3 s_worldForward_;
		static glm::vec3 s_worldUp_;
		static glm::vec3 s_worldRight_;
        
	private:
		
	};
}