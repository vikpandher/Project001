@ECHO off

REM Initialize and update all submodules
git submodule update --init --recursive

REM The absolute root directory of your project
SET "PROJECT_ROOT=%~dp0"

REM Build the submodules
CALL "%PROJECT_ROOT%Scripts\Build_Vendors.bat"

REM Set the build directory
SET "BUILD_DIR=%PROJECT_ROOT%Build"

REM Run CMake with the desired settings
cmake ^
  -S "%PROJECT_ROOT%." ^
  -B "%BUILD_DIR%" ^
  -G "Visual Studio 18 2026" ^
  -A x64 ^
  -DPROJECT001_VULKAN_BUILD=FALSE
