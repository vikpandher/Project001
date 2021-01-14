#pragma once



namespace Project001
{	
	class Application;
	struct Event;
	
	class Widget
	{
	public:
		virtual void Initialize(Application* applicationPtr) = 0;

		virtual void OnEvent(Event& event) = 0;

		virtual void OnUpdate(double frameTimestep) = 0;

	protected:

	private:
		
	};
}