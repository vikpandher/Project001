# ==============================================================================
# @AUTHOR Vik Pandher
# @DATE 2025-09-21

cmake_minimum_required(VERSION 3.22.0)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

set(TESTAPP_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Binary" CACHE PATH "")
set(TESTAPP_PROJECT_NAME "Project001_TestApp" CACHE PATH "") # Name without postfix and extension

option(TESTAPP_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# If TRUE, removes the console on Windows builds
# If FALSE, Windows builds have a console
option(TESTAPP_WINDOWS_NO_CONSOLE "If TRUE, removes the console on Windows builds. If FALSE, Windows builds have a console." FALSE)

# If TRUE, adds memory leak check to Windows builds
# If FALSE, doesn't
option(TESTAPP_WINDOWS_MEMORY_LEAK_CHECK "If TRUE, adds memory leak check to Windows builds. If False, doesn't." TRUE)

# ##############################################################################

project("${TESTAPP_PROJECT_NAME}" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION ""
    LANGUAGES C CXX
)

# C++11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

set(TESTAPP_FILES
    "Source/Main.cpp"
    "Source/TestAnimatedSpriteScene001.cpp"
    "Source/TestAnimatedSpriteScene001.h"
    "Source/TestApplicationData.h"
    "Source/TestButtonData001.h"
    "Source/TestComponents.h"
    "Source/TestInstructionScene001.cpp"
    "Source/TestInstructionScene001.h"
    "Source/TestResource_AntonioRegular_png.h"
    "Source/TestResource_AntonioRegular_ssf.h"
    "Source/TestResource_Bounce_wav.h"
    "Source/TestResource_Congratulations_ogg.h"
    "Source/TestResource_CounterclockwiseDie_png.h"
    "Source/TestResource_Cube_obj.h"
    "Source/TestScene001.cpp"
    "Source/TestScene001.h"
    "Source/TestScene002.cpp"
    "Source/TestScene002.h"
    "Source/TestScene003.cpp"
    "Source/TestScene003.h"
    "Source/TestScene004.cpp"
    "Source/TestScene004.h"
    "Source/TestScene006.cpp"
    "Source/TestScene006.h"
    "Source/TestScene007.cpp"
    "Source/TestScene007.h"
    "Source/TestScene008.cpp"
    "Source/TestScene008.h"
    "Source/TestScene009.cpp"
    "Source/TestScene009.h"
    "Source/TestScene010.cpp"
    "Source/TestScene010.h"
    "Source/TestScene011.cpp"
    "Source/TestScene011.h"
    "Source/TestScene012.cpp"
    "Source/TestScene012.h"
    "Source/TestScene013.cpp"
    "Source/TestScene013.h"
    "Source/TestScene015.cpp"
    "Source/TestScene015.h"
    "Source/TestScene016.cpp"
    "Source/TestScene016.h"
    "Source/TestScene017.cpp"
    "Source/TestScene017.h"
    "Source/TestScene023.cpp"
    "Source/TestScene023.h"
    "Source/TestScene030.cpp"
    "Source/TestScene030.h"
    "Source/TestScene031.cpp"
    "Source/TestScene031.h"
    "Source/TestScene032.cpp"
    "Source/TestScene032.h"
    "Source/TestScene033.cpp"
    "Source/TestScene033.h"
    "Source/TestScene034.cpp"
    "Source/TestScene034.h"
    "Source/TestScene050.cpp"
    "Source/TestScene050.h"
    "Source/TestScene051.cpp"
    "Source/TestScene051.h"
    "Source/TestScene052.cpp"
    "Source/TestScene052.h"
    "Source/TestScene060.cpp"
    "Source/TestScene060.h"
    "Source/TestScene101.cpp"
    "Source/TestScene101.h"
    "Source/TestSceneBase001.cpp"
    "Source/TestSceneBase001.h"
    "Source/TestSceneBase002.cpp"
    "Source/TestSceneBase002.h"
)

source_group("Source" FILES ${TESTAPP_FILES})

if (${TESTAPP_WINDOWS_NO_CONSOLE})
    # adding WIN32 makes WinMain() the entry point and so no conole appears
    set(CMAKE_WIN32 WIN32)
endif()

add_executable("${PROJECT_NAME}" ${CMAKE_WIN32}
    ${TESTAPP_FILES}
)

# target_link_options(${PROJECT_NAME} PRIVATE "/NODEFAULTLIB:msvcrt.lib")
# target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")

if (${PROJECT001_VULKAN_BUILD})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "VULKAN_BUILD")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_VULKAN")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_DEPTH_ZERO_TO_ONE")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_LEFT_HANDED"") # flipped the viewport instead
endif()

if (${TESTAPP_WINDOWS_NO_CONSOLE})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS_NO_CONSOLE")
endif()

if (${TESTAPP_WINDOWS_MEMORY_LEAK_CHECK})
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
    VS_DEBUGGER_WORKING_DIRECTORY "${TESTAPP_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${TESTAPP_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TESTAPP_OUTPUT_DIRECTORY}"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}_d"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}"
)

# ##############################################################################

if(${TESTAPP_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("TESTAPP_OUTPUT_DIRECTORY:" ${TESTAPP_OUTPUT_DIRECTORY})
    message("TESTAPP_PROJECT_NAME:" ${TESTAPP_PROJECT_NAME})

    message("TESTAPP_WINDOWS_NO_CONSOLE: ${TESTAPP_WINDOWS_NO_CONSOLE}")
    message("TESTAPP_WINDOWS_MEMORY_LEAK_CHECK: ${TESTAPP_WINDOWS_MEMORY_LEAK_CHECK}")

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