#include "ComponentStores.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    ComponentStores::ComponentStores(unsigned int maxNumberOfEntities, unsigned int maxTypesOfComponents)
        : maxNumberOfEntities_(maxNumberOfEntities)
        , maxTypesOfComponents_(maxTypesOfComponents)
    {}

    ComponentStores::~ComponentStores()
    {
        for (size_t i = 0; i < componentContainers_.size(); ++i)
        {
            delete componentContainers_[i];
        }
    }

    bool ComponentStores::CreateEntity(unsigned int& entityId)
    {
        if (recycledEntityIds_.empty())
        {
            unsigned int numberOfEntities = (unsigned int)entityDeletedFlags_.size();

            if (maxNumberOfEntities_ != 0 && numberOfEntities > maxNumberOfEntities_)
            {
                return false;
            }

            entityId = numberOfEntities;

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

        for (unsigned int i = 0; i < componentContainers_.size(); ++i)
        {
            componentContainers_[i]->DeleteComponent(entityId);
        }

        recycledEntityIds_.push(entityId);

        entityDeletedFlags_[entityId] = true;

        return true;
    }

    void ComponentStores::DeleteAllEntities()
    {
        for (unsigned int i = 0; i < componentContainers_.size(); ++i)
        {
            componentContainers_[i]->DeleteAllComponents();
        }

        for (unsigned int i = 0; i < entityDeletedFlags_.size(); ++i)
        {
            const bool& currentEntityAlreadyDeleted = entityDeletedFlags_[i];

            if (!currentEntityAlreadyDeleted)
            {
                recycledEntityIds_.push(i);

                entityDeletedFlags_[i] = true;
            }
        }
    }
}