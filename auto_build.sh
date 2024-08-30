#!/bin/bash

# Define the build directory
BUILD_DIR=build

# Remove any existing build files
rm -rf ${BUILD_DIR}

# Run cmake and build
cmake -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
cmake --build ${BUILD_DIR} --config Debug -j 16

# Run tests
cd ${BUILD_DIR}
ctest --rerun-failed --output-on-failure
