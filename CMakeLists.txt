# ==============================================================================
# @AUTHOR Vik Pandher
# @DATE 2024-12-22

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

cmake_minimum_required(VERSION 3.22.0)

# https://github.com/vikpandher/FileToHeaderConverter.git
set(VENDOR_FILE_TO_HEADER_CONVERTER_BINARY "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/FileToHeaderConverter/Bin/FileToHeaderConverter.exe" CACHE PATH "")

# https://glad.dav1d.de/
set(VENDOR_GLAD_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/include" CACHE PATH "")
set(VENDOR_GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/src" CACHE PATH "")

# https://github.com/glfw/glfw.git
set(VENDOR_GLFW_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw/include" CACHE PATH "")
set(VENDOR_GLFW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw/build/src/Release/glfw3.lib" CACHE PATH "")

# https://github.com/g-truc/glm.git
set(VENDOR_GLM_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glm" CACHE PATH "")

# https://github.com/mackron/miniaudio.git
set(VENDOR_MINIAUDIO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/miniaudio" CACHE PATH "")

# https://github.com/nothings/stb.git
set(VENDOR_STB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb" CACHE PATH "")

# https://vulkan.lunarg.com/
set(VENDOR_VULKAN_DIRECTORY "C:/VulkanSDK/1.3.261.1" CACHE PATH "")
set(VENDOR_VULKAN_LIBRARY "${VENDOR_VULKAN_DIRECTORY}/Lib/vulkan-1.lib" CACHE PATH "")

# If TRUE, sets renderer to Vulkan
# If FALSE, renderer is set to OpenGL
option(PROJECT001_VULKAN_BUILD "If TRUE, sets renderer to Vulkan. If FALSE, renderer is set to OpenGL" FALSE)

option(PROJECT001_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# ##############################################################################

project("Project001" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION ""
    LANGUAGES C CXX
)

add_custom_target("${PROJECT_NAME}" SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
)

if (${PROJECT001_VULKAN_BUILD})
    set(PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX "_v")
else()
    set(PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX "_o")
endif()

if(EXISTS "${VENDOR_VULKAN_DIRECTORY}/Bin/glslc.exe")
    file(WRITE "Scripts/glslc_path.txt" "${VENDOR_VULKAN_DIRECTORY}/Bin/glslc.exe")
else()
    message("${VENDOR_VULKAN_DIRECTORY}/Bin/glslc.exe NOT FOUND")
endif()

if(EXISTS "${VENDOR_FILE_TO_HEADER_CONVERTER_BINARY}")
    file(WRITE "Scripts/FileToHeaderConverter_path.txt" "${VENDOR_FILE_TO_HEADER_CONVERTER_BINARY}")
else()
    message("${VENDOR_FILE_TO_HEADER_CONVERTER_BINARY} NOT FOUND")
endif()

# ##############################################################################

if(${PROJECT001_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("VENDOR_FILE_TO_HEADER_CONVERTER_BINARY: ${VENDOR_FILE_TO_HEADER_CONVERTER_BINARY}")

    message("VENDOR_GLAD_INCLUDE: ${VENDOR_GLAD_INCLUDE}")
    message("VENDOR_GLAD_SOURCE: ${VENDOR_GLAD_SOURCE}")

    message("VENDOR_GLFW_INCLUDE: ${VENDOR_GLFW_INCLUDE}")
    message("VENDOR_GLFW_LIBRARY: ${VENDOR_GLFW_LIBRARY}")

    message("VENDOR_GLM_INCLUDE: ${VENDOR_GLM_INCLUDE}")

    message("VENDOR_MINIAUDIO_INCLUDE: ${VENDOR_MINIAUDIO_INCLUDE}")

    message("VENDOR_STB_INCLUDE: ${VENDOR_STB_INCLUDE}")

    message("VENDOR_VULKAN_DIRECTORY: ${VENDOR_VULKAN_DIRECTORY}")
    message("VENDOR_VULKAN_INCLUDE: ${VENDOR_VULKAN_INCLUDE}")

    message("PROJECT001_VULKAN_BUILD: ${PROJECT001_VULKAN_BUILD}")

    message("PROJECT_NAME: ${PROJECT_NAME}")
    message("PROJECT_VERSION: ${PROJECT_VERSION}")
    message("PROJECT_DESCRIPTION: ${PROJECT_DESCRIPTION}")
    
    get_property(_ENABLED_LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
    message("ENABLED_LANGUAGES:")
    foreach(VARIABLE ${_ENABLED_LANGUAGES})
        message("    ${VARIABLE}")
    endforeach()

    message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}") # ROOT DIR, top level CMakeLists.txt location
    message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}") # this CMakeLists.txt location
    message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

    message("CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

    message("--------------------------------------------------------------------------------")
endif()

# ##############################################################################

add_subdirectory("Framework")
add_subdirectory("TestApplication")
add_dependencies("${TESTAPP_PROJECT_NAME}" "${FRAMEWORK_PROJECT_NAME}")
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${TESTAPP_PROJECT_NAME}")

# ##############################################################################