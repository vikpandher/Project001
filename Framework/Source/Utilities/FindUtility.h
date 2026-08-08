// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-07

#pragma once

#include <vector>



namespace Project001
{
namespace Find
{
    template <typename T>
    bool Contains(const std::vector<T>& vec, const T& value)
    {
        for (const auto& element : vec)
        {
            if (element == value)
            {
                return true;
            }
        }
        return false;
    }
}
}