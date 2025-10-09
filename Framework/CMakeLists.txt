# ==============================================================================
# @AUTHOR Vik Pandher
# @DATE 2025-10-08

cmake_minimum_required(VERSION 3.22.0)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

set(FRAMEWORK_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Source" CACHE PATH "")
set(FRAMEWORK_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Library" CACHE PATH "")
set(FRAMEWORK_PROJECT_NAME "Project001_Framework" CACHE PATH "") # Name without postfix and extension

option(FRAMEWORK_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# ##############################################################################

project("${FRAMEWORK_PROJECT_NAME}" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION ""
    LANGUAGES C CXX
)

# C++11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GENERAL_FILES
    "Source/Application.cpp"
    "Source/Application.h"
    "Source/AutoIdMap.h"
    "Source/CollisionBodyQuadTree2D.cpp"
    "Source/CollisionBodyQuadTree2D.h"
    "Source/CollisionSystem2D.cpp"
    "Source/CollisionSystem2D.h"
    "Source/ComponentContainer.cpp"
    "Source/ComponentContainer.h"
    "Source/ComponentStores.cpp"
    "Source/ComponentStores.h"
    "Source/Event.h"
    "Source/FontData.h"
    "Source/FontLoader.cpp"
    "Source/FontLoader.h"
    "Source/FontMeshData.h"
    "Source/Logger.cpp"
    "Source/Logger.h"
    "Source/MeshData.h"
    "Source/MeshLoader.cpp"
    "Source/MeshLoader.h"
    "Source/MeshVertex.h"
    "Source/RenderData.h"
    "Source/Renderer.cpp"
    "Source/Renderer.h"
    "Source/RenderSystem.cpp"
    "Source/RenderSystem.h"
    "Source/ResourceStores.cpp"
    "Source/ResourceStores.h"
    "Source/Scene.cpp"
    "Source/Scene.h"
    "Source/SoundData.h"
    "Source/SoundLoader.cpp"
    "Source/SoundLoader.h"
    "Source/SoundPlayer.cpp"
    "Source/SoundPlayer.h"
    "Source/TextureData.h"
    "Source/TextureLoader.cpp"
    "Source/TextureLoader.h"
    "Source/TimeProfiler.cpp"
    "Source/TimeProfiler.h"
    "Source/UniqueBiMap.h"
    "Source/UniqueIdGenerator.cpp"
    "Source/UniqueIdGenerator.h"
    "Source/Window.cpp"
    "Source/Window.h"
)

source_group("Source" FILES ${GENERAL_FILES})

set (COMPONENTS_FILES
    "Source/Components/Camera.cpp"
    "Source/Components/Camera.h"
    "Source/Components/CollisionBody2D.cpp"
    "Source/Components/CollisionBody2D.h"
    "Source/Components/CollisionShape2D.h"
    "Source/Components/LightSource.h"
    "Source/Components/Placement.h"
    "Source/Components/Placement2D.h"
    "Source/Components/Position.h"
    "Source/Components/Position2D.h"
    "Source/Components/RenderedMesh.h"
    "Source/Components/RenderedModel.cpp"
    "Source/Components/RenderedModel.h"
)

source_group("Source/Components" FILES ${COMPONENTS_FILES})

set (MATH_FILES
    "Source/Math/MathUtilities.cpp"
    "Source/Math/MathUtilities.h"
    "Source/Math/Overlap2D.cpp"
    "Source/Math/Overlap2D.h"
    "Source/Math/Overlap3D.cpp"
    "Source/Math/Overlap3D.h"
    "Source/Math/Overlap3D_UnitTests.cpp"
    "Source/Math/Overlap3D_UnitTests.h"
)

source_group("Source/Math" FILES ${MATH_FILES})

set(PLATFORM_GLFW_FILES
    "Source/Platform/GLFW_Window.cpp"
    "Source/Platform/GLFW_Window.h"
)

set(PLATFORM_MINIAUDIO_FILES
    "Source/Platform/MiniAudio_SoundPlayer.cpp"
    "Source/Platform/MiniAudio_SoundPlayer.h"
)

set(PLATFORM_OPENGL_FILES
    "${VENDOR_GLAD_SOURCE}/glad.c"
    "Source/Platform/OpenGL_Mesh.cpp"
    "Source/Platform/OpenGL_Mesh.h"
    "Source/Platform/OpenGL_Renderer.cpp"
    "Source/Platform/OpenGL_Renderer.h"
    "Source/Platform/OpenGL_Shader.cpp"
    "Source/Platform/OpenGL_Shader.h"
    "Source/Platform/OpenGL_Texture.cpp"
    "Source/Platform/OpenGL_Texture.h"
)

set(PLATFORM_VULKAN_FILES
    "Source/Platform/Vulkan_Error.h"
    "Source/Platform/Vulkan_Renderer.cpp"
    "Source/Platform/Vulkan_Renderer.h"
)

source_group("Source/Platform" FILES
    ${PLATFORM_GLFW_FILES}
    ${PLATFORM_MINIAUDIO_FILES}
    ${PLATFORM_OPENGL_FILES}
    ${PLATFORM_VULKAN_FILES}
)

set(PLATFORM_OPENGL_SHADER_FILES
    "Source/Platform/OpenGL_Shaders/o_grid.frag"
    "Source/Platform/OpenGL_Shaders/o_grid.vert"
    "Source/Platform/OpenGL_Shaders/o_normal.frag"
    "Source/Platform/OpenGL_Shaders/o_normal.geom"
    "Source/Platform/OpenGL_Shaders/o_normal.vert"
    "Source/Platform/OpenGL_Shaders/o_primary.frag"
    "Source/Platform/OpenGL_Shaders/o_primary.vert"
    "Source/Platform/OpenGL_Shaders/o_screen.frag"
    "Source/Platform/OpenGL_Shaders/o_screen.vert"
    "Source/Platform/OpenGL_Shaders/o_wireframe.frag"
    "Source/Platform/OpenGL_Shaders/o_wireframe.geom"
    "Source/Platform/OpenGL_Shaders/o_wireframe.vert"
)

source_group("Source/Platform/OpenGL_Shaders" FILES ${PLATFORM_OPENGL_SHADER_FILES})

set(PLATFORM_OPENGL_GENERATED_SHADER_FILES
    "Source/Platform/OpenGL_Shaders/Generated/o_grid_frag.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_grid_vert.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_normal_frag.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_normal_geom.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_normal_vert.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_primary_frag.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_primary_vert.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_screen_frag.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_screen_vert.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_wireframe_frag.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_wireframe_geom.h"
    "Source/Platform/OpenGL_Shaders/Generated/o_wireframe_vert.h"
)

source_group("Source/Platform/OpenGL_Shaders/Generated" FILES ${PLATFORM_OPENGL_GENERATED_SHADER_FILES})

set(PLATFORM_VULKAN_SHADER_FILES
    "Source/Platform/Vulkan_Shaders/v_primary.frag"
    "Source/Platform/Vulkan_Shaders/v_primary.vert"
    "Source/Platform/Vulkan_Shaders/v_screen.frag"
    "Source/Platform/Vulkan_Shaders/v_screen.vert"
)

source_group("Source/Platform/Vulkan_Shaders" FILES ${PLATFORM_VULKAN_SHADER_FILES})

set(PLATFORM_VULKAN_GENERATED_SHADER_FILES
    "Source/Platform/Vulkan_Shaders/Generated/v_primary_frag_spv.h"
    "Source/Platform/Vulkan_Shaders/Generated/v_primary_vert_spv.h"
    "Source/Platform/Vulkan_Shaders/Generated/v_screen_frag_spv.h"
    "Source/Platform/Vulkan_Shaders/Generated/v_screen_vert_spv.h"
)

source_group("Source/Platform/Vulkan_Shaders/Generated" FILES ${PLATFORM_VULKAN_GENERATED_SHADER_FILES})

set (RESOURCE_FILES
    "Source/Resources/PixelFont5x6.cpp"
    "Source/Resources/PixelFont5x6.h"
)

source_group("Source/Resources" FILES ${RESOURCE_FILES})

# The following makes it so the unused graphics platform files doesn't get
# compiled, but the files are still listed in the visual studio project
if(${PROJECT001_VULKAN_BUILD})
    set_source_files_properties(${PLATFORM_OPENGL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set_source_files_properties(${PLATFORM_VULKAN_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

# Statically linking the Visual C++ runtime libraries?
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_library("${PROJECT_NAME}" STATIC
	$<$<BOOL:${PROJECT001_VULKAN_BUILD}>:${VENDOR_VULKAN_INCLUDE}>
    ${GENERAL_FILES}
    ${COMPONENTS_FILES}
    ${MATH_FILES}
    ${PLATFORM_GLFW_FILES}
    ${PLATFORM_MINIAUDIO_FILES}
    ${PLATFORM_OPENGL_FILES}
    ${PLATFORM_OPENGL_SHADER_FILES}
    ${PLATFORM_OPENGL_GENERATED_SHADER_FILES}
    ${PLATFORM_VULKAN_FILES}
    ${PLATFORM_VULKAN_SHADER_FILES}
    ${PLATFORM_VULKAN_GENERATED_SHADER_FILES}
    ${RESOURCE_FILES}
)

# target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")

if (${PROJECT001_VULKAN_BUILD})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "VULKAN_BUILD")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_VULKAN")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_DEPTH_ZERO_TO_ONE")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_LEFT_HANDED"") # flipped the viewport instead
endif()

target_include_directories("${PROJECT_NAME}"
    PRIVATE 
    "${VENDOR_GLAD_INCLUDE}"
    "${VENDOR_GLFW_INCLUDE}"
    "${VENDOR_GLM_INCLUDE}"
    "${VENDOR_MINIAUDIO_INCLUDE}"
    "${VENDOR_STB_INCLUDE}"
    "${VENDOR_VULKAN_INCLUDE}"
    "Source"
)

# target_link_directories("${PROJECT_NAME}"
#     PRIVATE
# )

# target_link_libraries("${PROJECT_NAME}"
#     PRIVATE
# )

set_target_properties("${PROJECT_NAME}" PROPERTIES
    # ARCHIVE_OUTPUT_DIRECTORY "${FRAMEWORK_OUTPUT_DIRECTORY}" # Creates a Debug and Release folder in given directory
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${FRAMEWORK_OUTPUT_DIRECTORY}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FRAMEWORK_OUTPUT_DIRECTORY}"
    # OUTPUT_NAME "${PROJECT_NAME}${FRAMEWORK_CMAKE_OUTPUT_NAME_SUFFIX}" # Alternative way to name the output
    # DEBUG_POSTFIX "_d"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}_d"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}${PROJECT001_OUTPUT_NAME_GRAPHICS_SUFFIX}"
)

# ##############################################################################

if(${FRAMEWORK_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("FRAMEWORK_INCLUDE:" ${FRAMEWORK_INCLUDE})
    message("FRAMEWORK_OUTPUT_DIRECTORY:" ${FRAMEWORK_OUTPUT_DIRECTORY})
    message("FRAMEWORK_PROJECT_NAME:" ${FRAMEWORK_PROJECT_NAME})

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

    get_property(_ARCHIVE_OUTPUT_DIRECTORY_DEBUG TARGET "${PROJECT_NAME}" PROPERTY ARCHIVE_OUTPUT_DIRECTORY_DEBUG)
    message("ARCHIVE_OUTPUT_DIRECTORY_DEBUG: ${_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")

    get_property(_ARCHIVE_OUTPUT_DIRECTORY_RELEASE TARGET "${PROJECT_NAME}" PROPERTY ARCHIVE_OUTPUT_DIRECTORY_RELEASE)
    message("ARCHIVE_OUTPUT_DIRECTORY_RELEASE: ${_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}")

    get_property(_OUTPUT_NAME_DEBUG TARGET "${PROJECT_NAME}" PROPERTY OUTPUT_NAME_DEBUG)
    message("OUTPUT_NAME_DEBUG: ${_OUTPUT_NAME_DEBUG}")

    get_property(_OUTPUT_NAME_RELEASE TARGET "${PROJECT_NAME}" PROPERTY OUTPUT_NAME_RELEASE)
    message("OUTPUT_NAME_RELEASE: ${_OUTPUT_NAME_RELEASE}")

    message("--------------------------------------------------------------------------------")
endif()

# ##############################################################################