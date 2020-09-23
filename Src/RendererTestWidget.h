#pragma once

#include "Engine/Widget.h"

#include <string>



namespace Project001
{
	class Application;
	struct Event;
	
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
        
	private:
		
	};
}