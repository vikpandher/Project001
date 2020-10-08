#pragma once

#include "Engine/Widget.h"

#include <string>
#include <map>


namespace Project001
{
	class Application;
	struct Event;
	class Renderer;
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
		Application* applicationPtr_;

		Window* windowPtr_;

		Stores* storesPtr_;

		Renderer* rendererPtr_;

		std::map<std::string, Widget*>* widgetContainerPtr_;
        
	private:
		
	};
}