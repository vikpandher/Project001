#pragma once

#include <queue>
#include <map>

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
			}
			else
			{
				entityId = recycledEntityIds_.front();
				recycledEntityIds_.pop();

				entityDeletedFlags_[entityId] = false;
			}

			return true;
		}

		bool DeleteEntity(unsigned int entityId)
		{
			if (!EntityExists(entityId))
			{
				return false;
			}

			for (int i = 0; i < componentContainers_.size(); ++i)
			{
				componentContainers_[i].DeleteComponent(entityId);
			}

			recycledEntityIds_.push(entityId);

			entityDeletedFlags_[entityId] = true;

			return true;
		}

		// Component Functions: ------------------------------------------------

		template <typename Component, typename... Args>
		bool CreateComponent(unsigned int entityId, Args... args)
		{
			if (!EntityExists(entityId))
			{
				return false;
			}

			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId) && !RegisterNewComponent(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].CreateComponent<Component>(entityId, args...);
		}

		template <typename Component>
		bool GetComponent(unsigned int entityId, Component*& component)
		{
			if (!EntityExists(entityId))
			{
				return false;
			}

			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].GetComponent<Component>(entityId, component);
		}

		template <typename Component>
		bool GetAllComponents(Component*& compoonents, size_t& count)
		{
			if (!EntityExists(entityId))
			{
				return false;
			}

			unsigned int componentTypeId = Component::typeId;
			if (!ComponentTypeExists(componentTypeId))
			{
				return false;
			}

			unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
			return componentContainers_[componentContainerIndex].GetAllComponents<Component>(compoonents, count);
		}

		template <typename Component>
		bool DeleteComponent(unsigned int entityId)
		{
			if (!EntityExists(entityId))
			{
				return false;
			}

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

		bool RegisterNewComponent(unsigned int componentTypeId)
		{
			unsigned int nextComponentContainerIndex = (unsigned int)componentContainers_.size();

			if (nextComponentContainerIndex > s_maxTypesOfComponents_)
			{
				return false;
			}
			
			componentTypeIdToComponentContainersIndexMap_[componentTypeId] = nextComponentContainerIndex;
			componentContainers_.resize((size_t)nextComponentContainerIndex + 1);

			return true;
		}

		static const unsigned int s_maxNumberOfEntities_ = 128;
		static const unsigned int s_maxTypesOfComponents_ = 32;

		unsigned int nextHighestEntityId_;

		std::queue<unsigned int> recycledEntityIds_;

		std::vector<bool> entityDeletedFlags_;

		std::map<unsigned int, unsigned int> componentTypeIdToComponentContainersIndexMap_;

		std::vector<ComponentContainer> componentContainers_;
	};
}