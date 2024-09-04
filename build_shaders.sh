#!/bin/bash

SHADER_DIR="shaders"
OUTPUT_DIR="shaders/compiled"

# Clean output directory if it does exist
rm -rf "$OUTPUT_DIR"
# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Find all .comp files and compile them
for shader in "${SHADER_DIR}"/*; do
    if [ -d "$shader" ]; then continue; fi
    shader_name=$(basename "$shader")
    echo "Compiling ${shader}..."
    glslang -V "${shader}" -o "$OUTPUT_DIR/${shader_name%.*}.spv"
done

echo "Shader compilation completed."
