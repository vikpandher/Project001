#pragma once

#include <map>

#include "ComponentContainer.h"



namespace Project001
{
	class ComponentStores
	{
	public:
		ComponentStores()
		{
		}

		~ComponentStores()
		{
		}

		ComponentStores(ComponentStores& other) = delete;
		void operator=(const ComponentStores&) = delete;

		template <typename Component, typename... Args>
		bool CreateComponent(unsigned int entityId, Args... args)
		{
			unsigned int componentTypeId = Component.typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				RegisterNewComponent(componentTypeId);
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[Component.typeId];
			return componentContainers_[componentContainerIndex].CreateComponent(entityId, args...);
		}

		template <typename Component>
		bool GetComponent(unsigned int entityId, Component*& component)
		{
			unsigned int componentTypeId = Component.typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[Component.typeId];
			return componentContainers_[componentContainerIndex].GetComponent(entityId, component);
		}

	protected:

	private:
		inline bool ComponentTypeExists(unsigned int componentTypeId) const
		{
			return componentTypeIdToComponentContainersIndexMap_.find(componentTypeId) != componentTypeIdToComponentContainersIndexMap_.end();
		}

		inline void RegisterNewComponent(unsigned int componentTypeId)
		{
			componentTypeIdToComponentContainersIndexMap_[componentTypeId] = (unsigned int)componentContainers_.size();
			componentContainers_.resize(componentContainers_.size() + 1);
		}

		std::map<unsigned int, unsigned int> componentTypeIdToComponentContainersIndexMap_;
		std::vector<ComponentContainer> componentContainers_;
	};
}