#!/bin/bash

SHADER_DIR="shaders"
OUTPUT_DIR="${SHADER_DIR}/compiled"

# Clean output directory if it exists
rm -rf "$OUTPUT_DIR"
# Create output directory
mkdir -p "$OUTPUT_DIR"

# Compile each shader
for shader in "${SHADER_DIR}"/*; do
    # Skip directories
    if [ -d "$shader" ]; then continue; fi
    
    # Extract the file name, omitting the file path
    shader_name=$(basename "$shader")
    # Substitute the file extension, from .comp to .spv
    shader_name="${shader_name%.*}.spv"

    # Compile the shader
    echo "Compiling ${shader}..."
    glslang -V "$shader" -o "${OUTPUT_DIR}/${shader_name}" || {
        echo "Error: Failed to compile ${shader}"
        exit 1
    }
done

echo "Shader compilation completed."
