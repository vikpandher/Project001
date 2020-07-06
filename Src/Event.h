#pragma once



namespace Project001
{
	// NOTES:
	// GLFW Callback Functions:
	// ----------------------------------------------------------------------------
	// ERROR CALLBACKS: https://www.glfw.org/docs/3.3/group__init.html
	// GLFWerrorfun glfwSetErrorCallback (GLFWerrorfun callback)
	// 
	// INPUT CALLBACKS: https://www.glfw.org/docs/3.3/group__input.html
	// GLFWkeyfun glfwSetKeyCallback (GLFWwindow *window, GLFWkeyfun callback)
	// GLFWcharfun glfwSetCharCallback (GLFWwindow *window, GLFWcharfun callback)
	// GLFWcharmodsfun glfwSetCharModsCallback (GLFWwindow *window, GLFWcharmodsfun callback)
	// GLFWmousebuttonfun glfwSetMouseButtonCallback (GLFWwindow *window, GLFWmousebuttonfun callback)
	// GLFWcursorposfun glfwSetCursorPosCallback (GLFWwindow *window, GLFWcursorposfun callback)
	// GLFWcursorenterfun glfwSetCursorEnterCallback (GLFWwindow *window, GLFWcursorenterfun callback)
	// GLFWscrollfun glfwSetScrollCallback (GLFWwindow *window, GLFWscrollfun callback)
	// GLFWdropfun glfwSetDropCallback (GLFWwindow *window, GLFWdropfun callback)
	// GLFWjoystickfun glfwSetJoystickCallback (GLFWjoystickfun callback)
	// 
	// MONITOR CALLBACKS: https://www.glfw.org/docs/3.3/group__monitor.html
	// GLFWmonitorfun glfwSetMonitorCallback (GLFWmonitorfun callback)
	// 
	// WINDOW CALLBACKS: https://www.glfw.org/docs/3.3/group__window.html
	// GLFWwindowposfun glfwSetWindowPosCallback (GLFWwindow *window, GLFWwindowposfun callback)
	// GLFWwindowsizefun glfwSetWindowSizeCallback (GLFWwindow *window, GLFWwindowsizefun callback)
	// GLFWwindowclosefun glfwSetWindowCloseCallback (GLFWwindow *window, GLFWwindowclosefun callback)
	// GLFWwindowrefreshfun glfwSetWindowRefreshCallback (GLFWwindow *window, GLFWwindowrefreshfun callback)
	// GLFWwindowfocusfun glfwSetWindowFocusCallback (GLFWwindow *window, GLFWwindowfocusfun callback)
	// GLFWwindowiconifyfun glfwSetWindowIconifyCallback (GLFWwindow *window, GLFWwindowiconifyfun callback)
	// GLFWwindowmaximizefun glfwSetWindowMaximizeCallback (GLFWwindow *window, GLFWwindowmaximizefun callback)
	// GLFWframebuffersizefun glfwSetFramebufferSizeCallback (GLFWwindow *window, GLFWframebuffersizefun callback)
	// GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback (GLFWwindow *window, GLFWwindowcontentscalefun callback)

	enum EventType
	{
		EVENT_TYPE_NONE = 0
	};
	
	class Event
	{
	public:
		Event();
		~Event();

		virtual EventType GetEventType() const = 0;


	protected:

	private:
		Event(const Event& rhs);
		Event& operator=(const Event& rhs) { return *this; }
	};
}