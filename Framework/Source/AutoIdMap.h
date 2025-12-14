// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#pragma once

#include <algorithm>
#include <deque>
#include <unordered_map>



namespace Project001
{
    // Automatic Id Generating Map
    // 
    // This map generates an Id as the key when a value is added to it. It will
    // also reuse Ids for values that have been erased.
    template <class Value>
    class AutoIdMap
    {
    public:
        using iterator = typename std::unordered_map<unsigned int, Value>::iterator;
        using const_iterator = typename std::unordered_map<unsigned int, Value>::const_iterator;

        AutoIdMap()
            : erasedIdQueueSorted_(true)
        {}

        iterator Add(unsigned int& id, const Value& value)
        {
            if (erasedIds_.empty())
            {
                id = static_cast<unsigned int>(idToValueMap_.size());
            }
            else
            {
                if (!erasedIdQueueSorted_)
                {
                    std::sort(erasedIds_.begin(), erasedIds_.end());
                    erasedIdQueueSorted_ = true;
                }
                id = erasedIds_.front();
                erasedIds_.pop_front();
            }
            return idToValueMap_.insert({ id, value }).first;
        }

        iterator Find(unsigned int id)
        {
            return idToValueMap_.find(id);
        }

        const_iterator Find(unsigned int id) const
        {
            return idToValueMap_.find(id);
        }

        iterator Erase(iterator iter)
        {
            if (iter != idToValueMap_.end())
            {
                erasedIdQueueSorted_ = false;
                erasedIds_.push_back(iter->first);
                return idToValueMap_.erase(iter);
            }
            return idToValueMap_.end();
        }

        iterator Erase(unsigned int id)
        {
            iterator iter = idToValueMap_.find(id);
            if (iter != idToValueMap_.end())
            {
                erasedIdQueueSorted_ = false;
                erasedIds_.push_back(id);
                return idToValueMap_.erase(iter);
            }
            return idToValueMap_.end();
        }

        void Clear()
        {
            erasedIdQueueSorted_ = true;
            erasedIds_.clear();
            idToValueMap_.clear();
        }

        size_t Size() const
        {
            return idToValueMap_.size();
        }

        iterator IteratorAtBeginning()
        {
            return idToValueMap_.begin();
        }

        const_iterator IteratorAtBeginning() const
        {
            return idToValueMap_.begin();
        }

        iterator IteratorPastTheEnd()
        {
            return idToValueMap_.end();
        }

        const_iterator IteratorPastTheEnd() const
        {
            return idToValueMap_.end();
        }

    protected:
        bool erasedIdQueueSorted_;
        std::deque<unsigned int> erasedIds_;
        std::unordered_map<unsigned int, Value> idToValueMap_;
    };
}