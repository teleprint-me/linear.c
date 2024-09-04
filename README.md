# linear.c
Linear Algebra written in pure C.

## feat

- logging
- floating-point comparisons
- vectors, matrices, and tensors
- common linear operations
- support 32-bit (single-precision)

### todo

- cpu via pthread
- gpu via vulkan
- 16-bit (half-precision)
- 16-bit (bfloat)
- signal processing
    - 8-bit (int for k-bit)
    - 4-bit (int for k-bit)

## auto build

### enable execution

```sh
chmod +x auto_build.sh
```

### run build and tests

```sh
# automates building, compiling, and testing
./auto-build # auto-build -> auto_build.sh
```

## manual build

### CPU build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DLINEAR_VULKAN=0
```

### Vulkan build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DLINEAR_VULKAN=1
```

### compile

```sh
cmake --build build --config Debug -j $(nproc)
```

### test

```sh
ctest --rerun-failed --output-on-failure --test-dir build
```

## License

This project is licensed under the AGPL License - see the [LICENSE](LICENSE)
file for details.
