#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Engine/EntityComponentSystemTypes.h"



namespace Project001
{
	struct BaseComponent;

	class BaseSystem
	{
	public:
		enum class ComponentFlag
		{
			FLAG_OPTIONAL = 1
		};

		BaseSystem();

		BaseSystem(BaseSystem& other) = delete;
		void operator=(const BaseSystem&) = delete;

		virtual void UpdateComponents(double timestep_s, BaseComponent** componentArrays);

		const std::vector<ComponentType> GetComponentTypes() const;

		const std::vector<glm::uint32> GetComponentFlags() const;

		bool IsValid();

	protected:
		void AddComponentType(ComponentType type, ComponentFlag flag);

	private:
		std::vector<ComponentType> componentTypes_;
		std::vector<glm::uint32> componentFlags_;
	};

	inline const std::vector<ComponentType> BaseSystem::GetComponentTypes() const
	{
		return componentTypes_;
	}

	inline const std::vector<glm::uint32> BaseSystem::GetComponentFlags() const
	{
		return componentFlags_;
	}

	class SystemList
	{
	public:
		bool AddSystem(BaseSystem& syatem);
		bool RemoveSystem(BaseSystem& system);

		inline size_t Size();

		inline BaseSystem* operator[](glm::uint32 index);

	private:
		std::vector<BaseSystem*> systemPtrs_;
	};

	size_t SystemList::Size()
	{
		return systemPtrs_.size();
	}

	BaseSystem* SystemList::operator[](glm::uint32 index)
	{
		return systemPtrs_[index];
	}
}