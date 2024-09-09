/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/scalar.h
 *
 * @brief A simple and easy-to-use Scalar API in C
 *
 * @note Only pure C is used with minimal dependencies on external libraries.
 */

#include "scalar.h"

#include <math.h>

// Scalar operations

float scalar_add(float x, float y) {
    return x + y;
}

float scalar_subtract(float x, float y) {
    return x - y;
}

float scalar_multiply(float x, float y) {
    return x * y;
}

float scalar_divide(float x, float y) {
    if (y == 0) {
        LOG_ERROR(
            "Division by zero is undefined. Cannot divide x (%f) by y (%f).\n",
            x,
            y
        );
        return NAN; // Division by zero is undefined
    }
    return x / y;
}
