#pragma once

#include "ComponentContainer.h"

#include <queue>
#include <unordered_map>
#include <vector>



namespace Project001
{
    class ComponentStores
    {
    public:
        ComponentStores(unsigned int maxNumberOfEntities = 0, unsigned int maxTypesOfComponents = 0);
        ~ComponentStores();

        // Entity Functions: ---------------------------------------------------

        bool CreateEntity(unsigned int& entityId);

        bool DeleteEntity(unsigned int entityId);

        void DeleteAllEntities();

        // Component Functions: ------------------------------------------------

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
        bool RegisterNewComponent(unsigned int componentTypeId);

        unsigned int maxNumberOfEntities_;
        unsigned int maxTypesOfComponents_;

        std::queue<unsigned int> recycledEntityIds_;

        std::vector<bool> entityDeletedFlags_;

        std::unordered_map<unsigned int, unsigned int> componentTypeIdToComponentContainersIndexMap_;

        std::vector<ComponentContainer*> componentContainers_;

    private:
        ComponentStores(const ComponentStores&);
        ComponentStores& operator=(const ComponentStores&);
    };

    // public ------------------------------------------------------------------

    template <typename Component, typename... Args>
    inline bool ComponentStores::CreateComponent(unsigned int entityId, Args... args)
    {
        if (!EntityExists(entityId))
        {
            return false;
        }

        unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
        if (!ComponentTypeExists(componentTypeId) && !RegisterNewComponent<Component>(componentTypeId))
        {
            return false;
        }

        unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
        return componentContainers_[componentContainerIndex]->CreateComponent<Component>(entityId, args...);
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
        return componentContainers_[componentContainerIndex]->DeleteComponent(entityId);
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
        componentContainers_[componentContainerIndex]->DeleteAllComponents();

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
        return componentContainers_[componentContainerIndex]->GetComponent<Component>(entityId, componentPtr);
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
        return componentContainers_[componentContainerIndex]->GetAllComponents<Component>(componentPtrs, componentCount);
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
        return componentContainers_[componentContainerIndex]->GetComponentEntityId<Component>(componentPtr, entityId);
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
        return componentContainers_[componentContainerIndex]->GetAllComponentEntityIds<Component>(componentEntityIdPtr, componentCount);
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
    inline bool ComponentStores::RegisterNewComponent(unsigned int componentTypeId)
    {
        unsigned int nextComponentContainerIndex = (unsigned int)componentContainers_.size();

        if (maxTypesOfComponents_ != 0 && nextComponentContainerIndex > maxTypesOfComponents_)
        {
            return false;
        }

        componentTypeIdToComponentContainersIndexMap_[componentTypeId] = nextComponentContainerIndex;

        ComponentContainer* newComponentContainerPtr = new ComponentContainer();
        newComponentContainerPtr->Initialize<Component>(1, 2, 0, 1, 2, 0);
        componentContainers_.push_back(newComponentContainerPtr);

        return true;
    }
}