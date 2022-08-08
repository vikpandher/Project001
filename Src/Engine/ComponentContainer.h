#pragma once

#include <typeinfo>
#include <vector>



namespace Project001
{
    class ComponentContainer
    {
    public:
        ComponentContainer();

        ~ComponentContainer();

        template <typename Component, typename... Args>
        bool CreateComponent(const unsigned int entityId, Args... args)
        {
            size_t componentTypeId = typeid(Component).hash_code();
            if (componentMemory_.empty())
            {
                typeId_ = componentTypeId;
                componentSize_ = sizeof(Component);
                ComponentDestructionFunction = [](void* componentPtr) { ((Component*)componentPtr)->~Component(); };
            }
            else if (!TypeIdValid(componentTypeId))
            {
                return false;
            }

            if (ComponentExists(entityId))
            {
                return false;
            }

            size_t componentMemoryIndex = componentMemory_.size();

            componentMemory_.resize(componentMemoryIndex + componentSize_);

            Component* newComponentPtr = new(&componentMemory_[componentMemoryIndex]) Component(args...);
            componentEntityIds_.push_back(entityId);

            if (entityId >= entityIdToComponentMemoryIndicies_.size())
            {
                entityIdToComponentMemoryIndicies_.resize((size_t)entityId + 1, -1);
            }

            entityIdToComponentMemoryIndicies_[entityId] = (int)componentMemoryIndex;

            return true;
        }

        template <typename Component>
        bool GetAllComponents(Component*& componentPtrs, size_t& count)
        {
            size_t componentTypeId = typeid(Component).hash_code();
            if (!TypeIdValid(componentTypeId))
            {
                count = 0;
                return false;
            }

            componentPtrs = (Component*)componentMemory_.data();
            count = componentMemory_.size() / sizeof(Component);

            return true;
        }

        template <typename Component>
        bool GetComponent(const unsigned int entityId, Component*& componentPtr)
        {
            size_t componentTypeId = typeid(Component).hash_code();
            if (!TypeIdValid(componentTypeId) || !ComponentExists(entityId))
            {
                return false;
            }

            componentPtr = (Component*)&componentMemory_[entityIdToComponentMemoryIndicies_[entityId]];

            return true;
        }

        template <typename Component>
        bool GetComponentEntityId(const Component* const componentPtr, unsigned int& entityId)
        {
            size_t componentTypeId = typeid(Component).hash_code();
            if (!TypeIdValid(componentTypeId))
            {
                return false;
            }

            Component* firstComponentPtr = (Component*)componentMemory_.data();
            Component* lastComponentPtr = firstComponentPtr + componentEntityIds_.size() - 1;

            if (componentPtr < firstComponentPtr || componentPtr > lastComponentPtr)
            {
                return false;
            }

            size_t componentOffset = (size_t)componentPtr - (size_t)firstComponentPtr;

            if (componentOffset % componentSize_ != 0)
            {
                return false;
            }

            size_t componentEntityIdIndex = componentOffset / componentSize_;
            entityId = componentEntityIds_[componentEntityIdIndex];

            return true;
        }

        void DeleteAllComponents();

        bool DeleteComponent(const unsigned int entityId);

    protected:

    private:
        inline bool ComponentExists(unsigned int entityId) const
        {
            return entityId < entityIdToComponentMemoryIndicies_.size() && entityIdToComponentMemoryIndicies_[entityId] >= 0;
        }

        inline bool TypeIdValid(size_t testTypeId) const
        {
            return typeId_ == testTypeId;
        }

        // This is the typeId of the components stored.
        // It's used to ensure the "ComponentContainer" only
        // contains one type of component at a time.
        size_t typeId_;

        // This is required when deleting components and not knowing the type.
        size_t componentSize_;

        // This is used to call the destructor when deleting components.
        void (*ComponentDestructionFunction)(void* componentPtr);

        // Components are stored in "componentMemory_".
        std::vector<unsigned char> componentMemory_;

        // The EntityIds of the components in "componentMemory_" are stored in
        // "componentEntityIds_".
        // 
        // The indicies of components in "componentMemory_" are in the same
        // order as the indicies of those components' EntityIds in 
        // "componentEntityIds_".
        std::vector<unsigned int> componentEntityIds_;

        // The indicies of "entityIdToComponentMemoryIndicies_" correspond with
        // entityIds and the values correspond with indicies into 
        // "componentMemory_".
        std::vector<int> entityIdToComponentMemoryIndicies_;
    };
}