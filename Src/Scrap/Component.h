#pragma once



namespace Project001
{
	class Component
	{
	public:
		Component();
		~Component();

		Component(Component& other) = delete;
		void operator=(const Component&) = delete;

	protected:

	private:

	};

}