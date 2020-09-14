#pragma once



namespace Project001
{
	class Widget;

	class Renderer
	{
	public:
		virtual void Render() = 0;
		virtual void SubmitWidget(Widget* widget) = 0;

	protected:

	private:
		
	};
}