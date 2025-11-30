@REM This sript builds the third-party libraries.

@ECHO OFF

SET cmake_generator="Visual Studio 17 2022"
SET cmake_architecture=x64
SET msbuild_path=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\Msbuild.exe

IF NOT EXIST "%msbuild_path%" (
    ECHO Microsoft Build Engine not found: %msbuild_path%
    GOTO :EOF
)

SET originalDir=%cd%

ECHO.
ECHO Building FileToHeaderConverter
ECHO ^
--------------------------------------------------------------------------------

CD /D %~dp0\..\Vendor\FileToHeaderConverter

cmake ^
    -B Build ^
    -G %cmake_generator% ^
    -A %cmake_architecture%

CD Build

CALL %msbuild_path% FileToHeaderConverter.sln /p:Configuration=Release /p:Platform=x64

ECHO.
ECHO Building glfw
ECHO ^
--------------------------------------------------------------------------------

CD %~dp0\..\Vendor\glfw

cmake ^
    -DGLFW_BUILD_DOCS=OFF ^
    -DGLFW_BUILD_EXAMPLES=OFF ^
    -DGLFW_BUILD_TESTS=OFF ^
    -DGLFW_INSTALL=OFF ^
    -B build ^
    -G %cmake_generator% ^
    -A %cmake_architecture%

CD build

CALL %msbuild_path% GLFW.sln /p:Configuration=Debug /p:Platform=x64

CALL %msbuild_path% GLFW.sln /p:Configuration=Release /p:Platform=x64

ECHO ^
--------------------------------------------------------------------------------

CD %originalDir%