#include "UniqueIdGenerator.h"

#include <algorithm>



namespace Project001
{
    // public ------------------------------------------------------------------

    UniqueIdGenerator::UniqueIdGenerator()
        : returnedIdQueueSorted_(true)
    {}

    void UniqueIdGenerator::GetNewId(unsigned int& id)
    {
        if (returnedIds_.empty())
        {
            id = (unsigned int)returnedIdFlags_.size();
            returnedIdFlags_.push_back(false);
        }
        else
        {
            if (!returnedIdQueueSorted_)
            {
                std::sort(returnedIds_.begin(), returnedIds_.end());
                returnedIdQueueSorted_ = true;
            }
            id = returnedIds_.front();
            returnedIds_.pop_front();

            returnedIdFlags_[id] = false;
        }
    }

    bool UniqueIdGenerator::ReturnId(const unsigned int& id)
    {
        if (!IdIsTaken(id))
        {
            return false;
        }

        returnedIdQueueSorted_ = false;
        returnedIds_.push_back(id);

        returnedIdFlags_[id] = true;

        return true;
    }

    void UniqueIdGenerator::ReturnAllIds()
    {
        returnedIdQueueSorted_ = true;
        returnedIds_.clear();
        returnedIdFlags_.clear();
    }

    bool UniqueIdGenerator::IdIsTaken(const unsigned int& id)
    {
        return id < returnedIdFlags_.size() && !returnedIdFlags_[id];
    }
}