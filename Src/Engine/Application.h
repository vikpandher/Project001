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
	
	// singleton class for Application
	class Application
	{
	public:
		Application(Window* windowPtr);
		virtual ~Application();

		Application(Application& other) = delete;
		void operator=(const Application&) = delete;

		void OnEvent(Event& event);

		void Run();

	protected:
		std::string EventToString(Event& event) const;

		Window* windowPtr_;

		std::string windowTitle_;
		unsigned int windowWidth_;
		unsigned int windowHeight_;

		bool running_;

		std::vector<Layer*> layerStack_;

	private:

	};
}