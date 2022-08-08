# cmake_minimum_required(VERSION 3.17.0)
cmake_minimum_required(VERSION 3.14.5)

option(PRINT_DEBUG_OUTPUT "" TRUE)

# https://github.com/freetype/freetype.git
set(CMAKE_FREETYPE_REPO "C:/Users/vikpa/Documents/Dev/freetype")
# https://glad.dav1d.de/
set(CMAKE_GLAD_FILES "C:/Users/vikpa/Documents/Dev/glad")
# https://github.com/glfw/glfw.git
set(CMAKE_GLFW_REPO "C:/Users/vikpa/Documents/Dev/glfw")
# https://github.com/g-truc/glm.git
set(CMAKE_GLM_REPO "C:/Users/vikpa/Documents/Dev/glm")
# https://github.com/kcat/openal-soft
set(CMAKE_OPENAL_REPO "C:/Users/vikpa/Documents/Dev/openal-soft")
# https://github.com/nothings/stb.git
set(CMAKE_STB_REPO "C:/Users/vikpa/Documents/Dev/stb")

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
# add_compile_definitions("GLM_FORCE_LEFT_HANDED")

set(SOURCE_FILES
    "Src/Main.cpp"
    "Src/DeathFlag.cpp"
    "Src/DeathFlag.h"
    "Src/TestComponents.cpp"
    "Src/TestComponents.h"
    "Src/TestScene001.cpp"
    "Src/TestScene001.h"
    "Src/TestScene002.cpp"
    "Src/TestScene002.h"
    "Src/TestScene003.cpp"
    "Src/TestScene003.h"
    "Src/TestScene004.cpp"
    "Src/TestScene004.h"
    "Src/TestScene005.cpp"
    "Src/TestScene005.h"
    "Src/TestScene010.cpp"
    "Src/TestScene010.h"
    "Src/TestScene011.cpp"
    "Src/TestScene011.h"
    "Src/TestScene012.cpp"
    "Src/TestScene012.h"
    "Src/TestScene013.cpp"
    "Src/TestScene013.h"
    "Src/TestScene020.cpp"
    "Src/TestScene020.h"
    "Src/TestSceneBase001.cpp"
    "Src/TestSceneBase001.h"
    "Src/TestSceneBase002.cpp"
    "Src/TestSceneBase002.h"
    "Src/TestSceneData.cpp"
    "Src/TestSceneData.h"
)

source_group("Src" FILES ${SOURCE_FILES})

set(ENGINE_FILES
    "Src/Engine/Application.cpp"
    "Src/Engine/Application.h"
    "Src/Engine/BiMap.h"
    "Src/Engine/ComponentContainer.cpp"
    "Src/Engine/ComponentContainer.h"
    "Src/Engine/ComponentStores.cpp"
    "Src/Engine/ComponentStores.h"
    "Src/Engine/Event.h"
    "Src/Engine/FontData.h"
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
    "Src/Engine/Window.cpp"
    "Src/Engine/Window.h"
)

source_group("Src/Engine" FILES ${ENGINE_FILES})

set (COMPONENTS_FILES
    "Src/Engine/Components/Camera.h"
    "Src/Engine/Components/CollisionBody.cpp"
    "Src/Engine/Components/CollisionBody.h"
    "Src/Engine/Components/CollisionBody2D.cpp"
    "Src/Engine/Components/CollisionBody2D.h"
    "Src/Engine/Components/LightSource.h"
    "Src/Engine/Components/Placement.h"
    "Src/Engine/Components/Placement2D.h"
    "Src/Engine/Components/Position.h"
    "Src/Engine/Components/Position2D.h"
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
    "Src/Engine/Math/Shapes2D.h"
    "Src/Engine/Math/Shapes3D.h"
    "Src/Engine/Math/VectorUtilities.h"
    "Src/Engine/Math/VectorUtilities.inl"
)

source_group("Src/Engine/Math" FILES ${MATH_FILES})

set(PLATFORM_FILES
    "${CMAKE_GLAD_FILES}/src/glad.c"
    "Src/Engine/Platform/GLFWWindow.cpp"
    "Src/Engine/Platform/GLFWWindow.h"
    "Src/Engine/Platform/OpenALError.h"
    "Src/Engine/Platform/OpenALSoundPlayer.cpp"
    "Src/Engine/Platform/OpenALSoundPlayer.h"
    "Src/Engine/Platform/OpenGLError.h"
    "Src/Engine/Platform/OpenGLRenderer.cpp"
    "Src/Engine/Platform/OpenGLRenderer.h"
    "Src/Engine/Platform/OpenGLRendererAlt.cpp"
    "Src/Engine/Platform/OpenGLRendererAlt.h"
    "Src/Engine/Platform/OpenGLShader.cpp"
    "Src/Engine/Platform/OpenGLShader.h"
    "Src/Engine/Platform/OpenGLTexture.cpp"
    "Src/Engine/Platform/OpenGLTexture.h"
)

source_group("Src/Engine/Platform" FILES ${PLATFORM_FILES})

set(SHADER_SOURCE_FILES
    "Src/Engine/Platform/ShaderSource/BatchShaderSource.h"
    "Src/Engine/Platform/ShaderSource/NormalShaderSource.h"
    "Src/Engine/Platform/ShaderSource/ScreenShaderSource.h"
    "Src/Engine/Platform/ShaderSource/WireFrameShaderSource.h"
)

source_group("Src/Engine/Platform/ShaderSource" FILES ${SHADER_SOURCE_FILES})

# adding WIN32 makes WinMain() the entry point and so no conole appears
add_executable("${PROJECT_NAME}" # WIN32
    ${SOURCE_FILES}
    ${ENGINE_FILES}
    ${COMPONENTS_FILES}
    ${MATH_FILES}
    ${PLATFORM_FILES}
    ${SHADER_SOURCE_FILES}
)

target_include_directories("${PROJECT_NAME}"
    PUBLIC 
    "${CMAKE_FREETYPE_REPO}/include"
    "${CMAKE_GLAD_FILES}/include"
    "${CMAKE_GLFW_REPO}/include"
    "${CMAKE_GLM_REPO}"
    "${CMAKE_OPENAL_REPO}/include"
    "${CMAKE_STB_REPO}"
    "Src"
)

target_link_directories("${PROJECT_NAME}"
    PUBLIC
)

# Getting a linker warning when building debug only:
    # LNK4098 defaultlib 'MSVCRT' confilics with use of other libs; use
    # /NODEFAULTLIB:library

target_link_libraries("${PROJECT_NAME}"
    "${CMAKE_FREETYPE_REPO}/objs/x64/Release/freetype.lib"
    "${CMAKE_GLFW_REPO}/build/src/Release/glfw3.lib"
    "${CMAKE_OPENAL_REPO}/build/Release/OpenAL32.lib"
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

    message("CMAKE_FREETYPE_REPO: ${CMAKE_FREETYPE_REPO}")
    message("CMAKE_GLAD_FILES: ${CMAKE_GLAD_FILES}")
    message("CMAKE_GLFW_REPO: ${CMAKE_GLFW_REPO}")
    message("CMAKE_GLM_REPO: ${CMAKE_GLM_REPO}")
    message("CMAKE_OPENAL_REPO: ${CMAKE_OPENAL_REPO}")
    message("CMAKE_STB_REPO: ${CMAKE_STB_REPO}")

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
