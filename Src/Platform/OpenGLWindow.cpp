#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "OpenGLWindow.h"
#include "../Engine/Logger.h"



namespace Project001
{	
	// public ------------------------------------------------------------------

	OpenGLWindow::OpenGLWindow(const char* title, int width, int height)
		: glfwWindow_(nullptr)
	{
		windowData_.title = title;
		windowData_.width = width;
		windowData_.height = height;
		
		if (s_glfwWindowCount_ == 0)
		{
			if (!glfwInit())
			{
				Logger::Error("Could not initalize GLFW!");
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfwSetErrorCallback([](int errorCode, const char* description)
				{
					Logger::Error("GLFW Error: (%d) %s", errorCode, description);
				});
		}

		glfwWindow_ = glfwCreateWindow(width, height, title, NULL, NULL);
		s_glfwWindowCount_++;

		glfwSetWindowUserPointer(glfwWindow_, &windowData_);

		glfwMakeContextCurrent(glfwWindow_);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Logger::Error("Failed to initialize Glad!");
		}

		Logger::Message("OpenGL Info:");
		Logger::Message("    Vendor: %s", glGetString(GL_VENDOR));
		Logger::Message("    Renderer: %s", glGetString(GL_RENDERER));
		Logger::Message("    Version: %s", glGetString(GL_VERSION));

		SetVSync(true);

		glfwSetKeyCallback(glfwWindow_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyEvent event((KeyCode)key, (ButtonAction)action, (KeyModifier)mods);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(glfwWindow_, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseButtonEvent event((MouseButton)button, (ButtonAction)action, (KeyModifier)mods);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(glfwWindow_, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				CursorPosEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			});

		glfwSetCursorEnterCallback(glfwWindow_, [](GLFWwindow* window, int entered)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				CursorEnterEvent event((bool)entered);
				data.EventCallback(event);
			});

		glfwSetScrollCallback(glfwWindow_, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				ScrollEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(glfwWindow_, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetWindowFocusCallback(glfwWindow_, [](GLFWwindow* window, int focused)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowFocusEvent event((bool)focused);
				data.EventCallback(event);
			});

		glfwSetFramebufferSizeCallback(glfwWindow_, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				FrameBufferSizeEvent event(width, height);
				data.EventCallback(event);
			});
	}

	OpenGLWindow::~OpenGLWindow()
	{
		glfwDestroyWindow(glfwWindow_);

		if (s_glfwWindowCount_ == 0)
		{
			glfwTerminate();
		}
	}

	void OpenGLWindow::OnUpdate()
	{		
		// make current context again incase it was changed by another window
		glfwMakeContextCurrent(glfwWindow_);
		
		glfwSwapBuffers(glfwWindow_);

		glfwPollEvents();
	}

	inline void OpenGLWindow::SetTime(const double time)
	{
		glfwSetTime(time);
	}

	inline double OpenGLWindow::GetTime() const
	{
		return glfwGetTime();
	}

	void OpenGLWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		windowData_.vSyncEnabled = enabled;
	}

	// private -----------------------------------------------------------------
	int OpenGLWindow::s_glfwWindowCount_ = 0;
}