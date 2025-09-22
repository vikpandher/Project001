@ECHO off

REM Initialize and update all submodules
git submodule update --init --recursive

REM Build the submodules
CALL Scripts\Build_Vendors.bat

CD %~dp0

REM Set the build directory
SET BUILD_DIR=Build

REM Run CMake with the desired settings
cmake -S . -B %BUILD_DIR% -G "Visual Studio 17 2022" -DPROJECT001_VULKAN_BUILD=FALSE
