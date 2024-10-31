// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include <vulkan/vulkan.h>

#include "Logger.h"


#ifndef NDEBUG
#define _VK_CHECK(f)\
{\
    VkResult result = (f);\
    if (result != VK_SUCCESS)\
    {\
        LOG_ERROR_F("Vulkan error code: " << result);\
    }\
} do {} while(0)
#else
#define _VK_CHECK(f) f
#endif