#pragma once

#include "ComponentContainer.h"
#include "UniqueIdGenerator.h"



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

        void CreateEntity(unsigned int& entityId);

        bool DeleteEntity(unsigned int entityId);

        void DeleteAllEntities();

        void DeleteAllEntitiesAndResetCapacities();

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
        bool GetComponent(Component*& componentPtr, unsigned int entityId);

        template <typename Component>
        bool GetAllComponents(Component*& componentPtrs, size_t& componentCount);

        template <typename Component>
        bool GetComponentEntityId(unsigned int& entityId, const Component* const componentPtr);

        template <typename Component>
        bool GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const;

    protected:
        bool ComponentTypeExists(unsigned int componentTypeId) const;

        template <typename Component>
        void RegisterNewComponent(size_t initialComponentCapacity, size_t componentMemoryGrowthRate, size_t componentMemoryCapacityCap);

        UniqueIdGenerator entityIdGenerator_;

        std::unordered_map<unsigned int, unsigned int> componentTypeIdToComponentContainerIndexMap_;

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

        RegisterNewComponent<Component>(initialComponentCapacity, componentMemoryGrowthRate, componentMemoryCapacityCap);
        return true;
    }

    template <typename Component, typename... Args>
    inline bool ComponentStores::CreateComponent(unsigned int entityId, Args... args)
    {
        if (!entityIdGenerator_.IdIsTaken(entityId))
        {
            return false; // Entity doesn't exist
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            RegisterNewComponent<Component>(1, 2, 0);
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->CreateComponent<Component>(entityId, args...);
    }

    template <typename Component>
    inline bool ComponentStores::DeleteComponent(unsigned int entityId)
    {
        if (!entityIdGenerator_.IdIsTaken(entityId))
        {
            return false; // Entity doesn't exist
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_[componentTypeId];
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

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_[componentTypeId];
        componentContainerPtrs_[componentContainerIndex]->DeleteAllComponents();

        return true;
    }

    template <typename Component>
    inline bool ComponentStores::GetComponent(Component*& componentPtr, unsigned int entityId)
    {
        if (!entityIdGenerator_.IdIsTaken(entityId))
        {
            return false; // Entity doesn't exist
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->GetComponent<Component>(componentPtr, entityId);
    }

    template <typename Component>
    inline bool ComponentStores::GetAllComponents(Component*& componentPtrs, size_t& componentCount)
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->GetAllComponents<Component>(componentPtrs, componentCount);
    }

    template <typename Component>
    inline bool ComponentStores::GetComponentEntityId(unsigned int& entityId, const Component* const componentPtr)
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_[componentTypeId];
        return componentContainerPtrs_[componentContainerIndex]->GetComponentEntityId<Component>(entityId, componentPtr);
    }

    template <typename Component>
    inline bool ComponentStores::GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const
    {
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainerIndexMap_.at(componentTypeId);
        return componentContainerPtrs_[componentContainerIndex]->GetAllComponentEntityIds<Component>(componentEntityIdPtr, componentCount);
    }

    // protected ---------------------------------------------------------------

    inline bool ComponentStores::ComponentTypeExists(unsigned int componentTypeId) const
    {
        return componentTypeIdToComponentContainerIndexMap_.find(componentTypeId) != componentTypeIdToComponentContainerIndexMap_.end();
    }

    template <typename Component>
    inline void ComponentStores::RegisterNewComponent(size_t initialComponentCapacity, size_t componentMemoryGrowthRate, size_t componentMemoryCapacityCap)
    {
        unsigned int nextComponentContainerIndex = (unsigned int)componentContainerPtrs_.size();
        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        componentTypeIdToComponentContainerIndexMap_[componentTypeId] = nextComponentContainerIndex;

        ComponentContainer* newComponentContainerPtr = new ComponentContainer();
        newComponentContainerPtr->Initialize<Component>(initialComponentCapacity, componentMemoryGrowthRate, componentMemoryCapacityCap);
        componentContainerPtrs_.push_back(newComponentContainerPtr);
    }
}