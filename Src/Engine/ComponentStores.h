#pragma once

#include "ComponentContainer.h"

#include <deque>
#include <vector>



namespace Project001
{
    class ComponentStores
    {
    public:
        ComponentStores();
        ~ComponentStores();

        ComponentStores(ComponentStores& other) = delete;
        void operator=(const ComponentStores&) = delete;

        // Entity Functions: ---------------------------------------------------

        bool CreateEntity(unsigned int& entityId);

        bool DeleteEntity(unsigned int entityId);

        void DeleteAllEntities();

        // Component Functions: ------------------------------------------------

        template <typename Component>
        bool InitializeComponentContainer(size_t initialComponentCapacity, size_t componentMemoryGrowthRate, size_t componentMemoryCapacityCap);

        template <typename Component, typename... Args>
        bool CreateComponent(unsigned int entityId, Args... args);

        template <typename Component>
        bool DeleteComponent(unsigned int entityId);

        template <typename Component>
        bool DeleteAllComponents();

        template <typename Component>
        bool GetComponent(unsigned int entityId, Component*& componentPtr);

        template <typename Component>
        bool GetAllComponents(Component*& componentPtrs, size_t& componentCount);

        template <typename Component>
        bool GetComponentEntityId(const Component* const componentPtr, unsigned int& entityId);

        template <typename Component>
        bool GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const;

    protected:
        bool EntityExists(unsigned int entityId) const;

        bool ComponentTypeExists(unsigned int componentTypeId) const;

        template <typename Component>
        bool RegisterNewComponent(size_t initialComponentCapacity, size_t componentMemoryGrowthRate, size_t componentMemoryCapacityCap);

        bool queueSorted_;
        std::deque<unsigned int> recycledIds_;
        std::vector<bool> entityDeletedFlags_;

        std::unordered_map<unsigned int, unsigned int> componentTypeIdToComponentContainersIndexMap_;

        std::vector<ComponentContainer*> componentContainerPtrs_;
    };

    // public ------------------------------------------------------------------

    template <typename Component>
    inline bool ComponentStores::InitializeComponentContainer(size_t initialComponentCapacity, size_t componentMemoryGrowthRate, size_t componentMemoryCapacityCap)
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        return RegisterNewComponent<Component>(initialComponentCapacity, componentMemoryGrowthRate, componentMemoryCapacityCap);
    }

    template <typename Component, typename... Args>
    inline bool ComponentStores::CreateComponent(unsigned int entityId, Args... args)
    {
        if (!EntityExists(entityId))
        {
            return false;
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId) && !RegisterNewComponent<Component>(1, 2, 0))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->CreateComponent<Component>(entityId, args...);
    }

    template <typename Component>
    inline bool ComponentStores::DeleteComponent(unsigned int entityId)
    {
        if (!EntityExists(entityId))
        {
            return false;
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->DeleteComponent(entityId);
    }

    template <typename Component>
    inline bool ComponentStores::DeleteAllComponents()
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        componentContainerPtrs_[componentContainerIndex]->DeleteAllComponents();

        return true;
    }

    template <typename Component>
    inline bool ComponentStores::GetComponent(unsigned int entityId, Component*& componentPtr)
    {
        if (!EntityExists(entityId))
        {
            return false;
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->GetComponent<Component>(entityId, componentPtr);
    }

    template <typename Component>
    inline bool ComponentStores::GetAllComponents(Component*& componentPtrs, size_t& componentCount)
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->GetAllComponents<Component>(componentPtrs, componentCount);
    }

    template <typename Component>
    inline bool ComponentStores::GetComponentEntityId(const Component* const componentPtr, unsigned int& entityId)
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->GetComponentEntityId<Component>(componentPtr, entityId);
    }

    template <typename Component>
    inline bool ComponentStores::GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_.at(componentTypeId);
        return componentContainerPtrs_[componentContainerIndex]->GetAllComponentEntityIds<Component>(componentEntityIdPtr, componentCount);
    }

    // protected ---------------------------------------------------------------

    inline bool ComponentStores::EntityExists(unsigned int entityId) const
    {
        return entityId < entityDeletedFlags_.size() && !entityDeletedFlags_[entityId];
    }

    inline bool ComponentStores::ComponentTypeExists(unsigned int componentTypeId) const
    {
        return componentTypeIdToComponentContainersIndexMap_.find(componentTypeId) != componentTypeIdToComponentContainersIndexMap_.end();
    }

    template <typename Component>
    inline bool ComponentStores::RegisterNewComponent(size_t initialComponentCapacity, size_t componentMemoryGrowthRate, size_t componentMemoryCapacityCap)
    {
        unsigned int nextComponentContainerIndex = (unsigned int)componentContainerPtrs_.size();
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        componentTypeIdToComponentContainersIndexMap_[componentTypeId] = nextComponentContainerIndex;

        ComponentContainer* newComponentContainerPtr = new ComponentContainer();
        newComponentContainerPtr->Initialize<Component>(initialComponentCapacity, componentMemoryGrowthRate, componentMemoryCapacityCap);
        componentContainerPtrs_.push_back(newComponentContainerPtr);

        return true;
    }
}