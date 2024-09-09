/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/scalar.c
 *
 * @brief A simple and easy-to-use Scalar API in C
 *
 * @note Only pure C is used with minimal dependencies on external libraries.
 *
 * @todo Add a compile-time check or static assertion to ensure that only
 *       supported types are passed to these functions.
 *
 * - this is a finite-state machine problem. reference lua source for guidance.
 *   this reminds of me of lisp. ugh.
 * - switch statements produce more overhead than if-else statements even
 *   though switch statements are easier to read.
 */

#include "scalar.h"
#include "logger.h"

// Add operations

void* scalar_add_float(void* a, void* b, void* result) {
    *(float*) result = (*(float*) a) + (*(float*) b);
    return result;
}

void* scalar_add_int32(void* a, void* b, void* result) {
    *(int32_t*) result = (*(int32_t*) a) + (*(int32_t*) b);
    return result;
}

// Subtract operations

void* scalar_subtract_float(void* a, void* b, void* result) {
    *(float*) result = (*(float*) a) - (*(float*) b);
    return (float*) result;
}

void* scalar_subtract_int32(void* a, void* b, void* result) {
    *(int32_t*) result = (*(int32_t*) a) - (*(int32_t*) b);
    return (float*) result;
}

// Multiply operations

void* scalar_multiply_float(void* a, void* b, void* result) {
    *(float*) result = (*(float*) a) * (*(float*) b);
    return (float*) result;
}

void* scalar_multiply_int32(void* a, void* b, void* result) {
    *(int32_t*) result = (*(int32_t*) a) * (*(int32_t*) b);
    return (float*) result;
}

// Divide operations

void* scalar_divide_float(void* a, void* b, void* result) {
    if (0.0f == *(float*) b) {
        LOG_ERROR("Division by zero is undefined. Cannot divide.\n");
        return NULL;
    }
    *(float*) result = (*(float*) a) / (*(float*) b);
    return (float*) result;
}

void* scalar_divide_int32(void* a, void* b, void* result) {
    if (0 == *(int*) b) {
        LOG_ERROR("Division by zero is undefined. Cannot divide.\n");
        return NULL;
    }
    *(int32_t*) result = (*(int32_t*) a) / (*(int32_t*) b);
    return (int32_t*) result;
}

// Scalar operations

void* scalar_add(void* a, void* b, void* result, linear_data_t type) {
    switch (type) {
        case LINEAR_FLOAT:
            return scalar_add_float(a, b, result);
        case LINEAR_INT32:
            return scalar_add_int32(a, b, result);
        default:
            LOG_ERROR("scalar_add: Unsupported data type\n");
            return NULL;
    }
}

void* scalar_subtract(void* a, void* b, void* result, linear_data_t type) {
    switch (type) {
        case LINEAR_FLOAT:
            return scalar_subtract_float(a, b, result);
        case LINEAR_INT32:
            return scalar_subtract_int32(a, b, result);
        default:
            LOG_ERROR("scalar_subtract: Unsupported data type\n");
            return NULL;
    }
}

void* scalar_multiply(void* a, void* b, void* result, linear_data_t type) {
    switch (type) {
        case LINEAR_FLOAT:
            return scalar_multiply_float(a, b, type);
        case LINEAR_INT32:
            return scalar_multiply_int32(a, b, result);
        default:
            LOG_ERROR("scalar_multiply: Unsupported data type\n");
            return NULL;
    }
}

void* scalar_divide(void* a, void* b, void* result, linear_data_t type) {
    switch (type) {
        case LINEAR_FLOAT:
            return scalar_divide_float(a, b, result);
        case LINEAR_INT32:
            return scalar_divide_int32(a, b, result);
        default:
            LOG_ERROR("scalar_divide: Unsupported data type\n");
            return NULL;
    }
}
