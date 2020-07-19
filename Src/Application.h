#pragma once

#include <string>



#ifdef _WIN32
/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define _PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#endif



struct GLFWwindow;

namespace Project001
{	
	struct Event;
	class Logger;
	class Window;
	
	// singleton class for Application
	class Application
	{
	public:
		Application();
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

	private:

	};
}