@REM This script calls the compile_shaders_in_dir_to_spv script to compile the
@REM Vulkan shaders into SPIR-V files. Then this script calls the 
@REM convert_spv_shaders_in_dir_to_headers script to turn all the SPIR-V files
@REM into header files.

@ECHO OFF

SETLOCAL

SET input_directory=../Source/Platform/Vulkan_Shaders
SET temp_directory=%input_directory%/Temp
SET output_directory=%input_directory%/Generated

SET input_directory=%input_directory:/=\%
SET temp_directory=%temp_directory:/=\%
SET output_directory=%output_directory:/=\%

IF EXIST glslc_path.txt (
    SET /p compiler=<glslc_path.txt
) ELSE (
    GOTO :EOF
)

IF EXIST FileToHeaderConverter_path.txt (
    SET /p converter=<FileToHeaderConverter_path.txt
) ELSE (
    GOTO :EOF
)

IF NOT EXIST "%input_directory%" (
    GOTO :EOF
)

IF NOT EXIST "%temp_directory%" (
    mkdir %temp_directory%
)

IF NOT EXIST "%output_directory%" (
    mkdir %output_directory%
)

CALL compile_shaders_in_dir_to_spv.bat %compiler% %input_directory% %temp_directory%

CALL convert_spv_shaders_in_dir_to_headers.bat %converter% %temp_directory% %output_directory%