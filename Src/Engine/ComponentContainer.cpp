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

    bool ComponentContainer::DeleteComponent(unsigned int entityId)
    {
        if (!ComponentExists(entityId))
        {
            return false;
        }

        int deletedIndex = componentIndicies_[entityId];
        void* deletedComponent = (void*)&componentMemory_[deletedIndex];

        int lastIndex = (int)(componentMemory_.size() - componentSize_);
        BaseComponent* lastComponent = (BaseComponent*)&componentMemory_[lastIndex];
        int lastIndexIndex = lastComponent->entityId;

        ComponentDestructionFunction(deletedComponent);
        ::memcpy(deletedComponent, lastComponent, componentSize_);
        componentIndicies_[entityId] = -1;
        componentIndicies_[lastIndexIndex] = deletedIndex;

        componentMemory_.resize(lastIndex);

        return true;
    }
}