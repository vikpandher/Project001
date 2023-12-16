@REM This script converts a binary file into a c header with an array of bytes.
@REM
@REM First parameter is the input file name
@REM
@REM Second parameter is the output file name
@REM
@REM Third parameter is the array variable name
@REM
@REM This script is pretty slow.

@ECHO OFF

IF "%~3"=="" (
    ECHO ERROR: not enough aguments
    GOTO :EOF
)

SET input_file=%~1
SET output_file=%~2
SET array_name=%~3

ECHO input: %input_file%
ECHO %TIME%

@REM Convert the binary file into a hexadecimal representation
@REM ---------------------------------------------------------------------------

SET tmp_file=%output_file%.tmp

certutil -encodehex %input_file% %tmp_file% > NUL

@REM Use the values in the hexadecimal file to generate header file
@REM ---------------------------------------------------------------------------

ECHO #pragma once > %output_file%
ECHO const unsigned char %array_name%[] = { >> %output_file%

SETLOCAL ENABLEDELAYEDEXPANSION

SET hexValues=
SET lastToken=
SET counter=0
FOR /F "TOKENS=*" %%A IN (
    %tmp_file%
) DO (
    SET line=%%A
    SET line=!line:~5,48!
    FOR %%B IN (
        !line!
    ) DO (
        SET hexValues=!hexValues! %%B
    )

    FOR %%C IN (!hexValues!) DO (
        IF DEFINED lastToken (
            IF !counter! EQU 8 (
                ECHO !lastToken!, >> %output_file%
                SET counter=0
            ) ELSE (
                <NUL SET /p =!lastToken!, >> %output_file%
            )
        )
        SET lastToken=0x%%~C
        SET /A counter+=1
    )
    SET hexValues=
)

IF DEFINED lastToken (
    ECHO !lastToken! >> %output_file%
)

ENDLOCAL

ECHO }; >> %output_file%

@REM Clean up temporary files
@REM DEL %tmp_file%

ECHO output: %output_file%
ECHO %TIME%