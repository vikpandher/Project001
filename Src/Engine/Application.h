#pragma once

#include <string>
#include <vector>



struct GLFWwindow;

namespace Project001
{	
	struct Event;
	class Layer;
	class Logger;
	class Window;
	
	class Application
	{
	public:
		Application(Window* windowPtr);
		virtual ~Application();

		Application(Application& other) = delete;
		void operator=(const Application&) = delete;

		void AddLayer(Layer* layer);

		void Run();

	protected:

		void OnEvent(Event& event);

		Window* windowPtr_;

		std::string windowTitle_;
		unsigned int windowWidth_;
		unsigned int windowHeight_;

		bool running_;

		std::vector<Layer*> layerStack_;

	private:

	};
}