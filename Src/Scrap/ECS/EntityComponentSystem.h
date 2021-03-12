#pragma once

#include <map>
//#include <utility>
#include <vector>

#include "glm/glm.hpp"

#include "Engine/EntityComponentSystemTypes.h"



namespace Project001
{
    struct BaseComponent;
    class BaseSystem;
    class SystemList;

    typedef glm::uint32 EntityIndex; // Index into entities_
    
    class EntityComponentSystem
    {
    public:
        EntityComponentSystem();
        ~EntityComponentSystem();

        EntityComponentSystem(EntityComponentSystem& other) = delete;
        void operator=(const EntityComponentSystem&) = delete;

        // Entity Methods
        EntityHandle CreateEntity(ComponentType* componentTypes, BaseComponent** components, size_t size);
        void RemoveEntity(EntityHandle entityHandle);

        // Component Methods
        template<typename T>
        inline void AddComponent(EntityHandle entityHandle, T* component);
        template<typename T>
        T* GetComponent(EntityHandle entityHandle);
        template<typename T>
        bool RemoveComponent(EntityHandle entityHandle);

        // System Methods
        void UpdateSystem(double timestep_s, SystemList systemList);

    private:
        EntityComponentSystem(const EntityComponentSystem& other) {}

        inline std::pair<EntityIndex, std::vector<std::pair<ComponentType, ComponentIndex>>>* GetEntityHandleRawType(EntityHandle entityHandle);
        inline EntityIndex GetEntityIndex(EntityHandle entityHandle);
        inline std::vector<std::pair<ComponentType, ComponentIndex>>& GetEntity(EntityHandle entityHandle);

        void AddComponentInternal(EntityHandle entityHandle, std::vector<std::pair<ComponentType, ComponentIndex>>& entity, ComponentType componentType, BaseComponent* baseComponentPtr);
        void DeleteComponent(ComponentType componentType, ComponentIndex componentIndex);
        BaseComponent* GetComponentInternal(std::vector<std::pair<ComponentType, ComponentIndex>>& entity, ComponentMemory componentMemory, ComponentType componentType);
        bool RemoveComponentInternal(EntityHandle entityHandle, ComponentType componentType);

        void UpdateSystemWithMultipleComponents(ComponentIndex index, double timestep_s, const std::vector<ComponentType>& componentTypes, std::vector<BaseComponent*>& componentPtrs, std::vector<ComponentMemory*>& componentMemoryPtrs, SystemList systemList);
        glm::uint32 FindLeastCommonComponent(const std::vector<ComponentType>& componentTypes, const std::vector<glm::uint32>& componentFlags);

        std::map<ComponentType, ComponentMemory> componentMap_;
        std::vector<std::pair<EntityIndex, std::vector<std::pair<ComponentType, ComponentIndex>>>*> entitieHandlePtrs_;
    };

    template<typename T>
    void EntityComponentSystem::AddComponent(EntityHandle entityHandle, T* component)
    {
        AddComponentInternal(GetEntity(entityHandle), T::type, component);
    }

    template<typename T>
    T* EntityComponentSystem::GetComponent(EntityHandle entityHandle)
    {
        return (T*)GetComponentInternal(GetEntity(entityHandle), componentMap_[T:type], T:type);
    }

    template<typename T>
    bool EntityComponentSystem::RemoveComponent(EntityHandle entityHandle)
    {
        return RemoveComponentInternal(entityHandle, T:type);
    }

    std::pair<EntityIndex, std::vector<std::pair<ComponentType, ComponentIndex>>>* EntityComponentSystem::GetEntityHandleRawType(EntityHandle entityHandle)
    {
        return (std::pair<EntityIndex, std::vector<std::pair<ComponentType, ComponentIndex>>>*)entityHandle;
    }

    EntityIndex EntityComponentSystem::GetEntityIndex(EntityHandle entityHandle)
    {
        return GetEntityHandleRawType(entityHandle)->first;
    }

    std::vector<std::pair<ComponentType, ComponentIndex>>& EntityComponentSystem::GetEntity(EntityHandle entityHandle)
    {
        return GetEntityHandleRawType(entityHandle)->second;
    }
}