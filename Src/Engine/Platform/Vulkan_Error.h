#pragma once

#include <vulkan/vulkan.h>

#include "Engine/Logger.h"


#ifndef NDEBUG
#define _VK_CHECK(f)\
{\
    VkResult result = (f);\
    if (result != VK_SUCCESS)\
    {\
        Project001::Logger::Error("Vulkan ErrorCode: %s %d %d", __FILE__, __LINE__, result);\
    }\
}
#else
#define _VK_CHECK(f) f
#endif