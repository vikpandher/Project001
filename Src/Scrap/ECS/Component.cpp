#include "Component.h"



namespace Project001
{
	ComponentType BaseComponent::registerComponentType(ComponentCreateFunction createFunction, ComponentFreeFunction freeFunction, size_t size)
	{
		if (types_ == nullptr)
		{
			types_ = new std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>();
		}

		ComponentType typeID = (ComponentType)types_->size();
		types_->push_back(std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>(createFunction, freeFunction, size));
		
		return typeID;
	}

	std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>* BaseComponent::types_;

	template<typename T>
	ComponentIndex ComponentCreate(ComponentMemory& memory, EntityHandle entityHandle, BaseComponent* baseComponentPtr)
	{
		MemoryIndex index = memory.size();
		memory.resize(index + T::size);
		T* componentPtr new(&memory[index]) T(*(T*)baseComponentPtr);
		componentPtr->entity = entityHandle;
		return index;
	}

	template<typename T>
	void ComponentFree(BaseComponent* baseComponentPtr)
	{
		T* componentPtr = (T*)baseComponentPtr;
		componentPtr->~T();
	}

	template<typename T>
	const ComponentCreateFunction Component<T>::CreateFunction(ComponentCreate<T>);

	template<typename T>
	const ComponentFreeFunction Component<T>::FreeFunction(ComponentFree<T>);

	template<typename T>
	const size_t Component<T>::size(sizeof(T));

	template<typename T>
	const ComponentType Component<T>::type(BaseComponent::registerComponentType(ComponentCreate<T>, ComponentFree<T>, sizeof(T)));

}