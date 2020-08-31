#pragma once



namespace Project001
{
	struct Event;
	
	class Layer
	{
	public:
		virtual void OnEvent(Event& event) = 0;
		virtual void OnUpdate(double frameTimestep) = 0;

	protected:

	private:
		
	};
}