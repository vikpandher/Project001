#pragma once

#include <deque>
#include <vector>



namespace Project001
{
    class UniqueIdGenerator
    {
    public:
        UniqueIdGenerator();

        void GetNewId(unsigned int& id);

        bool ReturnId(const unsigned int& id);

        void ReturnAllIds();

        bool IdIsTaken(const unsigned int& id);

    protected:
        bool returnedIdQueueSorted_;
        std::deque<unsigned int> returnedIds_;
        std::vector<bool> returnedIdFlags_;
    };
}