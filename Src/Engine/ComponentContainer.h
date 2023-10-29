#pragma once

#include <cstring>
#include <typeinfo>



namespace Project001
{
    class ComponentContainer
    {
    public:
        ComponentContainer();
        ~ComponentContainer();

        // Initialize the memory used to store components and set its growth
        // behavior. This clears all previously stored components. A component
        // container can be intialized to any component type even after it has
        // already been initialized.
        template <typename Component>
        bool Initialize(
            const size_t& initialComponentCapacity,
            const size_t& componentMemoryGrowthRate,
            const size_t& componentMemoryCapacityCap,
            const size_t& initialEntityIdCapacity,
            const size_t& entityIdToComponentIndiciesMemoryGrowthRate,
            const size_t& entityIdToComponentIndiciesMemoryCapacityCap);

        // Create a component for the given entityId. Memory may resize so
        // all previous pointers and if that happens all previously aquired
        // pointers to components are invalid.
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
        bool GetComponent(const unsigned int& entityId, Component*& componentPtr);

        // Get a pointer to a contiguous array of all components.
        template <typename Component>
        bool GetAllComponents(Component*& componentPtr, size_t& componetCount);

        // Get the entityId for a given component.
        template <typename Component>
        bool GetComponentEntityId(const Component* const& componentPtr, unsigned int& entityId) const;

        // Get a pointer to a contiguous array of all entityIds that have the given component.
        template <typename Component>
        bool GetAllComponentEntityIds(const unsigned int*& componentEntityIdPtr, size_t& componentCount) const;

    protected:
        // Here's a function that calls the component's destructor. Needed to
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
        // The index of the compoent entity Ids correspond with the order the
        // components are stored in compoenentMemory.
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
        size_t componentMemoryCapacityCap_;

        // A pointer to memory that holds the indicies of components, in order
        // according to their entity id, with the entity id being the index in
        // entityIdToComponentIndiciesMemoryPtr_ that holds the index for the
        // entity id's component.
        // ex: entityIdToComponentIndiciesMemoryPtr_[0] will hold the index
        // of (entity id == 0)'s component in component memory. 
        // entityIdToComponentIndiciesMemoryPtr_[n] will hold the index of
        // (entity id == n)'s component in component memory.
        // This will have gaps for when an entity id doesn't have a component.
        // The gaps will have a index value of -1, indicating that there are
        // no components for that entity id.
        // 
        // NOTE: Maybe I'll replace this with a unordered_map in the future.
        int* entityIdToComponentIndiciesMemoryPtr_;

        // Number of entity ids the entityId-to-component-indicies memory can
        // hold. It grows as the entity id value grows, not the number of entity
        // ids.
        size_t entityIdCapacity_;

        // How the entityId-to-component-indicies memory resizes when the
        // capacity is exceeded
        // 0 = no resizeing occurs
        // 1 = adds just enough for one more component
        // 2 = component memory doubles in size
        // n = component memory size increase by a multiple of (n)
        size_t entityIdToComponentIndiciesMemoryGrowthRate_;

        // The entityId-to-component-indicies memory is not allowed to excede
        // this. If growth will result in a size larger then this, it is capped.
        // 0 = no size cap
        size_t entityIdToComponentIndiciesMemoryCapacityCap_;

    private:
        ComponentContainer(const ComponentContainer&);
        ComponentContainer& operator=(const ComponentContainer&);
    };
    
    // public ------------------------------------------------------------------

    inline ComponentContainer::ComponentContainer()
        : ComponentDestructionFunction_(nullptr)
        , componentTypeId_(0)
        , componentSize_(0)
        , componentMemoryPtr_(nullptr)
        , componentEntityIdMemoryPtr_(nullptr)
        , componentCapacity_(0)
        , componentCount_(0)
        , componentMemoryGrowthRate_(0)
        , componentMemoryCapacityCap_(0)
        , entityIdToComponentIndiciesMemoryPtr_(nullptr)
        , entityIdCapacity_(0)
        , entityIdToComponentIndiciesMemoryGrowthRate_(0)
        , entityIdToComponentIndiciesMemoryCapacityCap_(0)
    {}

    inline ComponentContainer::~ComponentContainer()
    {
        if (ComponentDestructionFunction_ != nullptr)
        {
            for (size_t i = 0; i < componentCount_; ++i)
            {
                uint8_t* deletedComponent = componentMemoryPtr_ + i * componentSize_;
                ComponentDestructionFunction_(deletedComponent);
            }
        }

        free(componentMemoryPtr_);
        free(componentEntityIdMemoryPtr_);
        free(entityIdToComponentIndiciesMemoryPtr_);
    }

    template <typename Component>
    inline bool ComponentContainer::Initialize(
        const size_t& initialComponentCapacity,
        const size_t& componentMemoryGrowthRate,
        const size_t& componentMemoryCapacityCap,
        const size_t& initialEntityIdCapacity,
        const size_t& entityIdToComponentIndiciesMemoryGrowthRate,
        const size_t& entityIdToComponentIndiciesMemoryCapacityCap)
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

        entityIdCapacity_ = initialEntityIdCapacity;

        free(entityIdToComponentIndiciesMemoryPtr_);
        entityIdToComponentIndiciesMemoryPtr_ = (int*)malloc(sizeof(size_t) * entityIdCapacity_);
        if (entityIdToComponentIndiciesMemoryPtr_ == nullptr)
        {
            return false;
        }

