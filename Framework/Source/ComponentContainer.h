// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-21

#pragma once

#include <typeinfo>
#include <unordered_map>



namespace Project001
{
    class ComponentContainer
    {
    public:
        ComponentContainer();
        ~ComponentContainer();

        ComponentContainer(ComponentContainer& other) = delete;
        void operator=(const ComponentContainer&) = delete;

        // Initialize the memory used to store components and set its growth
        // behavior. This clears all previously stored components and frees the
        // allocated memory. A component container can be intialized to any
        // component type even after it has already been initialized.
        template <typename Component>
        bool Initialize(
            const size_t& initialComponentCapacity,
            const size_t& componentMemoryGrowthRate,
            const size_t& componentMemoryCapacityCap);

        // Create a component for the given entityId. Memory may resize. when
        // that happens, all previous pointers become invalid.
        template <typename Component, typename... Args>
        bool CreateComponent(const unsigned int& entityId, Args... args);

        // Delete a component for the given entityId. The last component in
        // memory will be moved to the deleted component's location, so
        // those pointers will be invalid.
        bool DeleteComponent(const unsigned int& entityId);

        // Delete all components. All previous pointers to components are
        // invalid.
        bool DeleteAllComponents();

        // Get a pointer to the component associated with the given entityId.
        template <typename Component>
        bool GetComponent(Component*& componentPtr, const unsigned int& entityId);

        // Get a pointer to a contiguous array of all components.
        template <typename Component>
        bool GetAllComponents(Component*& componentPtr, size_t& componetCount);

        // Get the entityId for a given component.
        template <typename Component>
        bool GetComponentEntityId(unsigned int& entityId, const Component* const& componentPtr) const;

        // Get a pointer to a contiguous array of all entityIds.
        template <typename Component>
        bool GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const;

    protected:
        // Here's a function that calls the component's destructor. It's used to
        // delete components with destructors.
        void (*ComponentDestructionFunction_)(uint8_t* componentPtr);

        // This is the typeId of the components stored.
        // It's used to ensure the "ComponentContainer" only
        // contains one type of component at a time.
        size_t componentTypeId_;

        // This is the size of a single component in bytes.
        size_t componentSize_;

        // A pointer to the memory holding the components.
        uint8_t* componentMemoryPtr_;

        // A pointer to the memory holding the components' entity Ids.
        // The indices of the component entity Ids correspond with the order of
        // the components stored in compoenentMemory.
        unsigned int* componentEntityIdMemoryPtr_;

        // Number of components the memory can hold
        size_t componentCapacity_;

        // Number of components currently in memory
        size_t componentCount_;

        // How the component memory resizes when the capacity is exceeded
        // (also applies to component entity id memory)
        // 0 = no resizeing occurs
        // 1 = adds just enough for one more component
        // 2 = component memory doubles in size
        // n = component memory size increase by a multiple of (n)
        size_t componentMemoryGrowthRate_;

        // Component memory is not allowed to excede this. If growth will
        // result in a size larger then this, it is capped.
        // (also applies to component entity id memory)
        // 0 = no size cap
        // n = memory size is capped at (n)
        size_t componentMemoryCapacityCap_;

        // Maps an entity id to it's corresponding component in component
        // memory.
        std::unordered_map<unsigned int, unsigned int> entityIdToComponentMemoryIndexMap_;
    };
    
    // public ------------------------------------------------------------------

    template <typename Component>
    inline bool ComponentContainer::Initialize(
        const size_t& initialComponentCapacity,
        const size_t& componentMemoryGrowthRate,
        const size_t& componentMemoryCapacityCap)
    {
        ComponentDestructionFunction_ = [](uint8_t* componentPtr) { ((Component*)componentPtr)->~Component(); };

        componentTypeId_ = typeid(Component).hash_code();

        componentSize_ = sizeof(Component);

        componentCapacity_ = initialComponentCapacity;

        free(componentMemoryPtr_);
        componentMemoryPtr_ = (uint8_t*)malloc(componentSize_ * componentCapacity_);
        if (componentMemoryPtr_ == nullptr)
        {
            return false;
        }

        free(componentEntityIdMemoryPtr_);
        componentEntityIdMemoryPtr_ = (unsigned int*)malloc(sizeof(unsigned int) * componentCapacity_);
        if (componentEntityIdMemoryPtr_ == nullptr)
        {
            return false;
        }

        componentCount_ = 0;

        componentMemoryGrowthRate_ = componentMemoryGrowthRate;

        componentMemoryCapacityCap_ = componentMemoryCapacityCap;

        return true;
    }

