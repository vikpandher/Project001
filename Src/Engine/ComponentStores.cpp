#include "ComponentStores.h"



namespace Project001
{
    ComponentStores::ComponentStores()
        : nextHighestEntityId_(0)
    {}

    ComponentStores::~ComponentStores()
    {}

    bool ComponentStores::CreateEntity(unsigned int& entityId)
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

    bool ComponentStores::DeleteEntity(unsigned int entityId)
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

    bool ComponentStores::RegisterNewComponent(unsigned int componentTypeId)
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
}