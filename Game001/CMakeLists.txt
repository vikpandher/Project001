# ==============================================================================
# @AUTHOR Vik Pandher
# @DATE 2025-11-30

cmake_minimum_required(VERSION 3.22.0)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

set(GAME001_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binary" CACHE PATH "")
set(GAME001_PROJECT_NAME "Game001" CACHE PATH "") # Name without postfix and extension

option(GAME001_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# If TRUE, removes the console on Windows builds
# If FALSE, Windows builds have a console
option(GAME001_WINDOWS_NO_CONSOLE "If TRUE, removes the console on Windows builds. If FALSE, Windows builds have a console." FALSE)

# If TRUE, adds memory leak check to Windows builds
# If FALSE, doesn't
option(GAME001_WINDOWS_MEMORY_LEAK_CHECK "If TRUE, adds memory leak check to Windows builds. If False, doesn't." TRUE)

# ##############################################################################

project("${GAME001_PROJECT_NAME}" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION ""
    LANGUAGES C CXX
)

# C++11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GAME001_FILES
    "Source/Main.cpp"
    "Source/Scene001.cpp"
    "Source/Scene001.h"
    "Source/Scene002.cpp"
    "Source/Scene002.h"
    "Source/SharedApplicationData.h"
)

source_group("Source" FILES ${GAME001_FILES})

set(GAME001_RESOURCE_FILES
    "Source/Resources/dropCoin_wav.h"
    "Source/Resources/gridDark_64x64_png.h"
    "Source/Resources/gridLit_64x64_png.h"
    "Source/Resources/hitHurt_wav.h"
    "Source/Resources/houseDark_384x256_png.h"
    "Source/Resources/houseLit_384x256_png.h"
    "Source/Resources/lampDark_32x160_png.h"
    "Source/Resources/lampLit_32x160_png.h"
    "Source/Resources/monsterLit_32x56_png.h"
    "Source/Resources/pickupCoin_wav.h"
    "Source/Resources/personLit_32x56_png.h"
    "Source/Resources/unknownDark_32x56_png.h"
)

source_group("Source/Resources" FILES ${GAME001_RESOURCE_FILES})

if (${GAME001_WINDOWS_NO_CONSOLE})
    # adding WIN32 makes WinMain() the entry point and so no conole appears
    set(CMAKE_WIN32 WIN32)
endif()

add_executable("${PROJECT_NAME}" ${CMAKE_WIN32}
    ${GAME001_FILES}
    ${GAME001_RESOURCE_FILES}
)

# target_link_options(${PROJECT_NAME} PRIVATE "/NODEFAULTLIB:msvcrt.lib")
# target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")

if (${PROJECT001_VULKAN_BUILD})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "VULKAN_BUILD")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_VULKAN")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_DEPTH_ZERO_TO_ONE")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_LEFT_HANDED"") # flipped the viewport instead
endif()

if (${GAME001_WINDOWS_NO_CONSOLE})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS_NO_CONSOLE")
endif()

if (${GAME001_WINDOWS_MEMORY_LEAK_CHECK})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS_MEMORY_LEAK_CHECK")
endif()

target_include_directories("${PROJECT_NAME}"
    PRIVATE 
    "${FRAMEWORK_SOURCE}"
    "${VENDOR_GLM_INCLUDE}"
    "Source"
)

# target_link_directories("${PROJECT_NAME}"
#     PRIVATE
# )

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    $<$<CONFIG:Debug>:${VENDOR_GLFW_LIBRARY_D}>
    $<$<CONFIG:Release>:${VENDOR_GLFW_LIBRARY_R}>
    $<$<CONFIG:Debug>:${FRAMEWORK_OUTPUT_DIRECTORY}/${FRAMEWORK_PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}_d.lib>
    $<$<CONFIG:Release>:${FRAMEWORK_OUTPUT_DIRECTORY}/${FRAMEWORK_PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}.lib>
)

if(${PROJECT001_VULKAN_BUILD})
    target_link_libraries("${PROJECT_NAME}"
        PRIVATE 
        "${VENDOR_VULKAN_LIBRARY}"
    )
endif()

set_target_properties("${PROJECT_NAME}" PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${GAME001_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME001_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME001_OUTPUT_DIRECTORY}"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}_d"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}"
)

# ##############################################################################

