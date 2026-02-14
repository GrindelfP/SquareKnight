#!/bin/bash

BUILD_DIR="build"
RUNABLE=0

# ADDITIONAL ARGS
for arg in "$@"; do
# CLEAN
    if [ "$arg" = "clean" ]; then
        echo "Performing clean build!"
        if [ -d "$BUILD_DIR" ]; then 
            rm -rf "$BUILD_DIR"
        fi
    fi
# RUN
    if [ "$arg" = "run" ]; then
        RUNABLE=1
    fi
done

# MKDIR IF DIR DOESN'T EXIST
mkdir -p "$BUILD_DIR"

# DIR CHANGE
cd "$BUILD_DIR"

# CMAKE PREPARING
cmake ..

# CMAKE BUILD
cmake --build .

if [ "$RUNABLE" -eq 1 ]; then
    cd ..
    ./run.sh
fi
