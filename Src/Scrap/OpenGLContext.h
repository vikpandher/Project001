#pragma once

#include "../Engine/RendererContext.h"



struct GLFWwindow;

namespace Project001
{
	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* glfwWindow);
		virtual ~OpenGLContext();

		OpenGLContext(OpenGLContext& other) = delete;
		void operator=(const OpenGLContext&) = delete;

		virtual void SwapBuffers() override;

	protected:
		GLFWwindow* glfwWindow_;

	private:
		
	};
}