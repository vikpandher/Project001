@rem This script generates resource files.

@echo off
setlocal enabledelayedexpansion

set exe=..\..\Vendor\FileToHeaderConverter\Bin\FileToHeaderConverter.exe
set outputPath=Resources\

set files= ^
hazard_4x4.png ^
penguin01.png ^
penguin01_beak.obj ^
penguin01_body.obj ^
penguin01_flipper_left.obj ^
penguin01_flipper_right.obj ^
penguin01_foot_left.obj ^
penguin01_foot_right.obj ^
penguin01_glasses.obj ^
penguin01_glasses_v2.obj ^
penguin01_glasses_v3.obj ^
penguin01_head.obj ^
penguin01_head_v2.obj ^
penguin01_v2.png

for %%f in (%files%) do (
    set "input_file=%%f"
    set "newName=!input_file:.=_!"
    set "output_file=%outputPath%!newName!.h"
    set "array_name=g_!newName!"
    "%exe%" !input_file! !output_file! !array_name!
)