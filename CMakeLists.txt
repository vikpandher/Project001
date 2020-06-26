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

set(FRAMEWORK_FILES
    "Src/Framework/Application.cpp"
    "Src/Framework/Application.h"
    "Src/Framework/Project001.h"
)

source_group("Src\\Framework" FILES ${FRAMEWORK_FILES})

set(SOURCE_FILES
    "Src/glad.c"
    "Src/LearnOpenGL.cpp"
    "Src/Main.cpp"
    "Src/Shaders.h"
    "Src/Vertices.h"
)

source_group("Src" FILES ${SOURCE_FILES})

add_executable("${PROJECT_NAME}"
    ${FRAMEWORK_FILES}
    ${SOURCE_FILES}
)

target_include_directories("${PROJECT_NAME}"
    PUBLIC 
    "Include"
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
