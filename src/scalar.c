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

void* scalar_add_float32(void* a, void* b, void* result) {
    *(float*) result = (*(float*) a) + (*(float*) b);
    return (float*) result;
}

void* scalar_add_int32(void* a, void* b, void* result) {
    *(int32_t*) result = (*(int32_t*) a) + (*(int32_t*) b);
    return (int32_t*) result;
}

// Subtract operations

void* scalar_subtract_float32(void* a, void* b, void* result) {
    *(float*) result = (*(float*) a) - (*(float*) b);
    return (float*) result;
}

void* scalar_subtract_int32(void* a, void* b, void* result) {
    *(int32_t*) result = (*(int32_t*) a) - (*(int32_t*) b);
    return (int32_t*) result;
}

// Multiply operations

void* scalar_multiply_float32(void* a, void* b, void* result) {
    *(float*) result = (*(float*) a) * (*(float*) b);
    return (float*) result;
}

void* scalar_multiply_int32(void* a, void* b, void* result) {
    *(int32_t*) result = (*(int32_t*) a) * (*(int32_t*) b);
    return (int32_t*) result;
}

// Divide operations

void* scalar_divide_float32(void* a, void* b, void* result) {
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

// Function pointer arrays for operations
static scalar_arithmetic_t scalar_operations_add[NUMERIC_TYPES] = {
    scalar_add_float32,
    scalar_add_int32,
};

static scalar_arithmetic_t scalar_operations_subtract[NUMERIC_TYPES] = {
    scalar_subtract_float32,
    scalar_subtract_int32,
};

static scalar_arithmetic_t scalar_operations_multiply[NUMERIC_TYPES] = {
    scalar_multiply_float32,
    scalar_multiply_int32,
};

static scalar_arithmetic_t scalar_operations_divide[NUMERIC_TYPES] = {
    scalar_divide_float32,
    scalar_divide_int32,
};

// Helper function to get the correct function pointer
static scalar_arithmetic_t
scalar_operation_get(scalar_arithmetic_t* operations, numeric_data_t type) {
    if (type >= 0 && type < NUMERIC_TYPES) {
        return operations[type];
    }
    LOG_ERROR("Unsupported data type\n");
    return NULL;
}

// Scalar operations

void* scalar_add(void* a, void* b, void* result, numeric_data_t type) {
    scalar_arithmetic_t op = scalar_operation_get(scalar_operations_add, type);
    return op ? op(a, b, result) : NULL;
}

void* scalar_subtract(void* a, void* b, void* result, numeric_data_t type) {
    scalar_arithmetic_t op
        = scalar_operation_get(scalar_operations_subtract, type);
    return op ? op(a, b, result) : NULL;
}

void* scalar_multiply(void* a, void* b, void* result, numeric_data_t type) {
    scalar_arithmetic_t op
        = scalar_operation_get(scalar_operations_multiply, type);
    return op ? op(a, b, result) : NULL;
}

void* scalar_divide(void* a, void* b, void* result, numeric_data_t type) {
    scalar_arithmetic_t op
        = scalar_operation_get(scalar_operations_divide, type);
    return op ? op(a, b, result) : NULL;
}
