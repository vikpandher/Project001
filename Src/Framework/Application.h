#pragma once



namespace Project001
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	protected:

	private:
		Application(const Application& rhs);
		Application& operator=(const Application& rhs) { return *this; }
	};
}