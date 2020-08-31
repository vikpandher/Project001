
#include "RootLayer.h"

#include "Engine/EventUtilities.h"
#include "Engine/Logger.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	RootLayer::RootLayer(Application* parentApplicationPtr)
	{
		parentApplicationPtr = parentApplicationPtr;
	}

	RootLayer::~RootLayer()
	{

	}

	void RootLayer::OnEvent(Event& event)
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

	void RootLayer::OnUpdate(double frameTimestep)
	{
		for (std::vector<Layer*>::iterator layerIterator = layerStack_.begin(); layerIterator != layerStack_.end(); ++layerIterator)
		{
			Layer* currentLayer = *layerIterator;
			currentLayer->OnUpdate(frameTimestep);
		}
	}
}