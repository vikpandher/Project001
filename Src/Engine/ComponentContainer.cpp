#include "ComponentContainer.h"



namespace Project001
{
    ComponentContainer::ComponentContainer()
        : typeId_(0)
        , componentSize_(0)
        , ComponentDestructionFunction(nullptr)
    {}

    ComponentContainer::~ComponentContainer()
    {}

    void ComponentContainer::DeleteAllComponents()
    {
        for (unsigned int i = 0; i < entityIdToComponentMemoryIndicies_.size(); ++i)
        {
            int deletedComponentMemoryIndex = entityIdToComponentMemoryIndicies_[i];
            if (deletedComponentMemoryIndex >= 0)
            {
                void* deletedComponent = (void*)&componentMemory_[deletedComponentMemoryIndex];
                ComponentDestructionFunction(deletedComponent);
                entityIdToComponentMemoryIndicies_[i] = -1;
            }
        }
        componentMemory_.resize(0);
        componentEntityIds_.resize(0);
    }

    bool ComponentContainer::DeleteComponent(const unsigned int entityId)
    {
        if (!ComponentExists(entityId))
        {
            return false;
        }

        int deletedComponentMemoryIndex = entityIdToComponentMemoryIndicies_[entityId];
        void* deletedComponentPtr = (void*)&componentMemory_[deletedComponentMemoryIndex];

        int lastComponentMemoryIndex = (int)(componentMemory_.size() - componentSize_);
        void* lastComponentPtr = (void*)&componentMemory_[lastComponentMemoryIndex];

        int lastComponentEntityIdIndex = lastComponentMemoryIndex / (int)componentSize_;
        unsigned int lastComponentEntityId = componentEntityIds_[lastComponentEntityIdIndex];

        ComponentDestructionFunction(deletedComponentPtr);
        ::memcpy(deletedComponentPtr, lastComponentPtr, componentSize_);
        entityIdToComponentMemoryIndicies_[entityId] = -1;
        entityIdToComponentMemoryIndicies_[lastComponentEntityId] = deletedComponentMemoryIndex;

        int deletedComponentEntityIdIndex = deletedComponentMemoryIndex / (int)componentSize_;
        componentEntityIds_[deletedComponentEntityIdIndex] = lastComponentEntityId;

        componentMemory_.resize(lastComponentMemoryIndex);
        componentEntityIds_.resize(lastComponentEntityIdIndex);

        return true;
    }
}