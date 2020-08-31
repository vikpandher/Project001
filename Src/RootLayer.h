#pragma once

#include "Engine/Layer.h"

#include "Engine/Application.h"


namespace Project001
{
	struct Event;
	
	class RootLayer : public Layer
	{
	public:
		RootLayer(Application* parentApplicationPtr);
		virtual ~RootLayer();

		RootLayer(RootLayer& other) = delete;
		void operator=(const RootLayer&) = delete;

		void OnEvent(Event& event) override;
		void OnUpdate(double frameTimestep) override;

	protected:
		Application* parentApplicationPtr_;

		std::vector<Layer*> layerStack_;

	private:
		
	};
}