// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-21

#include "ComponentContainer.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    ComponentContainer::ComponentContainer()
        : ComponentDestructionFunction_(nullptr)
        , componentTypeId_(0)
        , componentSize_(0)
        , componentMemoryPtr_(nullptr)
        , componentEntityIdMemoryPtr_(nullptr)
        , componentCapacity_(0)
        , componentCount_(0)
        , componentMemoryGrowthRate_(0)
        , componentMemoryCapacityCap_(0)
    {}

    ComponentContainer::~ComponentContainer()
    {
        if (ComponentDestructionFunction_ != nullptr)
        {
            for (size_t i = 0; i < componentCount_; ++i)
            {
                uint8_t* deletedComponent = componentMemoryPtr_ + i * componentSize_;
                ComponentDestructionFunction_(deletedComponent);
            }
        }

        ComponentDestructionFunction_ = nullptr;

        free(componentMemoryPtr_);
        free(componentEntityIdMemoryPtr_);
    }

    bool ComponentContainer::DeleteComponent(const unsigned int& entityId)
    {
        if (ComponentDestructionFunction_ == nullptr)
        {
            return false;
        }

        std::unordered_map<unsigned int, unsigned int>::iterator iter = entityIdToComponentMemoryIndexMap_.find(entityId);
        if (iter == entityIdToComponentMemoryIndexMap_.end())
        {
            return false;
        }

        int deletedComponentMemoryIndex = iter->second;
        uint8_t* deletedComponentPtr = (uint8_t*)componentMemoryPtr_ + deletedComponentMemoryIndex * componentSize_;

        ComponentDestructionFunction_(deletedComponentPtr);
        entityIdToComponentMemoryIndexMap_.erase(iter);

        unsigned int lastComponentEntityId = *(componentEntityIdMemoryPtr_ + (componentCount_ - 1));

        if (entityId != lastComponentEntityId)
        {
            uint8_t* lastComponentPtr = (uint8_t*)componentMemoryPtr_ + (componentCount_ - 1) * componentSize_;

            ::memcpy(deletedComponentPtr, lastComponentPtr, componentSize_);
            *(componentEntityIdMemoryPtr_ + deletedComponentMemoryIndex) = lastComponentEntityId;

            entityIdToComponentMemoryIndexMap_[lastComponentEntityId] = deletedComponentMemoryIndex;
        }

        componentCount_--;

        return true;
    }

    bool ComponentContainer::DeleteAllComponents()
    {
        if (ComponentDestructionFunction_ == nullptr)
        {
            return false;
        }

        for (size_t i = 0; i < componentCount_; ++i)
        {
            uint8_t* deletedComponent = (uint8_t*)componentMemoryPtr_ + i * componentSize_;
            ComponentDestructionFunction_(deletedComponent);
        }

        componentCount_ = 0;

        entityIdToComponentMemoryIndexMap_.clear();

        return true;
    }
}