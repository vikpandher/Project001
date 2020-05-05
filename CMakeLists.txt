# Here's a link to a great cmake tutorial:
# https://cliutils.gitlab.io/modern-cmake/

# cmake_minimum_required(VERSION 3.17.0)
cmake_minimum_required(VERSION 3.14.5)

project("Project001" # ${PROJECT_NAME}
    VERSION 0.1.0
    DESCRIPTION "This is project 001."
    LANGUAGES C CXX
)

set_property(GLOBAL
    PROPERTY USE_FOLDERS ON
)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}"
)

set("EXECUTABLE_NAME" "${PROJECT_NAME}")

add_executable("${EXECUTABLE_NAME}"
    "Src/glad.c"
    "Src/Main.cpp"
    "Src/Shaders.h"
)

target_link_directories("${EXECUTABLE_NAME}"
    PUBLIC
    "Lib"
)

target_link_libraries("${EXECUTABLE_NAME}"
    "glfw3.lib" # This is a 64 bit library
)

target_include_directories("${EXECUTABLE_NAME}"
    PUBLIC 
    "Include"
)