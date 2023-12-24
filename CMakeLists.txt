if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

cmake_minimum_required(VERSION 3.22.0)

# https://github.com/freetype/freetype.git
set(CMAKE_FREETYPE_INCLUDE "C:/Users/vikpa/Documents/Dev/freetype/include")
set(CMAKE_FREETYPE_LIBRARY "C:/Users/vikpa/Documents/Dev/freetype/build/Release/freetype.lib")
# https://glad.dav1d.de/
set(CMAKE_GLAD_INCLUDE "C:/Users/vikpa/Documents/Dev/glad/include")
set(CMAKE_GLAD_FILE "C:/Users/vikpa/Documents/Dev/glad/src/glad.c")
# https://github.com/glfw/glfw.git
set(CMAKE_GLFW_INCLUDE "C:/Users/vikpa/Documents/Dev/glfw/include")
set(CMAKE_GLFW_LIBRARY "C:/Users/vikpa/Documents/Dev/glfw/build/src/Release/glfw3.lib")
# https://github.com/g-truc/glm.git
set(CMAKE_GLM_INCLUDE "C:/Users/vikpa/Documents/Dev/glm")
# https://github.com/mackron/miniaudio.git
set(CMAKE_MINIAUDIO_INCLUDE "C:/Users/vikpa/Documents/Dev/miniaudio")
# https://github.com/nothings/stb.git
set(CMAKE_STB_INCLUDE "C:/Users/vikpa/Documents/Dev/stb")
# https://vulkan.lunarg.com/
set(CMAKE_VULKAN_REPO "C:/VulkanSDK/1.3.261.1")
set(CMAKE_VULKAN_INCLUDE "${CMAKE_VULKAN_REPO}/Include")
set(CMAKE_VULKAN_LIBRARY "${CMAKE_VULKAN_REPO}/Lib/vulkan-1.lib")