if(${GAME001_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("GAME001_OUTPUT_DIRECTORY:" ${GAME001_OUTPUT_DIRECTORY})
    message("GAME001_PROJECT_NAME:" ${GAME001_PROJECT_NAME})

    message("GAME001_WINDOWS_NO_CONSOLE: ${GAME001_WINDOWS_NO_CONSOLE}")
    message("GAME001_WINDOWS_MEMORY_LEAK_CHECK: ${GAME001_WINDOWS_MEMORY_LEAK_CHECK}")

    message("PROJECT_NAME: ${PROJECT_NAME}")
    message("PROJECT_VERSION: ${PROJECT_VERSION}")
    message("PROJECT_DESCRIPTION: ${PROJECT_DESCRIPTION}")

    get_property(_ENABLED_LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
    message("ENABLED_LANGUAGES:")
    foreach(VARIABLE ${_ENABLED_LANGUAGES})
        message("    ${VARIABLE}")
    endforeach()

    message("CMAKE_CXX_FLAGS:")
    message("    ${CMAKE_CXX_FLAGS}")

    message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}") # ROOT DIR, top level CMakeLists.txt location
    message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}") # this CMakeLists.txt location
    message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

    message("CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

    message("CMAKE_CONFIGURATION_TYPES:")
    foreach(VARIABLE ${CMAKE_CONFIGURATION_TYPES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_USE_FOLDERS GLOBAL PROPERTY USE_FOLDERS)
    message("USE_FOLDERS: ${_USE_FOLDERS}")

    get_property(_VS_STARTUP_PROJECT DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT)
    message("VS_STARTUP_PROJECT: ${_VS_STARTUP_PROJECT}")

    get_property(_SOURCES TARGET "${PROJECT_NAME}" PROPERTY SOURCES)
    message("SOURCES:")
    foreach(VARIABLE ${_SOURCES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_COMPILE_DEFINITIONS TARGET "${PROJECT_NAME}" PROPERTY COMPILE_DEFINITIONS)
    message("COMPILE_DEFINITIONS:")
    foreach(VARIABLE ${_COMPILE_DEFINITIONS})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_TARGET_INCLUDE_DIRECTORIES TARGET "${PROJECT_NAME}" PROPERTY INCLUDE_DIRECTORIES)
    message("TARGET_INCLUDE_DIRECTORIES:")
    foreach(VARIABLE ${_TARGET_INCLUDE_DIRECTORIES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_TARGET_LINK_DIRECTORIES TARGET "${PROJECT_NAME}" PROPERTY LINK_DIRECTORIES)
    message("TARGET_LINK_DIRECTORIES:")
    foreach(VARIABLE ${_TARGET_LINK_DIRECTORIES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_TARGET_LINK_LIBRARIES TARGET "${PROJECT_NAME}" PROPERTY LINK_LIBRARIES)
    message("TARGET_LINK_LIBRARIES:")
    foreach(VARIABLE ${_TARGET_LINK_LIBRARIES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_VS_DEBUGGER_WORKING_DIRECTORY TARGET "${PROJECT_NAME}" PROPERTY VS_DEBUGGER_WORKING_DIRECTORY)
    message("VS_DEBUGGER_WORKING_DIRECTORY: ${_VS_DEBUGGER_WORKING_DIRECTORY}")

    get_property(_RUNTIME_OUTPUT_DIRECTORY_DEBUG TARGET "${PROJECT_NAME}" PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG)
    message("RUNTIME_OUTPUT_DIRECTORY_DEBUG: ${_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")

    get_property(_RUNTIME_OUTPUT_DIRECTORY_RELEASE TARGET "${PROJECT_NAME}" PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE)
    message("RUNTIME_OUTPUT_DIRECTORY_RELEASE: ${_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")

    get_property(_OUTPUT_NAME_DEBUG TARGET "${PROJECT_NAME}" PROPERTY OUTPUT_NAME_DEBUG)
    message("OUTPUT_NAME_DEBUG: ${_OUTPUT_NAME_DEBUG}")

    get_property(_OUTPUT_NAME_RELEASE TARGET "${PROJECT_NAME}" PROPERTY OUTPUT_NAME_RELEASE)
    message("OUTPUT_NAME_RELEASE: ${_OUTPUT_NAME_RELEASE}")

    message("--------------------------------------------------------------------------------")
endif()

# ##############################################################################