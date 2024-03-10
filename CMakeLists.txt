if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

cmake_minimum_required(VERSION 3.22.0)

# https://github.com/vikpandher/FileToHeaderConverter.git
set(PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/FileToHeaderConverter/Bin/FileToHeaderConverter.exe" CACHE PATH "")
# https://github.com/freetype/freetype.git
set(PROJECT001_FREETYPE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/freetype/include" CACHE PATH "")
# https://glad.dav1d.de/
set(PROJECT001_GLAD_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/include" CACHE PATH "")
set(PROJECT001_GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/src" CACHE PATH "")
# https://github.com/glfw/glfw.git
set(PROJECT001_GLFW_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw/include" CACHE PATH "")
# https://github.com/g-truc/glm.git
set(PROJECT001_GLM_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glm" CACHE PATH "")
# https://github.com/mackron/miniaudio.git
set(PROJECT001_MINIAUDIO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/miniaudio" CACHE PATH "")
# https://github.com/nothings/stb.git
set(PROJECT001_STB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb" CACHE PATH "")
# https://vulkan.lunarg.com/
set(PROJECT001_VULKAN_DIRECTORY "C:/VulkanSDK/1.3.261.1" CACHE PATH "")
set(PROJECT001_VULKAN_INCLUDE "${PROJECT001_VULKAN_DIRECTORY}/Include")

set(PROJECT001_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Library")

option(PROJECT001_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# If TRUE, sets renderer to Vulkan
# If FALSE, renderer is set to OpenGL
option(PROJECT001_VULKAN_BUILD "If TRUE, sets renderer to Vulkan. If FALSE, renderer is set to OpenGL" FALSE)

# ##############################################################################
# ##############################################################################

project("Project001" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION "This is project 001."
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
    "Source/CollisionSystem2D.cpp"
    "Source/CollisionSystem2D.h"
    "Source/ComponentContainer.cpp"
    "Source/ComponentContainer.h"
    "Source/ComponentStores.cpp"
    "Source/ComponentStores.h"
    "Source/Event.h"
    "Source/FontData.h"
    "Source/FontMeshData.h"
    "Source/FreetypeTextLoader.cpp"
    "Source/FreetypeTextLoader.h"
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
    "Source/Math/CoordinateSystems.h"
    "Source/Math/CoordinateSystems.inl"
    "Source/Math/FloatComparators.h"
    "Source/Math/FloatComparators.inl"
    "Source/Math/Overlap2D.h"
    "Source/Math/Overlap2D.inl"
    "Source/Math/Overlap3D.h"
    "Source/Math/Overlap3D.inl"
    "Source/Math/Overlap3D_UnitTests.h"
    "Source/Math/Overlap3D_UnitTests.inl"
    "Source/Math/VectorUtilities.h"
    "Source/Math/VectorUtilities.inl"
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
    "${PROJECT001_GLAD_SOURCE}/glad.c"
    "Source/Platform/OpenGL_Error.h"
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

# The following makes it so the unused graphics platform files doesn't get
# compiled, but the files are still listed in the visual studio project
if(${PROJECT001_VULKAN_BUILD})
    set_source_files_properties(${PLATFORM_OPENGL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set_source_files_properties(${PLATFORM_VULKAN_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

add_library("${PROJECT_NAME}" STATIC
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
)

# target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")

if (${PROJECT001_VULKAN_BUILD})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "VULKAN_BUILD")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_VULKAN")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_DEPTH_ZERO_TO_ONE")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_FORCE_LEFT_HANDED"") # flipped the viewport instead

    set(CMAKE_OUTPUT_NAME_SUFFIX "_v")
else()
    set(CMAKE_OUTPUT_NAME_SUFFIX "_o")
endif()

target_include_directories("${PROJECT_NAME}"
    PRIVATE 
    "${PROJECT001_FREETYPE_INCLUDE}"
    "${PROJECT001_GLAD_INCLUDE}"
    "${PROJECT001_GLFW_INCLUDE}"
    "${PROJECT001_GLM_INCLUDE}"
    "${PROJECT001_MINIAUDIO_INCLUDE}"
    "${PROJECT001_STB_INCLUDE}"
    "${PROJECT001_VULKAN_INCLUDE}"
    "Source"
)

# target_link_directories("${PROJECT_NAME}"
#     PRIVATE
# )

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
)

set_target_properties("${PROJECT_NAME}" PROPERTIES
    # ARCHIVE_OUTPUT_DIRECTORY "${PROJECT001_OUTPUT_DIRECTORY}" # Creates a Debug and Release folder in given directory
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT001_OUTPUT_DIRECTORY}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT001_OUTPUT_DIRECTORY}"
    # OUTPUT_NAME "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}" # Alternative way to name the output
    # DEBUG_POSTFIX "_d"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}_d"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}"
)

if(EXISTS "${PROJECT001_VULKAN_DIRECTORY}/Bin/glslc.exe")
    file(WRITE "Scripts/glslc_path.txt" "${PROJECT001_VULKAN_DIRECTORY}/Bin/glslc.exe")
else()
    message("${PROJECT001_VULKAN_DIRECTORY}/Bin/glslc.exe NOT FOUND")
endif()

if(EXISTS "${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY}")
    file(WRITE "Scripts/FileToHeaderConverter_path.txt" "${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY}")
else()
    message("${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY} NOT FOUND")
endif()

# ##############################################################################
# ##############################################################################

if(${PROJECT001_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY: ${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY}")
    message("PROJECT001_FREETYPE_INCLUDE: ${PROJECT001_FREETYPE_INCLUDE}")
    message("PROJECT001_GLAD_INCLUDE: ${PROJECT001_GLAD_INCLUDE}")
    message("PROJECT001_GLAD_SOURCE: ${PROJECT001_GLAD_SOURCE}")
    message("PROJECT001_GLFW_INCLUDE: ${PROJECT001_GLFW_INCLUDE}")
    message("PROJECT001_GLM_INCLUDE: ${PROJECT001_GLM_INCLUDE}")
    message("PROJECT001_MINIAUDIO_INCLUDE: ${PROJECT001_MINIAUDIO_INCLUDE}")
    message("PROJECT001_STB_INCLUDE: ${PROJECT001_STB_INCLUDE}")
    message("PROJECT001_VULKAN_DIRECTORY: ${PROJECT001_VULKAN_DIRECTORY}")
    message("PROJECT001_VULKAN_INCLUDE: ${PROJECT001_VULKAN_INCLUDE}")

    message("PROJECT001_OUTPUT_DIRECTORY: ${PROJECT001_OUTPUT_DIRECTORY}")

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
# ##############################################################################

add_subdirectory(TestApplication)
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "TestApplication")