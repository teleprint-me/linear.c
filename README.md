# linear.c

A lightweight linear algebra library written in pure C.

## Features

- Logging for debugging and error tracking.
- Floating-point comparisons.
- Support for vectors, matrices, and tensors.
- Common linear algebra operations.
- Single-precision (32-bit) floating-point support.

## Planned Features

- Multithreading using `pthreads` for CPU computation.
- GPU acceleration with Vulkan.
- Half-precision (16-bit) floating-point support.
- Brain Floating Point (bfloat16) support.
- Signal processing with:
  - 8-bit integer precision.
  - 4-bit integer precision.

## Getting Started

### Clone the Repository

To clone this repository along with its submodules, run:

```sh
git clone --recurse-submodules -j$(nproc) \
    https://github.com/teleprint-me/linear.c linear
cd linear
```

### Update Submodules

To ensure all submodules are up to date, run:

```sh
git submodule update --init --recursive
```

### Build Instructions

#### 1. Build Shaders (for Vulkan builds)

If you plan to use GPU acceleration, you need to compile the shaders first:

1. Enable execution permissions for the shader build script:

    ```sh
    chmod +x build_shaders.sh
    ```

2. Run the script to compile shaders from `.comp` to `.spv`:

    ```sh
    ./build_shaders.sh
    ```

Compiled shaders will be located in the `shaders/compiled` directory.

#### 2. Automated Build

To automate the build, use the provided script:

1. Enable execution permissions:

    ```sh
    chmod +x auto_build.sh
    ```

2. Run the script to build, compile, and test:

    ```sh
    ./auto_build.sh
    ```

#### 3. Manual Build

To manually build the project:

1. **CPU-only Build:**

    ```sh
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DLINEAR_VULKAN=0
    ```

2. **Vulkan-accelerated Build:**

    ```sh
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DLINEAR_VULKAN=1
    ```

3. **Compile the Code:**

    ```sh
    cmake --build build --config Debug -j $(nproc)
    ```

4. **Run Tests:**

    ```sh
    ctest --rerun-failed --output-on-failure --test-dir build
    ```

## Current Status

- **Vectors:** Nearly complete, with most core functionality implemented.
- **Matrices and Tensors:** Work in progress; API drafts are still being refined.
- **Data Types:** Currently, only `float` is supported.
- **Multithreading and GPU Support:** Under development; prototypes and initial drafts are in progress.
- **Signal Processing:** Not yet implemented; planned for later stages due to its complexity.

## References

### Mathematical Background

- [Calculus](https://leanpub.com/apexcalculus)
- [Discrete Mathematics](https://discrete.openmathbooks.org/dmoi3.html)
- [Statistics and Probability](https://stats.libretexts.org/Courses/Las_Positas_College/Math_40%3A_Statistics_and_Probability)
- [Linear Algebra](https://understandinglinearalgebra.org/home.html)
- [Digital Signal Processing](https://www.dspguide.com/)

### Programming References

- [C Programming Language Standard](https://www.open-std.org/JTC1/SC22/WG14/)
- [C Reference](https://en.cppreference.com/w/c)
- [CMake Reference](https://cmake.org/cmake/help/latest/)
- [Bash Reference](https://www.gnu.org/software/bash/manual/html_node/index.html)
- [Vulkan Reference](https://docs.vulkan.org/)
- [OpenGL Shading Language](https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language)

## License

This project is licensed under the AGPL License. See the [LICENSE](LICENSE) file for details.

[†](https://cyberpunk.fandom.com/wiki/ITS)
