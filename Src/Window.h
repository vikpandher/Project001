#pragma once



namespace Project001
{
	class Window
	{
	public:
		Window();
		~Window();

	protected:

	private:
		Window(const Window& rhs);
		Window& operator=(const Window& rhs) { return *this; }
	};
}