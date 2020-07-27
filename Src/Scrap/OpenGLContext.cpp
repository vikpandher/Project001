#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../Engine/Logger.h"
#include "OpenGLContext.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	OpenGLContext::OpenGLContext(GLFWwindow* glfwWindow)
		: glfwWindow_(glfwWindow)
	{
		glfwMakeContextCurrent(glfwWindow_);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Logger::Error("Failed to initialize Glad!");
		}
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(glfwWindow_);
	}
}