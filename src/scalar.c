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

// Scalar operations

void* scalar_add(void* a, void* b, void* result, linear_data_t type) {
    if (LINEAR_FLOAT == type) {
        *(float*) result = *(float*) a + *(float*) b;
        return (float*) result;
    } else if (LINEAR_INT == type) {
        *(int*) result = *(int*) a + *(int*) b;
        return (int*) result;
    } else {
        LOG_ERROR("scalar_add: Unsupported data type\n");
        return NULL;
    }
}

void* scalar_subtract(void* a, void* b, void* result, linear_data_t type) {
    if (LINEAR_FLOAT == type) {
        *(float*) result = *(float*) a - *(float*) b;
        return (float*) result;
    } else if (LINEAR_INT == type) {
        *(int*) result = *(int*) a - *(int*) b;
        return (int*) result;
    } else {
        LOG_ERROR("scalar_subtract: Unsupported data type\n");
        return NULL;
    }
}

void* scalar_multiply(void* a, void* b, void* result, linear_data_t type) {
    if (LINEAR_FLOAT == type) {
        *(float*) result = (*(float*) a) * (*(float*) b);
        return (float*) result;
    } else if (LINEAR_INT == type) {
        *(int*) result = (*(int*) a) * (*(int*) b);
        return (int*) result;
    } else {
        LOG_ERROR("scalar_multiply: Unsupported data type\n");
        return NULL;
    }
}

void* scalar_divide(void* a, void* b, void* result, linear_data_t type) {
    if ((LINEAR_FLOAT == type && 0.0f == *(float*) b)
        || (LINEAR_INT == type && 0 == *(int*) b)) {
        LOG_ERROR("Division by zero is undefined. Cannot divide.\n");
        return NULL;
    }
    if (LINEAR_FLOAT == type) {
        *(float*) result = *(float*) a / *(float*) b;
        return (float*) result;
    } else if (LINEAR_INT == type) {
        *(int*) result = *(int*) a / *(int*) b;
        return (int*) result;
    } else {
        LOG_ERROR("scalar_divide: Unsupported data type\n");
        return NULL;
    }
}
