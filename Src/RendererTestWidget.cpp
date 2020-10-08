
#include "RendererTestWidget.h"

#include "Engine/Application.h"
#include "Engine/EventUtilities.h"
#include "Engine/Logger.h"
#include "Engine/Renderer.h"
#include "Engine/Stores.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	RendererTestWidget::RendererTestWidget()
		: applicationPtr_(nullptr)
		, windowPtr_(nullptr)
		, storesPtr_(nullptr)
		, rendererPtr_(nullptr)
		, widgetContainerPtr_(nullptr)
	{

	}

	RendererTestWidget::~RendererTestWidget()
	{

	}

	void RendererTestWidget::Initialize(Application* applicationPtr)
	{
		applicationPtr_ = applicationPtr;
		windowPtr_ = applicationPtr->windowPtr_;
		storesPtr_ = applicationPtr->storesPtr_;
		rendererPtr_ = applicationPtr->rendererPtr_;
		widgetContainerPtr_ = &(applicationPtr->widgetMap_);

		storesPtr_->LoadOBJFile("Cube", "../Models/Cube.obj");
		rendererPtr_->AddMesh(storesPtr_->GetMesh("Cube"));
	}

	void RendererTestWidget::OnEvent(Event& event)
	{
		
	}

	void RendererTestWidget::OnUpdate(double frameTimestep)
	{
		applicationPtr_->rendererPtr_->Render();
	}
}