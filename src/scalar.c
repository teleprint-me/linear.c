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
#include "logger.h"

#include <math.h>

// Scalar operations

void* scalar_add(void* a, void* b, linear_data_t type) {
    if (LINEAR_FLOAT == type) {
        float* result = malloc(sizeof(float));
        *result       = *(float*) a + *(float*) b;
        return result;
    } else if (LINEAR_INT) {
        int* result = malloc(sizeof(int));
        *result     = *(int*) a + *(int*) b;
        return result;
    } else {
        LOG_ERROR("scalar_add: Unsupported data type\n");
        return NULL;
    }
}

void* scalar_subtract(void* a, void* b, linear_data_t type) {
    if (LINEAR_FLOAT == type) {
        float* result = malloc(sizeof(float));
        *result       = *(float*) a - *(float*) b;
        return result;
    } else if (LINEAR_INT) {
        int* result = malloc(sizeof(int));
        *result     = *(int*) a - *(int*) b;
        return result;
    } else {
        LOG_ERROR("scalar_subtract: Unsupported data type\n");
        return NULL;
    }
}

void* scalar_multiply(void* a, void* b, linear_data_t type) {
    if (LINEAR_FLOAT == type) {
        float* result = malloc(sizeof(float));
        *result       = (*(float*) a) * (*(float*) b);
        return result;
    } else if (LINEAR_INT) {
        int* result = malloc(sizeof(int));
        *result     = (*(int*) a) * (*(int*) b);
        return result;
    } else {
        LOG_ERROR("scalar_multiply: Unsupported data type\n");
        return NULL;
    }
}

void* scalar_divide(void* a, void* b, linear_data_t type) {
    if (b == 0) {
        LOG_ERROR(
            "Division by zero is undefined. Cannot divide a (%f) by b (%f).\n",
            a,
            b
        );
        return NAN; // Division by zero is undefined
    }
    if (LINEAR_FLOAT == type) {
        float* result = malloc(sizeof(float));
        *result       = *(float*) a / *(float*) b;
        return result;
    } else if (LINEAR_INT) {
        int* result = malloc(sizeof(int));
        *result     = *(int*) a / *(int*) b;
        return result;
    } else {
        LOG_ERROR("scalar_divide: Unsupported data type\n");
        return NULL;
    }
}