set(CMAKE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Bin")

option(CMAKE_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# If TRUE, sets renderer to Vulkan
# If FALSE, renderer is set to OpenGL
set(CMAKE_VULKAN_MODE FALSE)

# If TRUE, removes the console on a windows build
# If FALSE, windows build has a console
set(CMAKE_WINDOWS_NO_CONSOLE FALSE)

# If TRUE, adds memory leak check to windows build
# If FALSE, doesn't
set(CMAKE_WINDOWS_MEMORY_LEAK_CHECK TRUE)

# ##############################################################################
# ##############################################################################

project("Project001" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION "This is project 001."
    LANGUAGES C CXX
)

# at least C++11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

# add_compile_definitions("GLM_ENABLE_EXPERIMENTAL")

if (${CMAKE_VULKAN_MODE})
    add_definitions(-DVULKAN_MODE)
    add_definitions(-DGLFW_INCLUDE_VULKAN)
    add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)
    # add_definitions(-DGLM_FORCE_LEFT_HANDED)

    set(CMAKE_OUTPUT_NAME_SUFFIX "_v")
else()
    set(CMAKE_OUTPUT_NAME_SUFFIX "_o")
endif()

if (${CMAKE_WINDOWS_NO_CONSOLE})
    add_definitions(-DWINDOWS_NO_CONSOLE)

    # adding WIN32 makes WinMain() the entry point and so no conole appears
    set(CMAKE_WIN32 WIN32)
endif()

if (${CMAKE_WINDOWS_MEMORY_LEAK_CHECK})
    add_definitions(-DWINDOWS_MEMORY_LEAK_CHECK)
endif()

set(SOURCE_FILES
    "Src/Main.cpp"
    "Src/TestButtonData001.h"
    "Src/TestComponents.cpp"
    "Src/TestComponents.h"
    "Src/TestInstructionScene001.cpp"
    "Src/TestInstructionScene001.h"
    "Src/TestResource_AntonioRegular_ttf.h"
    "Src/TestResource_Bounce_wav.h"
    "Src/TestResource_Congratulations_ogg.h"
    "Src/TestResource_CounterclockwiseDie_png.h"
    "Src/TestResource_Cube_obj.h"
    "Src/TestScene001.cpp"
    "Src/TestScene001.h"
    "Src/TestScene002.cpp"
    "Src/TestScene002.h"
    "Src/TestScene003.cpp"
    "Src/TestScene003.h"
    "Src/TestScene004.cpp"
    "Src/TestScene004.h"
    "Src/TestScene006.cpp"
    "Src/TestScene006.h"
    "Src/TestScene010.cpp"
    "Src/TestScene010.h"
    "Src/TestScene011.cpp"
    "Src/TestScene011.h"
    "Src/TestScene012.cpp"
    "Src/TestScene012.h"
    "Src/TestScene013.cpp"
    "Src/TestScene013.h"
    "Src/TestScene030.cpp"
    "Src/TestScene030.h"
    "Src/TestScene031.cpp"
    "Src/TestScene031.h"
    "Src/TestScene032.cpp"
    "Src/TestScene032.h"
    "Src/TestScene033.cpp"
    "Src/TestScene033.h"
    "Src/TestScene034.cpp"
    "Src/TestScene034.h"
    "Src/TestScene050.cpp"
    "Src/TestScene050.h"
    "Src/TestScene051.cpp"
    "Src/TestScene051.h"
    "Src/TestSceneBase001.cpp"
    "Src/TestSceneBase001.h"
    "Src/TestSceneBase002.cpp"
    "Src/TestSceneBase002.h"
    "Src/TestSceneIds.h"
)

source_group("Src" FILES ${SOURCE_FILES})

set(ENGINE_FILES
    "Src/Engine/Application.cpp"
    "Src/Engine/Application.h"
    "Src/Engine/AutoIdMap.h"
    "Src/Engine/CollisionSystem2D.cpp"
    "Src/Engine/CollisionSystem2D.h"
    "Src/Engine/ComponentContainer.cpp"
    "Src/Engine/ComponentContainer.h"
    "Src/Engine/ComponentStores.cpp"
    "Src/Engine/ComponentStores.h"
    "Src/Engine/Event.h"
    "Src/Engine/FontData.h"
    "Src/Engine/FontMeshData.h"
    "Src/Engine/FreetypeTextLoader.cpp"
    "Src/Engine/FreetypeTextLoader.h"
    "Src/Engine/Logger.cpp"
    "Src/Engine/Logger.h"
    "Src/Engine/MeshData.h"
    "Src/Engine/MeshLoader.cpp"
    "Src/Engine/MeshLoader.h"
    "Src/Engine/MeshVertex.h"
    "Src/Engine/RenderData.h"
    "Src/Engine/Renderer.cpp"
    "Src/Engine/Renderer.h"
    "Src/Engine/RenderSystem.cpp"
    "Src/Engine/RenderSystem.h"
    "Src/Engine/ResourceStores.cpp"
    "Src/Engine/ResourceStores.h"
    "Src/Engine/Scene.cpp"
    "Src/Engine/Scene.h"
    "Src/Engine/SoundData.h"
    "Src/Engine/SoundLoader.cpp"
    "Src/Engine/SoundLoader.h"
    "Src/Engine/SoundPlayer.cpp"
    "Src/Engine/SoundPlayer.h"
    "Src/Engine/TextureData.h"
    "Src/Engine/TextureLoader.cpp"
    "Src/Engine/TextureLoader.h"
    "Src/Engine/UniqueBiMap.h"
    "Src/Engine/UniqueIdGenerator.cpp"
    "Src/Engine/UniqueIdGenerator.h"
    "Src/Engine/Window.cpp"
    "Src/Engine/Window.h"
)

source_group("Src/Engine" FILES ${ENGINE_FILES})

set (COMPONENTS_FILES
    "Src/Engine/Components/Camera.cpp"
    "Src/Engine/Components/Camera.h"
    "Src/Engine/Components/CollisionBody2D.cpp"
    "Src/Engine/Components/CollisionBody2D.h"
    "Src/Engine/Components/CollisionShape2D.h"
    "Src/Engine/Components/CollisionData.h"
    "Src/Engine/Components/LightSource.h"
    "Src/Engine/Components/Placement.h"
    "Src/Engine/Components/Placement2D.h"
    "Src/Engine/Components/Position.h"
    "Src/Engine/Components/Position2D.h"
    "Src/Engine/Components/RenderedMesh.h"
    "Src/Engine/Components/RenderedModel.cpp"
    "Src/Engine/Components/RenderedModel.h"
)

source_group("Src/Engine/Components" FILES ${COMPONENTS_FILES})

set (MATH_FILES
    "Src/Engine/Math/CoordinateSystems.h"
    "Src/Engine/Math/CoordinateSystems.inl"
    "Src/Engine/Math/FloatComparators.h"
    "Src/Engine/Math/FloatComparators.inl"
    "Src/Engine/Math/Overlap2D.h"
    "Src/Engine/Math/Overlap2D.inl"
    "Src/Engine/Math/Overlap3D.h"
    "Src/Engine/Math/Overlap3D.inl"
    "Src/Engine/Math/Overlap3D_UnitTests.h"
    "Src/Engine/Math/Overlap3D_UnitTests.inl"
    "Src/Engine/Math/VectorUtilities.h"
    "Src/Engine/Math/VectorUtilities.inl"
)

source_group("Src/Engine/Math" FILES ${MATH_FILES})

set(PLATFORM_GLFW_FILES
    "Src/Engine/Platform/GLFW_Window.cpp"
    "Src/Engine/Platform/GLFW_Window.h"
)

set(PLATFORM_MINIAUDIO_FILES
    "Src/Engine/Platform/MiniAudio_SoundPlayer.cpp"
    "Src/Engine/Platform/MiniAudio_SoundPlayer.h"
)

set(PLATFORM_OPENGL_FILES
    "${CMAKE_GLAD_FILE}"
    "Src/Engine/Platform/OpenGL_Error.h"
    "Src/Engine/Platform/OpenGL_Mesh.cpp"
    "Src/Engine/Platform/OpenGL_Mesh.h"
    "Src/Engine/Platform/OpenGL_Renderer.cpp"
    "Src/Engine/Platform/OpenGL_Renderer.h"
    "Src/Engine/Platform/OpenGL_Shader.cpp"
    "Src/Engine/Platform/OpenGL_Shader.h"
    "Src/Engine/Platform/OpenGL_Texture.cpp"
    "Src/Engine/Platform/OpenGL_Texture.h"
)

set(PLATFORM_VULKAN_FILES
    "Src/Engine/Platform/Vulkan_Error.h"
    "Src/Engine/Platform/Vulkan_Renderer.cpp"
    "Src/Engine/Platform/Vulkan_Renderer.h"
)

source_group("Src/Engine/Platform" FILES
    ${PLATFORM_GLFW_FILES}
    ${PLATFORM_MINIAUDIO_FILES}
    ${PLATFORM_OPENGL_FILES}
    ${PLATFORM_VULKAN_FILES}
)

set(PLATFORM_SHADER_SOURCE_FILES
    "Src/Engine/Platform/ShaderSource/GridShaderSource.h"
    "Src/Engine/Platform/ShaderSource/NormalShaderSource.h"
    "Src/Engine/Platform/ShaderSource/PrimaryShaderSource.h"
    "Src/Engine/Platform/ShaderSource/ScreenShaderSource.h"
    "Src/Engine/Platform/ShaderSource/WireFrameShaderSource.h"
)

source_group("Src/Engine/Platform/ShaderSource" FILES ${PLATFORM_SHADER_SOURCE_FILES})

# The following makes it so the unused graphics platform files doesn't get
# compiled, but the files are still listed in the visual studio project
if(${CMAKE_VULKAN_MODE})
    set_source_files_properties(${PLATFORM_OPENGL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set_source_files_properties(${PLATFORM_VULKAN_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

add_executable("${PROJECT_NAME}" ${CMAKE_WIN32}
    ${SOURCE_FILES}
    ${ENGINE_FILES}
    ${COMPONENTS_FILES}
    ${MATH_FILES}
    ${PLATFORM_GLFW_FILES}
    ${PLATFORM_MINIAUDIO_FILES}
    ${PLATFORM_OPENGL_FILES}
    ${PLATFORM_VULKAN_FILES}
    ${PLATFORM_SHADER_SOURCE_FILES}
)

target_include_directories("${PROJECT_NAME}"
    PRIVATE 
    "${CMAKE_FREETYPE_INCLUDE}"
    "${CMAKE_GLAD_INCLUDE}"
    "${CMAKE_GLFW_INCLUDE}"
    "${CMAKE_GLM_INCLUDE}"
    "${CMAKE_MINIAUDIO_INCLUDE}"
    "${CMAKE_STB_INCLUDE}"
    "${CMAKE_VULKAN_INCLUDE}"
    "Src"
)

target_link_directories("${PROJECT_NAME}"
    PRIVATE
)

# Getting a linker warning when building debug only:
    # LNK4098 defaultlib 'MSVCRT' confilics with use of other libs; use
    # /NODEFAULTLIB:library

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    "${CMAKE_FREETYPE_LIBRARY}"
    "${CMAKE_GLFW_LIBRARY}"
    "${CMAKE_VULKAN_LIBRARY}"
)

set_target_properties("${PROJECT_NAME}" PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_OUTPUT_DIR}"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}_d"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}"
)

if(EXISTS "${CMAKE_VULKAN_REPO}/Bin/glslc.exe")
    file(WRITE "Scripts/glslc_path.txt" "${CMAKE_VULKAN_REPO}/Bin/glslc.exe")

    # NOTE: Adding commands will require me to run Visual Studio as Admin

    # add_custom_command(TARGET "${PROJECT_NAME}"
    #                    POST_BUILD
    #                    COMMAND cmd /c "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/test.bat ${CMAKE_VULKAN_REPO}/Bin/glslc.exe ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/")
endif()

if(${CMAKE_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("CMAKE_FREETYPE_INCLUDE: ${CMAKE_FREETYPE_INCLUDE}")
    message("CMAKE_FREETYPE_LIBRARY: ${CMAKE_FREETYPE_LIBRARY}")
    message("CMAKE_GLAD_INCLUDE: ${CMAKE_GLAD_INCLUDE}")
    message("CMAKE_GLAD_FILE: ${CMAKE_GLAD_FILE}")
    message("CMAKE_GLFW_INCLUDE: ${CMAKE_GLFW_INCLUDE}")
    message("CMAKE_GLFW_LIBRARY: ${CMAKE_GLFW_LIBRARY}")
    message("CMAKE_GLM_INCLUDE: ${CMAKE_GLM_INCLUDE}")
    message("CMAKE_MINIAUDIO_INCLUDE: ${CMAKE_MINIAUDIO_INCLUDE}")
    message("CMAKE_STB_INCLUDE: ${CMAKE_STB_INCLUDE}")
    message("CMAKE_VULKAN_REPO: ${CMAKE_VULKAN_REPO}")
    message("CMAKE_VULKAN_INCLUDE: ${CMAKE_VULKAN_INCLUDE}")
    message("CMAKE_VULKAN_LIBRARY: ${CMAKE_VULKAN_LIBRARY}")

    message("CMAKE_OUTPUT_DIR: ${CMAKE_OUTPUT_DIR}")

    message("CMAKE_VULKAN_MODE: ${CMAKE_VULKAN_MODE}")
    message("CMAKE_WINDOWS_NO_CONSOLE: ${CMAKE_WINDOWS_NO_CONSOLE}")
    message("CMAKE_WINDOWS_MEMORY_LEAK_CHECK: ${CMAKE_WINDOWS_MEMORY_LEAK_CHECK}")

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

    message("CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

    message("CMAKE_CONFIGURATION_TYPES:")
    foreach(VARIABLE ${CMAKE_CONFIGURATION_TYPES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_USE_FOLDERS GLOBAL PROPERTY USE_FOLDERS)
    message("USE_FOLDERS: ${_USE_FOLDERS}")

    get_property(_VS_STARTUP_PROJECT DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT)
    message("VS_STARTUP_PROJECT: ${_VS_STARTUP_PROJECT}")

    get_property(_COMPILE_DEFINITIONS DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY COMPILE_DEFINITIONS)
    message("COMPILE_DEFINITIONS:")
    foreach(VARIABLE ${_COMPILE_DEFINITIONS})
        message("    ${VARIABLE}")
    endforeach()

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
