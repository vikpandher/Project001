#pragma once

#include <vector>



namespace Project001
{
	class ComponentContainer
	{
	public:
		ComponentContainer();
		~ComponentContainer();

		ComponentContainer(ComponentContainer& other) = delete;
		void operator=(const ComponentContainer&) = delete;

	protected:

	private:

	};

}