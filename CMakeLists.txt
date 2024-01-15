if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

cmake_minimum_required(VERSION 3.22.0)

# https://github.com/vikpandher/FileToHeaderConverter.git
set(PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY "C:/Users/vikpa/Documents/Dev/FileToHeaderConverter/Bin/FileToHeaderConverter.exe" CACHE PATH "")
# https://github.com/freetype/freetype.git
set(PROJECT001_FREETYPE_INCLUDE "C:/Users/vikpa/Documents/Dev/freetype/include" CACHE PATH "")
set(PROJECT001_FREETYPE_LIBRARY "C:/Users/vikpa/Documents/Dev/freetype/build/Release/freetype.lib" CACHE PATH "")
# https://glad.dav1d.de/
set(PROJECT001_GLAD_INCLUDE "C:/Users/vikpa/Documents/Dev/glad/include" CACHE PATH "")
set(PROJECT001_GLAD_FILE "C:/Users/vikpa/Documents/Dev/glad/src/glad.c" CACHE PATH "")
# https://github.com/glfw/glfw.git
set(PROJECT001_GLFW_INCLUDE "C:/Users/vikpa/Documents/Dev/glfw/include" CACHE PATH "")
set(PROJECT001_GLFW_LIBRARY "C:/Users/vikpa/Documents/Dev/glfw/build/src/Release/glfw3.lib" CACHE PATH "")
# https://github.com/g-truc/glm.git
set(PROJECT001_GLM_INCLUDE "C:/Users/vikpa/Documents/Dev/glm" CACHE PATH "")
# https://github.com/mackron/miniaudio.git
set(PROJECT001_MINIAUDIO_INCLUDE "C:/Users/vikpa/Documents/Dev/miniaudio" CACHE PATH "")
# https://github.com/nothings/stb.git
set(PROJECT001_STB_INCLUDE "C:/Users/vikpa/Documents/Dev/stb" CACHE PATH "")
# https://vulkan.lunarg.com/
set(PROJECT001_VULKAN_DIRECTORY "C:/VulkanSDK/1.3.261.1" CACHE PATH "")
set(PROJECT001_VULKAN_INCLUDE "${PROJECT001_VULKAN_DIRECTORY}/Include")
set(PROJECT001_VULKAN_LIBRARY "${PROJECT001_VULKAN_DIRECTORY}/Lib/vulkan-1.lib")

set(PROJECT001_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/TestApplication/Binary")

option(PROJECT001_SANITY_CHECK_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

# If TRUE, sets renderer to Vulkan
# If FALSE, renderer is set to OpenGL
option(PROJECT001_VULKAN_BUILD "If TRUE, sets renderer to Vulkan. If FALSE, renderer is set to OpenGL" FALSE)

# If TRUE, removes the console on Windows builds
# If FALSE, Windows builds have a console
option(PROJECT001_WINDOWS_NO_CONSOLE "If TRUE, removes the console on Windows builds. If FALSE, Windows builds have a console." FALSE)

# If TRUE, adds memory leak check to Windows builds
# If FALSE, doesn't
option(PROJECT001_WINDOWS_MEMORY_LEAK_CHECK "If TRUE, adds memory leak check to Windows builds. If False, doesn't." TRUE)

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

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

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
    "Source/Components/CollisionData.h"
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
    "${PROJECT001_GLAD_FILE}"
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

set(TEST_APPLICATION_FILES
    "TestApplication/Source/Main.cpp"
    "TestApplication/Source/TestButtonData001.h"
    "TestApplication/Source/TestComponents.cpp"
    "TestApplication/Source/TestComponents.h"
    "TestApplication/Source/TestInstructionScene001.cpp"
    "TestApplication/Source/TestInstructionScene001.h"
    "TestApplication/Source/TestResource_AntonioRegular_ttf.h"
    "TestApplication/Source/TestResource_Bounce_wav.h"
    "TestApplication/Source/TestResource_Congratulations_ogg.h"
    "TestApplication/Source/TestResource_CounterclockwiseDie_png.h"
    "TestApplication/Source/TestResource_Cube_obj.h"
    "TestApplication/Source/TestScene001.cpp"
    "TestApplication/Source/TestScene001.h"
    "TestApplication/Source/TestScene002.cpp"
    "TestApplication/Source/TestScene002.h"
    "TestApplication/Source/TestScene003.cpp"
    "TestApplication/Source/TestScene003.h"
    "TestApplication/Source/TestScene004.cpp"
    "TestApplication/Source/TestScene004.h"
    "TestApplication/Source/TestScene006.cpp"
    "TestApplication/Source/TestScene006.h"
    "TestApplication/Source/TestScene010.cpp"
    "TestApplication/Source/TestScene010.h"
    "TestApplication/Source/TestScene011.cpp"
    "TestApplication/Source/TestScene011.h"
    "TestApplication/Source/TestScene012.cpp"
    "TestApplication/Source/TestScene012.h"
    "TestApplication/Source/TestScene013.cpp"
    "TestApplication/Source/TestScene013.h"
    "TestApplication/Source/TestScene030.cpp"
    "TestApplication/Source/TestScene030.h"
    "TestApplication/Source/TestScene031.cpp"
    "TestApplication/Source/TestScene031.h"
    "TestApplication/Source/TestScene032.cpp"
    "TestApplication/Source/TestScene032.h"
    "TestApplication/Source/TestScene033.cpp"
    "TestApplication/Source/TestScene033.h"
    "TestApplication/Source/TestScene034.cpp"
    "TestApplication/Source/TestScene034.h"
    "TestApplication/Source/TestScene050.cpp"
    "TestApplication/Source/TestScene050.h"
    "TestApplication/Source/TestScene051.cpp"
    "TestApplication/Source/TestScene051.h"
    "TestApplication/Source/TestSceneBase001.cpp"
    "TestApplication/Source/TestSceneBase001.h"
    "TestApplication/Source/TestSceneBase002.cpp"
    "TestApplication/Source/TestSceneBase002.h"
    "TestApplication/Source/TestSceneIds.h"
)

source_group("TestApplication/Source" FILES ${TEST_APPLICATION_FILES})

add_executable("${PROJECT_NAME}" ${CMAKE_WIN32}
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
    ${TEST_APPLICATION_FILES}
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

if (${PROJECT001_WINDOWS_NO_CONSOLE})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS_NO_CONSOLE")

    # adding WIN32 makes WinMain() the entry point and so no conole appears
    set(CMAKE_WIN32 WIN32)
endif()

if (${PROJECT001_WINDOWS_MEMORY_LEAK_CHECK})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS_MEMORY_LEAK_CHECK")
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
    "TestApplication"
)

# target_link_directories("${PROJECT_NAME}"
#     PRIVATE
# )

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    "${PROJECT001_FREETYPE_LIBRARY}"
    "${PROJECT001_GLFW_LIBRARY}"
    "${PROJECT001_VULKAN_LIBRARY}"
)

set_target_properties("${PROJECT_NAME}" PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT001_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT001_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT001_OUTPUT_DIRECTORY}"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}_d"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}${CMAKE_OUTPUT_NAME_SUFFIX}"
)

