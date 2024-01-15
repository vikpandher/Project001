#pragma once

#include <queue>
#include <map>

#include "ComponentContainer.h"



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

        void DeleteAllEntities();

        bool DeleteEntity(unsigned int entityId);

        // Component Functions: ------------------------------------------------

        // Note that creating a Component voids all previously aquired Component
        // ptrs.
        template <typename Component, typename... Args>
        bool CreateComponent(unsigned int entityId, Args... args)
        {
            if (!EntityExists(entityId))
            {
                return false;
            }

            unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
            if (!ComponentTypeExists(componentTypeId) && !RegisterNewComponent(componentTypeId))
            {
                return false;
            }

            unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
            return componentContainers_[componentContainerIndex].CreateComponent<Component>(entityId, args...);
        }

        template <typename Component>
        bool GetAllComponents(Component*& compoonentPtrs, size_t& count)
        {
            unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
            if (!ComponentTypeExists(componentTypeId))
            {
                count = 0;
                return false;
            }

            unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
            return componentContainers_[componentContainerIndex].GetAllComponents<Component>(compoonentPtrs, count);
        }

        template <typename Component>
        bool GetComponent(unsigned int entityId, Component*& componentPtr)
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
            return componentContainers_[componentContainerIndex].GetComponent<Component>(entityId, componentPtr);
        }

        template <typename Component>
        bool GetComponentEntityId(const Component* const componentPtr, unsigned int& entityId)
        {
            unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
            if (!ComponentTypeExists(componentTypeId))
            {
                return false;
            }

            unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
            return componentContainers_[componentContainerIndex].GetComponentEntityId<Component>(componentPtr, entityId);
        }

        template <typename Component>
        bool DeleteAllComponents()
        {
            unsigned int componentTypeId = (unsigned int)typeid(Component).hash_code();
            if (!ComponentTypeExists(componentTypeId))
            {
                return false;
            }

            unsigned int componentContainerIndex = componentTypeIdToComponentContainersIndexMap_[componentTypeId];
            componentContainers_[componentContainerIndex].DeleteAllComponents();

            return true;
        }

        template <typename Component>
        bool DeleteComponent(unsigned int entityId)
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
            return componentContainers_[componentContainerIndex].DeleteComponent(entityId);
        }

    protected:

    private:
        inline bool EntityExists(unsigned int entityId) const
        {
            return entityId < entityDeletedFlags_.size() && !entityDeletedFlags_[entityId];
        }

        inline bool ComponentTypeExists(unsigned int componentTypeId) const
        {
            return componentTypeIdToComponentContainersIndexMap_.find(componentTypeId) != componentTypeIdToComponentContainersIndexMap_.end();
        }

        bool RegisterNewComponent(unsigned int componentTypeId);

        static const unsigned int s_maxNumberOfEntities_ = 128;
        static const unsigned int s_maxTypesOfComponents_ = 32;

        std::queue<unsigned int> recycledEntityIds_;

        std::vector<bool> entityDeletedFlags_;

        std::map<unsigned int, unsigned int> componentTypeIdToComponentContainersIndexMap_;

        std::vector<ComponentContainer> componentContainers_;
    };
}