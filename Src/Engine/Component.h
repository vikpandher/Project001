#pragma once

#include <string>



namespace Project001
{
	const int COMPONENT_TYPE_ID_TEST = 100;

	static std::string ComponentTypeToString(int componentTypeId)
	{
		switch (componentTypeId)
		{
		case COMPONENT_TYPE_ID_TEST:
		{
			return std::string("TestComponent");
			break;
		}
		}
		return std::string("UNKNOWN");
	}

#define COMPONENT(uniqueComponentTypeId)\
	const static int typeId = uniqueComponentTypeId;\
	unsigned int entityId = 0;

	// Components:
	// ----------------------------------------------------------------------------
	// * Components must use the "COMPONENT" macro given a unique id.

	struct TestComponent
	{
		COMPONENT(COMPONENT_TYPE_ID_TEST)

		TestComponent(int a, int b, int c)
			: a(a)
			, b(b)
			, c(c)
		{}

		int a;
		int b;
		int c;
	};
}