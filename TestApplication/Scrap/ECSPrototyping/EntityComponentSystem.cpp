#include "EntityComponentSystem.h"

#include "Engine/Component.h"
#include "Engine/Logger.h"
#include "Engine/System.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    EntityComponentSystem::EntityComponentSystem()
    {

    }

    EntityComponentSystem::~EntityComponentSystem()
    {
        for (std::map<ComponentType, ComponentMemory>::iterator itr = componentMap_.begin(); itr != componentMap_.end(); ++itr)
        {
            ComponentType componentType = (*itr).first;
            ComponentMemory& componentMemory = (*itr).second;
            
            size_t componentSize = BaseComponent::GetTypeSize(componentType);
            ComponentFreeFunction componentFreeFunction = BaseComponent::GetTypeFreeFunction(componentType);
            for (size_t i = 0; i < componentMemory.size(); i += componentSize)
            {
                componentFreeFunction((BaseComponent*)componentMemory[i]);
            }
        }
        
        for (unsigned int i = 0; i < entitieHandlePtrs_.size(); ++i)
        {
            delete entitieHandlePtrs_[i];
        }
    }

    EntityHandle EntityComponentSystem::CreateEntity(ComponentType* componentTypes, BaseComponent** components, size_t size)
    {
        std::pair<EntityIndex, std::vector<std::pair<ComponentType, ComponentIndex>>>* newEntityPtr = new std::pair<EntityIndex, std::vector<std::pair<ComponentType, ComponentIndex>>>();
        EntityHandle entityHandle = (EntityHandle)newEntityPtr;

        for (unsigned int i = 0; i < size; ++i)
        {
            ComponentType& componentType = componentTypes[i];
            if (!BaseComponent::IsTypeValid(componentType))
            {
                Logger::Error("Invalid ComponentType");
                delete newEntityPtr;
                return nullptr;
            }
            else
            {
                AddComponentInternal(entityHandle, newEntityPtr->second, componentType, components[i]);
            }
        }
        
        newEntityPtr->first = (EntityIndex)entitieHandlePtrs_.size();
        entitieHandlePtrs_.push_back(newEntityPtr);

        return entityHandle;
    }

    void EntityComponentSystem::RemoveEntity(EntityHandle entityHandle)
    {
        std::vector<std::pair<ComponentType, ComponentIndex>>& entity = GetEntity(entityHandle);
        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            DeleteComponent(entity[i].first, entity[i].second);
        }

        EntityIndex destinationIndex = GetEntityIndex(entityHandle);
        EntityIndex sourceIndex = (EntityIndex)(entitieHandlePtrs_.size() - 1);
        delete entitieHandlePtrs_[destinationIndex];
        entitieHandlePtrs_[destinationIndex] = entitieHandlePtrs_[sourceIndex];
        entitieHandlePtrs_[destinationIndex]->first = destinationIndex;
        entitieHandlePtrs_.pop_back();
    }

    void EntityComponentSystem::UpdateSystem(double timestep_s, SystemList systemList)
    {
        std::vector<BaseComponent*> componentPtrs;
        std::vector<ComponentMemory*> componentMemoryPtrs;
        for (unsigned int i = 0; i < systemList.Size(); ++i)
        {
            const std::vector<ComponentType>& componentTypes = systemList[i]->GetComponentTypes();
            if (componentTypes.size() == 1)
            {
                size_t componentSize = BaseComponent::GetTypeSize(componentTypes[0]);
                ComponentMemory& componentMemory = componentMap_[componentTypes[0]];
                for (size_t j = 0; j < componentMemory.size(); j += componentSize)
                {
                    BaseComponent* currentComponentPtr = (BaseComponent*)&componentMemory[j];
                    systemList[i]->UpdateComponents(timestep_s, &currentComponentPtr);
                }
            }
            else
            {
                UpdateSystemWithMultipleComponents(i, timestep_s, componentTypes, componentPtrs, componentMemoryPtrs, systemList);
            }
        }
    }

    // private: ----------------------------------------------------------------

    void EntityComponentSystem::AddComponentInternal(EntityHandle entityHandle, std::vector<std::pair<ComponentType, ComponentIndex>>& entity, ComponentType componentType, BaseComponent* baseComponentPtr)
    {
        ComponentCreateFunction componentCreateFunction = BaseComponent::GetTypeCreateFunction(componentType);
        std::pair<ComponentType, ComponentIndex> newComponentInfo;
        newComponentInfo.first = componentType;
        newComponentInfo.second = componentCreateFunction(componentMap_[componentType], entityHandle, baseComponentPtr);
        entity.push_back(newComponentInfo);
    }

    void EntityComponentSystem::DeleteComponent(ComponentType componentType, ComponentIndex componentIndex)
    {
        ComponentMemory componentMemory = componentMap_[componentType];
        ComponentFreeFunction componentFreeFunction = BaseComponent::GetTypeFreeFunction(componentType);
        size_t typeSize = BaseComponent::GetTypeSize(componentType);

        ComponentIndex sourceIndex = (ComponentIndex)(componentMemory.size() - typeSize);

        BaseComponent* sourceComponentPtr = (BaseComponent*)&componentMemory[sourceIndex];
        BaseComponent* destinationComponentPtr = (BaseComponent*)&componentMemory[componentIndex];
        componentFreeFunction(destinationComponentPtr);

        if (componentIndex == sourceIndex)
        {
            componentMemory.resize(sourceIndex);
            return;
        }

        ::memcpy(destinationComponentPtr, sourceComponentPtr, typeSize);

        std::vector<std::pair<ComponentType, ComponentIndex>>& sourceEntity = GetEntity(sourceComponentPtr->entityHandle);
        for (unsigned int i = 0; i < sourceEntity.size(); ++i)
        {
            // Every component is of a unique type
            if (componentType == sourceEntity[i].first /*&& sourceIndex == sourceEntity[i].second*/)
            {
                sourceEntity[i].second = componentIndex;
                break;
            }
        }

        componentMemory.resize(sourceIndex);
    }

    BaseComponent* EntityComponentSystem::GetComponentInternal(std::vector<std::pair<ComponentType, ComponentIndex>>& entity, ComponentMemory componentMemory, ComponentType componentType)
    {
        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            if (componentType == entity[i].first)
            {
                // Just because we're accessing things in arrays, doesn't mean
                // the access is contiguous right here. The array is pre-cached,
                // but if the index of the component is scattered randomly in
                // the array, it's possible we're jumping to a point in the
                // array that is not in the cache.
                return (BaseComponent*)&componentMemory[entity[i].second];
            }
        }
        return nullptr;
    }

    bool EntityComponentSystem::RemoveComponentInternal(EntityHandle entityHandle, ComponentType componentType)
    {
        std::vector<std::pair<ComponentType, ComponentIndex>>& entity = GetEntity(entityHandle);
        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            // Every component is of a unique type
            if (componentType == entity[i].first /*&& sourceIndex == sourceEntity[i].second*/)
            {
                DeleteComponent(entity[i].first, entity[i].second);
                EntityIndex sourceIndex = (EntityIndex)(entity.size() - 1);
                EntityIndex destinationIndex = i;
                entity[destinationIndex] = entity[sourceIndex];
                entity.pop_back();
                return true;
            }
        }
        return false;
    }

    void EntityComponentSystem::UpdateSystemWithMultipleComponents(ComponentIndex index, double timestep_s, const std::vector<ComponentType>& componentTypes, std::vector<BaseComponent*>& componentPtrs, std::vector<ComponentMemory*>& componentMemoryPtrs, SystemList systemList)
    {
        const std::vector<glm::uint32>& componentFlags = systemList[index]->GetComponentFlags();
        
        size_t componentTypesSize = componentTypes.size();
        size_t componentPtrsSize = componentPtrs.size();
        componentPtrsSize = componentTypesSize > componentPtrsSize ? componentTypesSize : componentPtrsSize;
        componentPtrs.resize(componentPtrsSize);
        size_t componentMemoryPtrsSize = componentMemoryPtrs.size();
        componentMemoryPtrsSize = componentTypesSize > componentMemoryPtrsSize ? componentTypesSize : componentMemoryPtrsSize;
        componentMemoryPtrs.resize(componentMemoryPtrsSize);

        for (unsigned int i = 0; i < componentTypesSize; ++i)
        {
            componentMemoryPtrs[i] = &componentMap_[componentTypes[i]];
        }

        glm::uint32 minSizeIndex = FindLeastCommonComponent(componentTypes, componentFlags);

        size_t typeSize = BaseComponent::GetTypeSize(componentTypes[0]);
        ComponentMemory& componentMemory = *componentMemoryPtrs[minSizeIndex];
        for (size_t i = 0; i < componentMemory.size(); i += typeSize)
        {
            componentPtrs[minSizeIndex] = (BaseComponent*)&componentMemory[i];

            // CACHE IMPROVEMENTS STILL NEED TO BE MADE:
            // Where we have out entity-components array, we're doing a double
            // de-reference. And, we're doing that every single iteration
            // through the outer loop. (Look at GetEntity)
            std::vector<std::pair<ComponentType, ComponentIndex>>& entity = GetEntity(componentPtrs[minSizeIndex]->entityHandle);

            bool isValid = true;
            for (unsigned int j = 0; j < componentTypes.size(); ++j)
            {
                if (j == minSizeIndex)
                {
                    continue;
                }

                componentPtrs[j] = GetComponentInternal(entity, *componentMemoryPtrs[j], componentTypes[j]);
                if (componentPtrs[j] == nullptr && (componentFlags[j] & (glm::uint32)BaseSystem::ComponentFlag::FLAG_OPTIONAL) == 0)
                {
                    isValid = false;
                    break;
                }
            }

            if (isValid)
            {
                systemList[index]->UpdateComponents(timestep_s, &componentPtrs[0]);
            }
        }
    }

    glm::uint32 EntityComponentSystem::FindLeastCommonComponent(const std::vector<ComponentType>& componentTypes, const std::vector<glm::uint32>& componentFlags)
    {
        size_t minSize = (glm::uint32)-1;
        glm::uint32 minIndex = (glm::uint32)-1;

        for (unsigned int i = 0; i < componentTypes.size(); ++i)
        {
            if ((componentFlags[i] & (glm::uint32)BaseSystem::ComponentFlag::FLAG_OPTIONAL) != 0)
            {
                continue;
            }
            
            size_t typeSize = BaseComponent::GetTypeSize(componentTypes[i]);
            size_t size = componentMap_[componentTypes[i]].size() / typeSize;
            if (size <= minSize)
            {
                minSize = size;
                minIndex = i;
            }
        }

        return minIndex;
    }
}