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
- Signal processing for:
  - 8-bit integer precision.
  - 4-bit integer precision.

## Build Instructions

### Automated Build

To use the automated build script:

1. Enable execution permissions:

    ```sh
    chmod +x auto_build.sh
    ```

2. Run the script to build, compile, and test:

    ```sh
    ./auto_build.sh
    ```

### Manual Build

To manually build the project, follow these steps:

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

## License

This project is licensed under the AGPL License. See the [LICENSE](LICENSE) file for details.