    template <typename Component, typename... Args>
    inline bool ComponentContainer::CreateComponent(const unsigned int& entityId, Args... args)
    {
        size_t componentTypeId = typeid(Component).hash_code();
        if (componentTypeId != componentTypeId_)
        {
            return false;
        }

        if (componentCount_ >= componentCapacity_)
        {
            size_t newComponentCapacity;
            if (componentMemoryGrowthRate_ == 0)
            {
                newComponentCapacity = componentCapacity_;
            }
            else if (componentMemoryGrowthRate_ == 1)
            {
                newComponentCapacity = componentCapacity_ + 1;
            }
            else // (componentMemoryGrowthRate_ > 1)
            {
                newComponentCapacity = componentCapacity_ * componentMemoryGrowthRate_;

                if (newComponentCapacity < componentCapacity_)
                {
                    // size_t overflow
                    return false;
                }

                if (componentMemoryCapacityCap_ != 0 &&
                    newComponentCapacity > componentMemoryCapacityCap_)
                {
                    newComponentCapacity = componentMemoryCapacityCap_;
                }
            }

            if (componentCount_ >= newComponentCapacity)
            {
                return false;
            }

            uint8_t* newComponentMemoryPtr = (uint8_t*)malloc(componentSize_ * newComponentCapacity);
            if (newComponentMemoryPtr == nullptr)
            {
                return false;
            }

            ::memcpy(newComponentMemoryPtr, componentMemoryPtr_, componentSize_ * componentCapacity_);

            free(componentMemoryPtr_);
            componentMemoryPtr_ = newComponentMemoryPtr;

            unsigned int* newComponentEntityIdMemoryPtr = (unsigned int*)malloc(sizeof(unsigned int) * newComponentCapacity);
            if (newComponentEntityIdMemoryPtr == nullptr)
            {
                return false;
            }

            ::memcpy(newComponentEntityIdMemoryPtr, componentEntityIdMemoryPtr_, sizeof(unsigned int) * componentCapacity_);

            free(componentEntityIdMemoryPtr_);
            componentEntityIdMemoryPtr_ = newComponentEntityIdMemoryPtr;

            componentCapacity_ = newComponentCapacity;
        }

        Component* newComponentPtr = new((Component*)componentMemoryPtr_ + componentCount_) Component(args...);
        *(componentEntityIdMemoryPtr_ + componentCount_) = entityId;

        entityIdToComponentMemoryIndexMap_[entityId] = (unsigned int)componentCount_;

        componentCount_++;

        return true;
    }

    template <typename Component>
    inline bool ComponentContainer::GetComponent(Component*& componentPtr, const unsigned int& entityId)
    {
        size_t componentTypeId = typeid(Component).hash_code();
        if (componentTypeId != componentTypeId_ ||
            entityIdToComponentMemoryIndexMap_.find(entityId) == entityIdToComponentMemoryIndexMap_.end())
        {
            return false;
        }

        const unsigned int& componentIndex = entityIdToComponentMemoryIndexMap_[entityId];
        componentPtr = (Component*)componentMemoryPtr_ + componentIndex;

        return true;
    }

    template <typename Component>
    inline bool ComponentContainer::GetAllComponents(Component*& componentPtr, size_t& componetCount)
    {
        size_t componentTypeId = typeid(Component).hash_code();
        if (componentTypeId != componentTypeId_)
        {
            return false;
        }

        componentPtr = (Component*)componentMemoryPtr_;
        componetCount = componentCount_;

        return true;
    }

    template <typename Component>
    inline bool ComponentContainer::GetComponentEntityId(unsigned int& entityId, const Component* const& componentPtr) const
    {
        size_t componentTypeId = typeid(Component).hash_code();
        if (componentTypeId != componentTypeId_)
        {
            return false;
        }

        uint8_t* lastComponentPtr = (uint8_t*)componentMemoryPtr_ + (componentCount_ - 1) * componentSize_;

        if ((uint8_t*)componentPtr < componentMemoryPtr_ || (uint8_t*)componentPtr > lastComponentPtr)
        {
            return false;
        }

        size_t componentOffset = (size_t)componentPtr - (size_t)componentMemoryPtr_;

        // if (componentOffset % componentSize_ != 0)
        // {
        //     return false;
        // }

        size_t componentEntityIdIndex = componentOffset / componentSize_;
        entityId = *(componentEntityIdMemoryPtr_ + componentEntityIdIndex);

        return true;
    }

    template <typename Component>
    inline bool ComponentContainer::GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const
    {
        size_t componentTypeId = typeid(Component).hash_code();
        if (componentTypeId != componentTypeId_)
        {
            return false;
        }

        componentEntityIdPtr = componentEntityIdMemoryPtr_;
        componentCount = componentCount_;

        return true;
    }
}