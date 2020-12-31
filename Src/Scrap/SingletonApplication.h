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
		static void AddWidget(std::string widgetName, Widget* widgetPtr);

		static void Run();

		virtual ~Application();

		Application(Application& other) = delete;
		void operator=(const Application&) = delete;

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
		static Application* GetInstance();

		Application(const char* windowTitle, unsigned int windowWidth, unsigned int windowHeight);
		//Application(const Application&);
		//void operator=(const Application&);

		void OnEvent(Event& event);

		static Application* s_instance_;

	};
}