# Here's a link to a great cmake tutorial:
# https://cliutils.gitlab.io/modern-cmake/

cmake_minimum_required(VERSION 3.17.0)

project("Project001" # ${PROJECT_NAME}
    VERSION 0.1.0
    DESCRIPTION "This is project 001."
    LANGUAGES C CXX
)

set_property(DIRECTORY "."
    PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}"
)

set("EXECUTABLE_NAME" "${PROJECT_NAME}")

add_executable("${EXECUTABLE_NAME}"
    "Src/glad.c"
    "Src/Main.cpp"
)

target_link_directories("${EXECUTABLE_NAME}"
    PUBLIC
    "Lib"
)

target_link_libraries("${EXECUTABLE_NAME}"
    "glfw3.lib"
)

target_include_directories("${EXECUTABLE_NAME}"
    PUBLIC 
    "Include"
)