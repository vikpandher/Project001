#pragma once

#include <bitset>
#include <map>
#include <queue>

#include "ComponentContainer.h"



namespace Project001
{
	class ComponentStores
	{
	public:
		ComponentStores()
			: nextHighestEntityId_(0)
		{
		}

		~ComponentStores()
		{
		}

		ComponentStores(ComponentStores& other) = delete;
		void operator=(const ComponentStores&) = delete;

		// Entity Functions: ---------------------------------------------------

		bool CreateEntity(unsigned int& entityId)
		{
			if (recycledEntityIds_.empty())
			{
				if (nextHighestEntityId_ > s_maxNumberOfEntities_)
				{
					return false;
				}
				
				entityId = nextHighestEntityId_++;

				entityDeletedFlags_.push_back(false);
				entityBitmasks_.push_back(std::bitset<s_maxTypesOfComponents>());
			}
			else
			{
				entityId = recycledEntityIds_.front();
				recycledEntityIds_.pop();

				entityDeletedFlags_[entityId] = false;
				entityBitmasks_[entityId].reset();
			}

			return true;
		}

		bool DeleteEntity(unsigned int& entityId)
		{
			if (!EntityExists(entityId))
			{
				return false;
			}

			// TODO: Delete all components that belong to the entity
			////for (int i = 0; i < componentContainers_.size(); ++i)
			////{
			////	componentContainers_[i].DeleteComponent(entityId); // This won't work...
			////}

			recycledEntityIds_.push(entityId);

			entityDeletedFlags_[entityId] = true;

			return true;
		}

		// Component Functions: ------------------------------------------------

		template <typename Component, typename... Args>
		bool CreateComponent(unsigned int entityId, Args... args)
		{
			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				RegisterNewComponent(componentTypeId);
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].CreateComponent(entityId, args...);
		}

		template <typename Component>
		bool GetComponent(unsigned int entityId, Component*& component)
		{
			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].GetComponent(entityId, component);
		}

		template <typename Component>
		bool GetAllComponents(Component*& compoonents, size_t& count)
		{
			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].GetAllComponents(compoonents, count);
		}

		template <typename Component>
		bool DeleteComponent(unsigned int entityId)
		{
			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].DeleteComponent(entityId);
		}

	protected:

	private:
		inline bool EntityExists(unsigned int entityId) const
		{
			return entityId < nextHighestEntityId_ && !entityDeletedFlags_[entityId];
		}

		inline bool ComponentTypeExists(unsigned int componentTypeId) const
		{
			return componentTypeIdToComponentContainersIndexMap_.find(componentTypeId) != componentTypeIdToComponentContainersIndexMap_.end();
		}

		inline bool RegisterNewComponent(unsigned int componentTypeId)
		{
			componentTypeIdToComponentContainersIndexMap_[componentTypeId] = (unsigned int)componentContainers_.size();
			componentContainers_.resize(componentContainers_.size() + 1);

			return;
		}

		static const unsigned int s_maxNumberOfEntities_ = 128;
		static const unsigned int s_maxTypesOfComponents = 64;

		static unsigned int s_numberOfRegisteredComponents_;

		unsigned int nextHighestEntityId_;

		std::queue<unsigned int> recycledEntityIds_;

		std::vector<bool> entityDeletedFlags_;

		std::vector<std::bitset<s_maxTypesOfComponents>> entityBitmasks_;

		std::map<unsigned int, unsigned int> componentTypeIdToComponentContainersIndexMap_;

		std::vector<ComponentContainer> componentContainers_;
	};

	unsigned int ComponentStores::s_numberOfRegisteredComponents_ = 0;
}