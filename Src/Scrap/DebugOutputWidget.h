#pragma once

#include "Engine/Widget.h"

#include <string>



namespace Project001
{
	struct Event;
	
	class DebugOutputWidget : public Widget
	{
	public:
		DebugOutputWidget();
		virtual ~DebugOutputWidget();

		DebugOutputWidget(DebugOutputWidget& other) = delete;
		void operator=(const DebugOutputWidget&) = delete;

		void Initialize(Application* applicationPtr) override;

		void OnEvent(Event& event) override;
		void OnUpdate(double frameTimestep) override;

	protected:
		std::string EventToString(Event& event) const;

	private:
		
	};
}