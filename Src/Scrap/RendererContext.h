#pragma once



namespace Project001
{
	class RendererContext
	{
	public:
		RendererContext();
		virtual ~RendererContext();

		RendererContext(RendererContext& other) = delete;
		void operator=(const RendererContext&) = delete;

		virtual void SwapBuffers() = 0;

	protected:

	private:
		
	};
}