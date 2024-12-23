// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "ResourceStores.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    ResourceStores::~ResourceStores()
    {
        for (size_t i = 0; i < resourceContainerPtrs_.size(); ++i)
        {
            delete resourceIdGeneratorPtrs_[i];
            delete resourceContainerPtrs_[i];
        }
    }

    void ResourceStores::DeleteAllResources()
    {
        for (size_t i = 0; i < resourceContainerPtrs_.size(); ++i)
        {
            resourceIdGeneratorPtrs_[i]->ReturnAllIds();
            resourceContainerPtrs_[i]->DeleteAllComponents();
        }
    }

    void ResourceStores::DeleteAllResourcesAndResetCapacities()
    {
        for (size_t i = 0; i < resourceContainerPtrs_.size(); ++i)
        {
            delete resourceIdGeneratorPtrs_[i];
            delete resourceContainerPtrs_[i];
        }

        resourceTypeIdToResourceContainerIndexMap_.clear();
        resourceIdGeneratorPtrs_.clear();
        resourceContainerPtrs_.clear();
    }
}