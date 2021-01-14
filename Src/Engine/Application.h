#pragma once

#include <string>



namespace Project001
{	
	struct Event;
	class Scene;
	class ResourceStores;
	class Window;
	
	class Application
	{
	public:
		Application(const char* windowTitile, unsigned int windowWidth, unsigned int windowHeight);
		virtual ~Application();

		Application(Application& other) = delete;
		void operator=(const Application&) = delete;

		void Run();

		void OnEvent(Event& event);

		std::string windowTitle_;
		unsigned int windowWidth_;
		unsigned int windowHeight_;

		bool running_;

		double secondsPerFrame_;

		Window* windowPtr_;

		ResourceStores* storesPtr_;

	protected:

	private:

	};
}