@REM This sript builds the third-party libraries.

@ECHO OFF

SET cmake_generator="Visual Studio 18 2026"
SET cmake_architecture=x64

ECHO.
ECHO Building FileToHeaderConverter
ECHO ^
--------------------------------------------------------------------------------

SET "FTH_DIR=%~dp0..\Vendor\FileToHeaderConverter"

cmake ^
    -S "%FTH_DIR%" ^
    -B "%FTH_DIR%/Build" ^
    -G %cmake_generator% ^
    -A %cmake_architecture%
cmake ^
    --build "%FTH_DIR%/Build" ^
    --config Release

ECHO.
ECHO Building glfw
ECHO ^
--------------------------------------------------------------------------------

SET "GLFW_DIR=%~dp0..\Vendor\glfw"

cmake ^
    -S "%GLFW_DIR%" ^
    -B "%GLFW_DIR%/build" ^
    -DGLFW_BUILD_DOCS=OFF ^
    -DGLFW_BUILD_EXAMPLES=OFF ^
    -DGLFW_BUILD_TESTS=OFF ^
    -DGLFW_INSTALL=OFF ^
    -G %cmake_generator% ^
    -A %cmake_architecture%
cmake ^
    --build "%GLFW_DIR%/build" ^
    --config Debug
cmake ^
    --build "%GLFW_DIR%/build" ^
    --config Release

ECHO ^
--------------------------------------------------------------------------------