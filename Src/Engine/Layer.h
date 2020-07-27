#pragma once



namespace Project001
{
	struct Event;
	
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		Layer(Layer& other) = delete;
		void operator=(const Layer&) = delete;

		virtual void OnEvent(Event& event) = 0;
		virtual void OnUpdate(float frameTimestep) = 0;

	protected:

	private:
		
	};
}