
#include "RendererTestWidget.h"

#include "Engine/Application.h"
#include "Engine/EventUtilities.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	RendererTestWidget::RendererTestWidget()
		: applicationPtr_(nullptr)
	{

	}

	RendererTestWidget::~RendererTestWidget()
	{

	}

	void RendererTestWidget::Initialize(Application* applicationPtr)
	{
		applicationPtr_ = applicationPtr;
	}

	void RendererTestWidget::OnEvent(Event& event)
	{
		
	}

	void RendererTestWidget::OnUpdate(double frameTimestep)
	{
		applicationPtr_->rendererPtr_->Render();
	}
}