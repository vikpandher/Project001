#pragma once



namespace Project001
{
	class Application
	{
	public:
		Application();
		~Application();

	protected:

	private:
		Application(const Application& rhs);
		Application& operator=(const Application& rhs) { return *this; }
	};
}