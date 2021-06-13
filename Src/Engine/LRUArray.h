#pragma once

#include <queue>



namespace Project001
{
    template <class T>
    class LRUArray
    {
    public:
        LRUArray(size_t capacity)
        {
            if (capacity <= 0)
            {
                capacity = 1;
            }
            
            capacity_ = (unsigned int)capacity;
            dataPtr_ = new T[capacity];
            lastRecentlyUsedValuesPtr_ = new unsigned int[capacity];

            for (unsigned int i = 0; i < capacity; ++i)
            {
                lastRecentlyUsedValuesPtr_[i] = i;
            }
        }

        LRUArray(const LRUArray<T>& other)
        {
            capacity_ = other.capacity_;
            dataPtr_ = new T[capacity_];
            for (unsigned int i = 0; i < capacity_; i++)
            {
                dataPtr_[i] = other[i];
            }
        }

        ~LRUArray()
        {
            if (dataPtr_ != nullptr)
            {
                delete dataPtr_;
            }
            if (lastRecentlyUsedValuesPtr_ != nullptr)
            {
                delete lastRecentlyUsedValuesPtr_;
            }
        }

        T& operator[](size_t i)
        {
            return dataPtr_[i];
        }

        const T& operator[](size_t i) const
        {
            return dataPtr_[i];
        }

        void Add(T t)
        {
            unsigned int nextIndex = 0;
            for (unsigned int i = 0; i < capacity_; ++i)
            {
                if (lastRecentlyUsedValuesPtr_[i] == 0)
                {
                    lastRecentlyUsedValuesPtr_[i] = (unsigned int)capacity_;
                    nextIndex = i;
                }
                lastRecentlyUsedValuesPtr_[i]--;
            }

            dataPtr_[nextIndex] = t;
        }

        int Find(const T& t) const
        {
            int index = -1;
            for (unsigned int i = 0; i < capacity; ++i)
            {
                if (dataPtr_[i] == T)
                {
                    index = i;
                    break;
                }
            }
            return index;
        }

        T& Get(unsigned int index)
        {
            unsigned int indexValue = lastRecentlyUsedValuesPtr_[index];
            lastRecentlyUsedValuesPtr_[index] = (unsigned int)capacity_;
            for (unsigned int i = 0; i < capacity_; ++i)
            {
                if (lastRecentlyUsedValuesPtr_[i] > indexValue)
                {
                    lastRecentlyUsedValuesPtr_[i]--;
                }
            }
            return dataPtr_[index];
        }

        inline size_t Size() const
        {
            return capacity_;
        }

    protected:
        size_t capacity_;
        T* dataPtr_;
        unsigned int* lastRecentlyUsedValuesPtr_;

    private:
    };
}