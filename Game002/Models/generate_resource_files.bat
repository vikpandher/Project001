@rem This script generates resource files.

@echo off
setlocal enabledelayedexpansion

set exe=..\..\Vendor\FileToHeaderConverter\Bin\FileToHeaderConverter.exe
set outputPath=..\Source\Resources\

set files= ^
dotted_1_3.png ^
hazard_4x4.png ^
penguin_beak.obj ^
penguin_body.obj ^
penguin_eyes.obj ^
penguin_eyes_v2.obj ^
penguin_flipper_left.obj ^
penguin_flipper_right.obj ^
penguin_foot_left.obj ^
penguin_foot_right.obj ^
penguin_glasses.obj ^
penguin_glasses_v2.obj ^
penguin_glasses_v3.obj ^
penguin_head.obj ^
penguin_texture.png ^
penguin_texture_v2.png ^
penguin_texture_v3.png ^
penguin_texture_v4.png

for %%f in (%files%) do (
    set "input_file=%%f"
    set "newName=!input_file:.=_!"
    set "output_file=%outputPath%!newName!.h"
    set "array_name=g_!newName!"
    "%exe%" !input_file! !output_file! !array_name!
)