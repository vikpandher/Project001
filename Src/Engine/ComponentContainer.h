#pragma once

#include <map>
#include <vector>



namespace Project001
{
	class ComponentContainer
	{
	public:
		ComponentContainer()
			: typeId_(0)
		{
		}

		~ComponentContainer()
		{
		}

		template <typename Component, typename... Args>
		bool CreateComponent(unsigned int entityId, Args... args)
		{
			if (componentMemory_.empty())
			{
				typeId_ = Component::typeId;
			}
			else if (!TypeIdValid(Component::typeId))
			{
				return false;
			}

			if (ComponentExists(entityId))
			{
				return false;
			}

			size_t componentSize = sizeof(Component);
			size_t componentIndex = componentMemory_.size();

			componentMemory_.resize(componentIndex + componentSize);

			void* destination = componentMemory_.data() + componentIndex;

			Component* newComponent = new(&componentMemory_[componentIndex]) Component(args...);
			newComponent->entityId = entityId;

			if (entityId >= componentIndicies_.size())
			{
				componentIndicies_.resize((size_t)entityId + 1, -1);
			}

			componentIndicies_[entityId] = (int)componentIndex;

			return true;
		}

		template <typename Component>
		bool GetComponent(unsigned int entityId, Component*& component)
		{
			if (!TypeIdValid(Component::typeId) || !ComponentExists(entityId))
			{
				return false;
			}

			component = (Component*)&componentMemory_[componentIndicies_[entityId]];

			return true;
		}

		template <typename Component>
		bool GetAllComponents(Component*& compoonents, size_t& count)
		{
			if (!TypeIdValid(Component::typeId))
			{
				return false;
			}
			
			compoonents = (Component*)componentMemory_.data();
			count = componentMemory_.size() / sizeof(Component);

			return true;
		}

		template <typename Component>
		bool DeleteComponent(unsigned int entityId)
		{
			if (!TypeIdValid(Component::typeId) || !ComponentExists(entityId))
			{
				return false;
			}

			int deletedIndex = componentIndicies_[entityId];
			Component* deletedComponent = (Component*)&componentMemory_[deletedIndex];

			size_t componentSize = sizeof(Component);
			int lastIndex = (int)componentMemory_.size() - (int)componentSize;
			Component* lastComponent = (Component*)&componentMemory_[lastIndex];
			int lastIndexIndex = lastComponent->entityId;

			deletedComponent->~Component();
			*deletedComponent = *lastComponent;
			componentIndicies_[entityId] = -1;
			componentIndicies_[lastIndexIndex] = deletedIndex;

			componentMemory_.resize(lastIndex);

			return true;
		}

	protected:

	private:
		inline bool ComponentExists(unsigned int entityId) const
		{
			return entityId < componentIndicies_.size() && componentIndicies_[entityId] >= 0;
		}

		inline bool TypeIdValid(int testTypeId) const
		{
			return typeId_ == testTypeId;
		}

		// This is the typeId of the components stored.
		// It's used toensure the "ComponentContainer" only
		// contains one type of component at a time.
		int typeId_;

		// Components are stored in "componentMemory_".
		std::vector<unsigned char> componentMemory_;
		
		// The index of a component in "componentMemory_" is stored in
		// "componentIndicies_".
		// The indicies of "componentIndicies_" correspond with entityIds.
		std::vector<int> componentIndicies_;
	};
}