        for (size_t i = 0; i < entityIdCapacity_; ++i)
        {
            *(entityIdToComponentIndiciesMemoryPtr_ + i) = -1;
        }

        entityIdToComponentIndiciesMemoryGrowthRate_ = entityIdToComponentIndiciesMemoryGrowthRate;

        entityIdToComponentIndiciesMemoryCapacityCap_ = entityIdToComponentIndiciesMemoryCapacityCap;

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

        if (entityId >= entityIdCapacity_)
        {
            size_t newEntityIdCapacity;
            if (entityIdToComponentIndiciesMemoryGrowthRate_ == 0)
            {
                newEntityIdCapacity = entityIdCapacity_;
            }
            else if (entityIdToComponentIndiciesMemoryGrowthRate_ == 1)
            {
                newEntityIdCapacity = entityId;
            }
            else // (entityIdToComponentIndiciesMemoryGrowthRate_ > 1)
            {
                newEntityIdCapacity = entityIdCapacity_;
                size_t prevNewEntityIdCapacity;
                while (newEntityIdCapacity <= entityId)
                {
                    prevNewEntityIdCapacity = newEntityIdCapacity;

                    newEntityIdCapacity *= entityIdToComponentIndiciesMemoryGrowthRate_;

                    if (newEntityIdCapacity < prevNewEntityIdCapacity)
                    {
                        // size_t overflow
                        return false;
                    }
                }

                if (entityIdToComponentIndiciesMemoryCapacityCap_ != 0 &&
                    newEntityIdCapacity > entityIdToComponentIndiciesMemoryCapacityCap_)
                {
                    newEntityIdCapacity = entityIdToComponentIndiciesMemoryCapacityCap_;
                }
            }

            if (entityId >= newEntityIdCapacity)
            {
                return false;
            }

            int* newEntityIdToComponentIndiciesMemoryPtr = (int*)malloc(sizeof(int) * newEntityIdCapacity);
            if (newEntityIdToComponentIndiciesMemoryPtr == nullptr)
            {
                return false;
            }

            ::memcpy(newEntityIdToComponentIndiciesMemoryPtr, entityIdToComponentIndiciesMemoryPtr_, sizeof(int) * entityIdCapacity_);
            for (size_t i = entityIdCapacity_; i < newEntityIdCapacity; ++i)
            {
                *(newEntityIdToComponentIndiciesMemoryPtr + i) = -1;
            }

            free(entityIdToComponentIndiciesMemoryPtr_);
            entityIdToComponentIndiciesMemoryPtr_ = newEntityIdToComponentIndiciesMemoryPtr;

            entityIdCapacity_ = newEntityIdCapacity;
        }
        else if (*(entityIdToComponentIndiciesMemoryPtr_ + entityId) != -1)
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

        *(entityIdToComponentIndiciesMemoryPtr_ + entityId) = (int)componentCount_;

        componentCount_++;

        return true;
    }

    inline bool ComponentContainer::DeleteComponent(const unsigned int& entityId)
    {
        if (ComponentDestructionFunction_ == nullptr ||
            entityId > entityIdCapacity_ ||
            *(entityIdToComponentIndiciesMemoryPtr_ + entityId) < 0)
        {
            return false;
        }

        int deletedComponentMemoryIndex = *(entityIdToComponentIndiciesMemoryPtr_ + entityId);
        uint8_t* deletedComponentPtr = (uint8_t*)componentMemoryPtr_ + deletedComponentMemoryIndex * componentSize_;

        uint8_t* lastComponentPtr = (uint8_t*)componentMemoryPtr_ + (componentCount_ - 1) * componentSize_;

        unsigned int lastComponentEntityId = *(componentEntityIdMemoryPtr_ + (componentCount_ - 1));

        ComponentDestructionFunction_(deletedComponentPtr);
        *(entityIdToComponentIndiciesMemoryPtr_ + entityId) = -1;

        if (entityId != lastComponentEntityId)
        {
            ::memcpy(deletedComponentPtr, lastComponentPtr, componentSize_);
            *(componentEntityIdMemoryPtr_ + deletedComponentMemoryIndex) = lastComponentEntityId;

            *(entityIdToComponentIndiciesMemoryPtr_ + lastComponentEntityId) = deletedComponentMemoryIndex;
        }

        componentCount_--;

        return true;
    }

    inline bool ComponentContainer::DeleteAllComponents()
    {
        if (ComponentDestructionFunction_ == nullptr)
        {
            return false;
        }

        for (size_t i = 0; i < componentCount_; ++i)
        {
            uint8_t* deletedComponent = (uint8_t*)componentMemoryPtr_ + i * componentSize_;
            ComponentDestructionFunction_(deletedComponent);
        }

        componentCount_ = 0;

        for (size_t i = 0; i < entityIdCapacity_; ++i)
        {
            *(entityIdToComponentIndiciesMemoryPtr_ + i) = -1;
        }

        return true;
    }

    template <typename Component>
    inline bool ComponentContainer::GetComponent(const unsigned int& entityId, Component*& componentPtr)
    {
        size_t componentTypeId = typeid(Component).hash_code();
        if (componentTypeId != componentTypeId_)
        {
            return false;
        }

        if (entityId > entityIdCapacity_ || *(entityIdToComponentIndiciesMemoryPtr_ + entityId) < 0)
        {
            return false;
        }

        int componentIndex = *(entityIdToComponentIndiciesMemoryPtr_ + entityId);
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
    inline bool ComponentContainer::GetComponentEntityId(const Component* const& componentPtr, unsigned int& entityId) const
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