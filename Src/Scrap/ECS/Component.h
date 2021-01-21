#pragma once

#include <tuple>
#include <vector>

#include "glm/glm.hpp"

#include "Engine/EntityComponentSystemTypes.h"



namespace Project001
{
	struct BaseComponent;
	typedef ComponentIndex(*ComponentCreateFunction)(ComponentMemory& memory, EntityHandle entityHandle, BaseComponent* baseComponentPtr);
	typedef void(*ComponentFreeFunction)(BaseComponent* baseComponentPtr);

	// -------------------------------------------------------------------------
	struct BaseComponent
	{
	public:
		BaseComponent(BaseComponent& other) = delete;
		void operator=(const BaseComponent&) = delete;

		static ComponentType registerComponentType(ComponentCreateFunction createFunction, ComponentFreeFunction freeFunction, size_t size);
		
		EntityHandle entityHandle = nullptr;

		inline static ComponentCreateFunction GetTypeCreateFunction(ComponentType type);
		inline static ComponentFreeFunction GetTypeFreeFunction(ComponentType type);
		inline static size_t GetTypeSize(ComponentType type);
		inline static bool IsTypeValid(ComponentType type);

	private:
		// I don't like this.
		static std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>* types_;
	};

	ComponentCreateFunction BaseComponent::GetTypeCreateFunction(ComponentType type)
	{
		return std::get<0>((*types_)[type]);
	}

	ComponentFreeFunction BaseComponent::GetTypeFreeFunction(ComponentType type)
	{
		return std::get<1>((*types_)[type]);
	}

	size_t BaseComponent::GetTypeSize(ComponentType type)
	{
		return std::get<2>((*types_)[type]);
	}

	bool BaseComponent::IsTypeValid(ComponentType type)
	{
		return type < types_->size();
	}

	// -------------------------------------------------------------------------
	template<typename T>
	struct Component : public BaseComponent
	{
		static const ComponentCreateFunction CreateFunction;
		static const ComponentFreeFunction FreeFunction;
		
		static const size_t size;
		static const ComponentType type;
	};

	// -------------------------------------------------------------------------
	struct TestComponent : public Component<TestComponent>
	{
		float a;
		float b;
		float c;
	};
}