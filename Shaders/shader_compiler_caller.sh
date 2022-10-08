#!/bin/bash

# This script calls the glslc compiler to individually compile
# each shader file in the current directory.
# 
# Supported file extensions are as follows:
# 
# .vert - a vertex shader
# .tesc - a tessellation control shader
# .tese - a tessellation evaluation shader
# .geom - a geometry shader
# .frag - a fragment shader
# .comp - a compute shader
# 
# NOTE THIS SCRIPT DOESN'T WORK IF THE PATH IS GIVEN WITH BACK SLASHES

if [ "$#" -eq 0 ]; then
    echo "ERROR: provide glslc path as parameter"
    exit
fi

if [ "$#" -eq 1 ]; then
    echo "ERROR: provide shader directory"
    exit
fi

if [ "$#" -gt 2 ]; then
    echo "ERROR: too many parameters"
    exit
fi

if [ ! -e $1 ]; then
    echo "ERROR: glslc doesn't exist"
    exit
fi

if [ ! -e $d ]; then
    echo "ERROR: shader directory doesn't exist"
    exit
fi

shopt -s nullglob
for f in $2/*.vert $2/*.tesc $2/*.tese $2/*.geom $2/*.frag $2/*.comp
do
    $1 "$f" "-o" "${f%.*}_${f##*.}.spv"
done
