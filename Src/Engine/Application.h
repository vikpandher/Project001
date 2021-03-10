#pragma once

#include <string>



namespace Project001
{
	class ComponentStores;
	struct Event;
	class Scene;
	class ResourceStores;
	class Window;
	struct WindowCloseEvent;
	
	class Application
	{
	public:
		Application(const char* windowTitile, unsigned int windowWidth, unsigned int windowHeight);
		virtual ~Application();

		Application(Application& other) = delete;
		void operator=(const Application&) = delete;

		void Run();

	protected:
		void Stop(WindowCloseEvent& windowCloseEvent);

		void OnEvent(Event& event);

		std::string windowTitle_;
		unsigned int windowWidth_;
		unsigned int windowHeight_;

		bool running_;

		double secondsPerFrame_;

		ComponentStores* componentStoresPtr_;

		ResourceStores* resourceStoresPtr_;

		Window* windowPtr_;

		Scene* activeScenePtr_;

	private:

	};
}