@ECHO OFF

@REM This script calls the glslc compiler to individually compile
@REM each shader file in the current directory.
@REM 
@REM Supported file extensions are as follows:
@REM
@REM .vert - a vertex shader
@REM .tesc - a tessellation control shader
@REM .tese - a tessellation evaluation shader
@REM .geom - a geometry shader
@REM .frag - a fragment shader
@REM .comp - a compute shader
@REM
@REM First Parameter is the compiler (it's sort of optional)
@REM Second parameters is the shader directory (it's optional)

SET e=%1

IF "%1"=="" (
@REM ECHO ERROR: provide glslc path as parameter
@REM GOTO :EOF
set /p e=<glslc_path.txt
)

SET d=%2

IF "%2"=="" (
@REM ECHO ERROR: provide shader directory
@REM GOTO :EOF
SET d=.
)

IF NOT "%3"=="" (
ECHO ERROR: too many parameters
GOTO :EOF
)

IF NOT EXIST %e% (
ECHO ERROR: glslc doesn't exist
GOTO :EOF
)

IF NOT EXIST %d%\NUL (
ECHO ERROR: shader directory doesn't exist
GOTO :EOF
)

PUSHD %2

SETLOCAL ENABLEDELAYEDEXPANSION
FOR /f %%G IN ('DIR *.vert *.tesc *.tese *.geom *.frag *.comp /b') DO (
SET x=%%~xG
%e% %%G -o %%~nG_!x:~1!.spv
ECHO input:  %%G
ECHO output: %%~nG_!x:~1!.spv
)

POPD

PAUSE