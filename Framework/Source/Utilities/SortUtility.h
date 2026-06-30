// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-06-29

#pragma once

#include <vector>
#include <functional>
#include <algorithm> // For std::min



namespace Project001
{
namespace Sort
{
    // Ping Pong Merge Sort
    template <typename T, typename Allocator = std::allocator<T>, typename Comparator = std::less<T>>
    void MergeSort(std::vector<T, Allocator>& vec, Comparator comp = Comparator{})
    {
        const size_t n = vec.size();
        if (n <= 1)
        {
            return;
        }

        std::vector<T, Allocator> temp(n);

        std::vector<T, Allocator>* src = &vec;
        std::vector<T, Allocator>* dest = &temp;

        // sub array size: 1, 2, 4, 8, ...
        for (size_t width = 1; width < n; width *= 2)
        {
            for (size_t left = 0; left < n; left += 2 * width)
            {
                size_t mid = left + width - 1;
                size_t right = std::min(left + (2 * width) - 1, n - 1);

                if (mid >= right)
                {
                    // no right sub-array
                    for (size_t idx = left; idx <= right; ++idx)
                    {
                        (*dest)[idx] = std::move((*src)[idx]);
                    }
                    continue;
                }

                size_t i = left;    // Left sub-array index
                size_t j = mid + 1; // Right sub-array index
                size_t k = left;    // Destination index

                // sub array merge logic
                while (i <= mid && j <= right)
                {
                    if (comp((*src)[i], (*src)[j]))
                    {
                        (*dest)[k++] = std::move((*src)[i++]);
                    }
                    else
                    {
                        (*dest)[k++] = std::move((*src)[j++]);
                    }
                }

                // move remaining items
                while (i <= mid)
                {
                    (*dest)[k++] = std::move((*src)[i++]);
                }
                while (j <= right)
                {
                    (*dest)[k++] = std::move((*src)[j++]);
                }
            }

            // swith working vector (Ping Pong)
            std::swap(src, dest);
        }

        // make sure the sorted result is back in the input vector
        if (src == &temp)
        {
            for (size_t idx = 0; idx < n; ++idx)
            {
                vec[idx] = std::move(temp[idx]);
            }
        }
    }
}
}