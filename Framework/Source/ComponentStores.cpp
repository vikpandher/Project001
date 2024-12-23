// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "ComponentStores.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    ComponentStores::ComponentStores()
    {}

    ComponentStores::~ComponentStores()
    {
        for (size_t i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            delete componentContainerPtrs_[i];
        }
    }

    void ComponentStores::CreateEntity(unsigned int& entityId)
    {
        entityIdGenerator_.GetNewId(entityId);
    }

    bool ComponentStores::DeleteEntity(unsigned int entityId)
    {
        if (!entityIdGenerator_.ReturnId(entityId))
        {
            return false;
        }

        for (unsigned int i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            componentContainerPtrs_[i]->DeleteComponent(entityId);
        }

        return true;
    }

    void ComponentStores::DeleteAllEntities()
    {
        entityIdGenerator_.ReturnAllIds();

        for (unsigned int i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            componentContainerPtrs_[i]->DeleteAllComponents();
        }
    }

    void ComponentStores::DeleteAllEntitiesAndResetCapacities()
    {
        entityIdGenerator_.ReturnAllIds();

        for (unsigned int i = 0; i < componentContainerPtrs_.size(); ++i)
        {
            delete componentContainerPtrs_[i];
        }
        componentContainerPtrs_.clear();
        componentTypeIdToComponentContainerIndexMap_.clear();
    }
}