#pragma once

#include <string>
#include <functional>



struct GLFWwindow;

namespace Project001
{
	struct WindowData
	{
		std::string title;
		unsigned int width, height;
		
		std::function<void(GLFWwindow*, int, int)> WindowSizeCallback;
	};
}