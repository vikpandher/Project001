#include "System.h"

#include "Engine/Component.h"



namespace Project001
{
	// public: -----------------------------------------------------------------
	
	BaseSystem::BaseSystem()
	{

	}

	void BaseSystem::UpdateComponents(double timestep_s, BaseComponent** componentArrays)
	{

	}

	bool BaseSystem::IsValid()
	{
		for (unsigned int i = 0; i < componentFlags_.size(); ++i)
		{
			if ((componentFlags_[i] & (glm::uint32)BaseSystem::ComponentFlag::FLAG_OPTIONAL) == 0)
			{
				return true;
			}
		}
		return false;
	}

	// protected: --------------------------------------------------------------

	void BaseSystem::AddComponentType(ComponentType type, ComponentFlag flag)
	{
		componentTypes_.push_back(type);
		componentFlags_.push_back((glm::uint32)flag);
	}

	//

	// public: -----------------------------------------------------------------

	bool SystemList::AddSystem(BaseSystem& system)
	{
		if (system.IsValid())
		{
			return false;
		}
		systemPtrs_.push_back(&system);
		return true;
	}

	bool SystemList::RemoveSystem(BaseSystem& system)
	{
		for (unsigned int i = 0; i < systemPtrs_.size(); ++i)
		{
			if (&system == systemPtrs_[i])
			{
				systemPtrs_.erase(systemPtrs_.begin() + i);
				return true;
			}
		}
		return false;
	}
}