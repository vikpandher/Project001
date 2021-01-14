# cmake_minimum_required(VERSION 3.17.0)
cmake_minimum_required(VERSION 3.14.5)

option(PRINT_DEBUG_OUTPUT "" TRUE)

project("Project001" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION "This is project 001."
    LANGUAGES C CXX
)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

set(SOURCE_FILES
    "Src/Main.cpp"
)

source_group("Src" FILES ${SOURCE_FILES})

set(ENGINE_FILES
    "Src/Engine/Application.cpp"
    "Src/Engine/Application.h"
    "Src/Engine/Event.h"
    "Src/Engine/Logger.cpp"
    "Src/Engine/Logger.h"
    "Src/Engine/ModelData.h"
    "Src/Engine/RenderData.h"
    "Src/Engine/ResourceStores.cpp"
    "Src/Engine/ResourceStores.h"
    "Src/Engine/TextureData.h"
    "Src/Engine/Window.h"
)

source_group("Src/Engine" FILES ${ENGINE_FILES})

set(PLATFORM_FILES
    "Src/Platform/glad.c"
    "Src/Platform/OpenGLShader.cpp"
    "Src/Platform/OpenGLShader.h"
    "Src/Platform/OpenGLTexture.cpp"
    "Src/Platform/OpenGLTexture.h"
    "Src/Platform/OpenGLWindow.cpp"
    "Src/Platform/OpenGLWindow.h"
    "Src/Platform/Shaders.h"
)

source_group("Src/Platform" FILES ${PLATFORM_FILES})

# adding WIN32 makes WinMain() the entry point and so no conole appears
add_executable("${PROJECT_NAME}" # WIN32
    ${SOURCE_FILES}
    ${ENGINE_FILES}
    ${PLATFORM_FILES}
)

target_include_directories("${PROJECT_NAME}"
    PUBLIC 
    "Include"
    "Src"
)

target_link_directories("${PROJECT_NAME}"
    PUBLIC
    "Lib"
)

target_link_libraries("${PROJECT_NAME}"
    "glfw3.lib" # This is a 64 bit library
)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Bin")
set_target_properties("${PROJECT_NAME}" PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}_debug"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
)

if(${PRINT_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")
    message("PROJECT_NAME: ${PROJECT_NAME}")
    message("PROJECT_VERSION: ${PROJECT_VERSION}")
    message("PROJECT_DESCRIPTION: ${PROJECT_DESCRIPTION}")
    
    get_property(_ENABLED_LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
    message("ENABLED_LANGUAGES:")
    foreach(VARIABLE ${_ENABLED_LANGUAGES})
        message("    ${VARIABLE}")
    endforeach()
    
    message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
    message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
    message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
    
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
    
    message("OUTPUT_DIR: ${OUTPUT_DIR}")

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
