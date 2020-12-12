#pragma once

#include <string>
#include <map>



namespace Project001
{	
	struct Event;
	class Renderer;
	class Stores;
	class Widget;
	class Window;
	
	class Application
	{
	public:
		Application(const char* windowTitile, unsigned int windowWidth, unsigned int windowHeight);
		virtual ~Application();

		Application(Application& other) = delete;
		void operator=(const Application&) = delete;

		void AddWidget(std::string widgetName, Widget* widgetPtr);

		void Run();

		void OnEvent(Event& event);

		std::string windowTitle_;
		unsigned int windowWidth_;
		unsigned int windowHeight_;

		bool running_;

		double secondsPerFrame_;

		Window* windowPtr_;

		Stores* storesPtr_;

		Renderer* rendererPtr_;

		std::map<std::string, Widget*> widgetMap_;

	protected:

	private:

	};
}