# ##############################################################################
# ##############################################################################

if(EXISTS "${PROJECT001_VULKAN_DIRECTORY}/Bin/glslc.exe")
    file(WRITE "Scripts/glslc_path.txt" "${PROJECT001_VULKAN_DIRECTORY}/Bin/glslc.exe")
endif()

if(EXISTS "${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY}")
    file(WRITE "Scripts/FileToHeaderConverter_path.txt" "${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY}")
endif()

if(${PROJECT001_SANITY_CHECK_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY: ${PROJECT001_FILE_TO_HEADER_CONVERTER_BINARY}")
    message("PROJECT001_FREETYPE_INCLUDE: ${PROJECT001_FREETYPE_INCLUDE}")
    message("PROJECT001_FREETYPE_LIBRARY: ${PROJECT001_FREETYPE_LIBRARY}")
    message("PROJECT001_GLAD_INCLUDE: ${PROJECT001_GLAD_INCLUDE}")
    message("PROJECT001_GLAD_FILE: ${PROJECT001_GLAD_FILE}")
    message("PROJECT001_GLFW_INCLUDE: ${PROJECT001_GLFW_INCLUDE}")
    message("PROJECT001_GLFW_LIBRARY: ${PROJECT001_GLFW_LIBRARY}")
    message("PROJECT001_GLM_INCLUDE: ${PROJECT001_GLM_INCLUDE}")
    message("PROJECT001_MINIAUDIO_INCLUDE: ${PROJECT001_MINIAUDIO_INCLUDE}")
    message("PROJECT001_STB_INCLUDE: ${PROJECT001_STB_INCLUDE}")
    message("PROJECT001_VULKAN_DIRECTORY: ${PROJECT001_VULKAN_DIRECTORY}")
    message("PROJECT001_VULKAN_INCLUDE: ${PROJECT001_VULKAN_INCLUDE}")
    message("PROJECT001_VULKAN_LIBRARY: ${PROJECT001_VULKAN_LIBRARY}")

    message("PROJECT001_OUTPUT_DIRECTORY: ${PROJECT001_OUTPUT_DIRECTORY}")

    message("PROJECT001_VULKAN_BUILD: ${PROJECT001_VULKAN_BUILD}")
    message("PROJECT001_WINDOWS_NO_CONSOLE: ${PROJECT001_WINDOWS_NO_CONSOLE}")
    message("PROJECT001_WINDOWS_MEMORY_LEAK_CHECK: ${PROJECT001_WINDOWS_MEMORY_LEAK_CHECK}")

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
