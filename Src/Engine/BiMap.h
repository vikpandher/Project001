#pragma once

#include <map>



namespace Project001
{
    // Bi-directional Map
    template <class X, class Y>
    class BiMap
    {
    public:
        void Add(X x, Y y)
        {
            if (Find_X(x))
            {
                Remove_Using_X(x);
            }
            if (Find_Y(y))
            {
                Remove_Using_Y(y);
            }

            mapXY_[x] = y;
            mapYX_[y] = x;
        }

        void Clear()
        {
            mapXY_.clear();
            mapYX_.clear();
        }

        bool Find_X(X x)
        {
            bool success = false;

            if (mapXY_.find(x) != mapXY_.end())
            {
                success = true;
            }

            return success;
        }

        Y& Get_Using_X(X x)
        {
            return mapXY_[x];
        }

        void Remove_Using_X(X x)
        {
            Y& y = mapXY_[x];
            std::map<Y, X>::iterator iter = mapYX_.find(y);
            mapXY_.erase(x);
            mapYX_.erase(iter);
        }

        bool Find_Y(Y y)
        {
            bool success = false;

            if (mapYX_.find(y) != mapYX_.end())
            {
                success = true;
            }

            return success;
        }

        X& Get_Using_Y(Y y)
        {
            return mapYX_[y];
        }

        void Remove_Using_Y(Y y)
        {
            X& x = mapYX_[y];
            std::map<X, Y>::iterator iter = mapXY_.find(x);
            mapYX_.erase(y);
            mapXY_.erase(iter);
        }

        size_t Size()
        {
            return mapYX_.size();
        }
        
    protected:
        std::map<X, Y> mapXY_;
        std::map<Y, X> mapYX_;

    private:
    };
}