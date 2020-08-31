#pragma once

#include "Engine/Layer.h"

#include <string>



namespace Project001
{
	struct Event;
	
	class DebugLayer : public Layer
	{
	public:
		DebugLayer();
		virtual ~DebugLayer();

		DebugLayer(DebugLayer& other) = delete;
		void operator=(const DebugLayer&) = delete;

		void OnEvent(Event& event) override;
		void OnUpdate(double frameTimestep) override;

	protected:
		std::string EventToString(Event& event) const;

	private:
		
	};
}