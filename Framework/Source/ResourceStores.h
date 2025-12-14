// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#pragma once

#include "ComponentContainer.h"
#include "UniqueIdGenerator.h"



namespace Project001
{
    class ResourceStores
    {
    public:
        ~ResourceStores();

        template <typename Resource>
        bool Initialize(
            const size_t& initialResourceCapacity,
            const size_t& resourceMemoryGrowthRate,
            const size_t& resourceMemoryCapacityCap);

        template <typename Resource, typename... Args>
        bool CreateResource(unsigned int& resourceId, Args... args);

        template <typename Resource>
        bool DeleteResource(unsigned int resourceId);

        template <typename Resource>
        bool DeleteAllResources();

        void DeleteAllResources();

        void DeleteAllResourcesAndResetCapacities();

        template <typename Resource>
        bool GetResource(Resource*& resourcePtr, unsigned int resourceId);

        template <typename Resource>
        bool GetAllResources(Resource*& resourcePtrs, size_t& resourceCount);

        template <typename Resource>
        bool GetResourceId(unsigned int& resourceId, const Resource* const resourcePtr);

        template <typename Resource>
        bool GetAllResourceIds(const unsigned int*& resourceIdPtr, size_t& resourceCount) const;

    protected:
        bool ResourceTypeExists(unsigned int resourceTypeId) const;

        template <typename Resource>
        void RegisterNewResource(size_t initialResourceCapacity, size_t resourceMemoryGrowthRate, size_t resourceMemoryCapacityCap);

        std::unordered_map<unsigned int, unsigned int> resourceTypeIdToResourceContainerIndexMap_;

        std::vector<UniqueIdGenerator*> resourceIdGeneratorPtrs_;
        std::vector<ComponentContainer*> resourceContainerPtrs_;
    };

    // public ------------------------------------------------------------------

    template <typename Resource>
    inline bool ResourceStores::Initialize(
        const size_t& initialResourceCapacity,
        const size_t& resourceMemoryGrowthRate,
        const size_t& resourceMemoryCapacityCap)
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            RegisterNewResource<Resource>(1, 2, 0);
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        return resourceContainerPtrs_[resourceContainerIndex]->Initialize<Resource>(initialResourceCapacity, resourceMemoryGrowthRate, resourceMemoryCapacityCap);
    }

    template <typename Resource, typename... Args>
    inline bool ResourceStores::CreateResource(unsigned int& resourceId, Args... args)
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            RegisterNewResource<Resource>(1, 2, 0);
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        unsigned int newResourceId;
        resourceIdGeneratorPtrs_[resourceContainerIndex]->GetNewId(newResourceId);
        if (!resourceContainerPtrs_[resourceContainerIndex]->CreateComponent<Resource>(newResourceId, args...))
        {
            resourceIdGeneratorPtrs_[resourceContainerIndex]->ReturnId(newResourceId);
            return false;
        }
        resourceId = newResourceId;
        return true;
    }

    template <typename Resource>
    inline bool ResourceStores::DeleteResource(unsigned int resourceId)
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            return false;
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        return resourceIdGeneratorPtrs_[resourceContainerIndex]->ReturnId(resourceId) && 
            resourceContainerPtrs_[resourceContainerIndex]->DeleteComponent(resourceId);
    }

    template <typename Resource>
    inline bool ResourceStores::DeleteAllResources()
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            return false;
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        resourceIdGeneratorPtrs_[resourceContainerIndex]->ReturnAllIds();
        return resourceContainerPtrs_[resourceContainerIndex]->DeleteAllComponents();
    }

    template <typename Resource>
    inline bool ResourceStores::GetResource(Resource*& resourcePtr, unsigned int resourceId)
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            return false;
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        return resourceContainerPtrs_[resourceContainerIndex]->GetComponent(resourcePtr, resourceId);
    }

    template <typename Resource>
    inline bool ResourceStores::GetAllResources(Resource*& resourcePtrs, size_t& resourceCount)
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            return false;
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        return resourceContainerPtrs_[resourceContainerIndex]->GetAllComponents(resourcePtrs, resourceCount);
    }

    template <typename Resource>
    inline bool ResourceStores::GetResourceId(unsigned int& resourceId, const Resource* const resourcePtr)
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            return false;
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_[resourceTypeId];
        return resourceContainerPtrs_[resourceContainerIndex]->GetComponentEntityId(resourceId, resourcePtr);
    }

    template <typename Resource>
    inline bool ResourceStores::GetAllResourceIds(const unsigned int*& resourceIdPtr, size_t& resourceCount) const
    {
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        if (!ResourceTypeExists(resourceTypeId))
        {
            return false;
        }

        unsigned int resourceContainerIndex = resourceTypeIdToResourceContainerIndexMap_.at(resourceTypeId);
        return resourceContainerPtrs_[resourceContainerIndex]->GetAllComponentEntityIds<Resource>(resourceIdPtr, resourceCount);
    }

    // protected ---------------------------------------------------------------

    inline bool ResourceStores::ResourceTypeExists(unsigned int resourceTypeId) const
    {
        return resourceTypeIdToResourceContainerIndexMap_.find(resourceTypeId) != resourceTypeIdToResourceContainerIndexMap_.end();
    }

    template <typename Resource>
    inline void ResourceStores::RegisterNewResource(size_t initialResourceCapacity, size_t resourceMemoryGrowthRate, size_t resourceMemoryCapacityCap)
    {
        unsigned int nextResourceContainerIndex = static_cast<unsigned int>(resourceContainerPtrs_.size());
        unsigned int resourceTypeId = static_cast<unsigned int>(typeid(Resource).hash_code());
        resourceTypeIdToResourceContainerIndexMap_[resourceTypeId] = nextResourceContainerIndex;

        UniqueIdGenerator* newUniqueIdGenerator = new UniqueIdGenerator();
        resourceIdGeneratorPtrs_.push_back(newUniqueIdGenerator);

        ComponentContainer* newComponentContainerPtr = new ComponentContainer();
        newComponentContainerPtr->Initialize<Resource>(initialResourceCapacity, resourceMemoryGrowthRate, resourceMemoryCapacityCap);
        resourceContainerPtrs_.push_back(newComponentContainerPtr);
    }
}