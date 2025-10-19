// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-18

#include "UniqueIdGenerator.h"

#include <algorithm>



namespace Project001
{
    // public ------------------------------------------------------------------

    UniqueIdGenerator::UniqueIdGenerator(unsigned int minimumId)
        : minimumId_(minimumId)
        , availableIdQueueSorted_(true)
    {}

    void UniqueIdGenerator::GetNewId(unsigned int& id)
    {
        unsigned int internalId;

        if (availableIds_.empty())
        {
            internalId = (unsigned int)availableIdFlags_.size();
            availableIdFlags_.push_back(false);
        }
        else
        {
            if (!availableIdQueueSorted_)
            {
                std::sort(availableIds_.begin(), availableIds_.end());
                availableIdQueueSorted_ = true;
            }
            internalId = availableIds_.front();
            availableIds_.pop_front();
            availableIdFlags_[internalId] = false;
        }

        id = internalId + minimumId_;
    }

    bool UniqueIdGenerator::ReturnId(const unsigned int& id)
    {
        if (IdIsAvailable(id))
        {
            return false;
        }

        unsigned int internalId = id - minimumId_;

        availableIdQueueSorted_ = false;
        availableIds_.push_back(internalId);
        availableIdFlags_[internalId] = true;

        return true;
    }

    void UniqueIdGenerator::ReturnAllIds()
    {
        availableIdQueueSorted_ = true;
        availableIds_.clear();
        availableIdFlags_.clear();
    }

    bool UniqueIdGenerator::IdIsAvailable(const unsigned int& id)
    {
        if (id < minimumId_)
        {
            return false;
        }

        unsigned int internalId = id - minimumId_;

        return internalId >= availableIdFlags_.size() || availableIdFlags_[internalId];
    }
}