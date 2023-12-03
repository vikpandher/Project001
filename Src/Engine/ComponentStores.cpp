#include "ComponentStores.h"

#include <algorithm>



namespace Project001
{
    // public ------------------------------------------------------------------

    ComponentStores::ComponentStores()
        : queueSorted_(true)
    {}

    ComponentStores::~ComponentStores()
    {
        for (size_t i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            delete componentContainerPtrs_[i];
        }
    }

    bool ComponentStores::CreateEntity(unsigned int& entityId)
    {
        if (recycledIds_.empty())
        {
            entityId = (unsigned int)entityDeletedFlags_.size();
            entityDeletedFlags_.push_back(false);
        }
        else
        {
            if (!queueSorted_)
            {
                std::sort(recycledIds_.begin(), recycledIds_.end());
                queueSorted_ = true;
            }
            entityId = recycledIds_.front();
            recycledIds_.pop_front();

            entityDeletedFlags_[entityId] = false;
        }

        return true;
    }

    bool ComponentStores::DeleteEntity(unsigned int entityId)
    {
        if (!EntityExists(entityId))
        {
            return false;
        }

        for (unsigned int i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            componentContainerPtrs_[i]->DeleteComponent(entityId);
        }

        queueSorted_ = false;
        recycledIds_.push_back(entityId);

        entityDeletedFlags_[entityId] = true;

        return true;
    }

    void ComponentStores::DeleteAllEntities()
    {
        for (unsigned int i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            // componentContainerPtrs_[i]->DeleteAllComponents();
            delete componentContainerPtrs_[i];
        }
        componentContainerPtrs_.clear();
        componentTypeIdToComponentContainersIndexMap_.clear();

        queueSorted_ = true;
        recycledIds_.clear();
        entityDeletedFlags_.clear();
    }
}