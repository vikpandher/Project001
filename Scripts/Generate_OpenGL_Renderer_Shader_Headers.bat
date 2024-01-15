@REM This script calls the convert_txt_shaders_in_dir_to_headers script to
@REM turn all the OpenGL shaders into header files.

@ECHO OFF

SET input_directory=../Source/Platform/OpenGL_Shaders
SET output_directory=%input_directory%/Generated

SET input_directory=%input_directory:/=\%
SET output_directory=%output_directory:/=\%

IF EXIST FileToHeaderConverter_path.txt (
    SET /p converter=<FileToHeaderConverter_path.txt
) ELSE (
    GOTO :EOF
)

IF NOT EXIST "%input_directory%" (
    GOTO :EOF
)

IF NOT EXIST "%output_directory%" (
    mkdir %output_directory%
)

CALL convert_txt_shaders_in_dir_to_headers.bat %converter% %input_directory% %output_directory%