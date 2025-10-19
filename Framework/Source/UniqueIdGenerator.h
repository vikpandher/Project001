// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-18

#pragma once

#include <deque>
#include <vector>



namespace Project001
{
    class UniqueIdGenerator
    {
    public:
        UniqueIdGenerator(unsigned int minimumId = 1);

        void GetNewId(unsigned int& id);

        bool ReturnId(const unsigned int& id);

        void ReturnAllIds();

        bool IdIsAvailable(const unsigned int& id);

    protected:
        unsigned int minimumId_;
        bool availableIdQueueSorted_;
        std::deque<unsigned int> availableIds_;
        std::vector<bool> availableIdFlags_;
    };
}