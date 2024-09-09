/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/scalar.h
 *
 * @brief A simple and easy-to-use Scalar API in C
 *
 * @note Only pure C is used with minimal dependencies on external libraries.
 *
 * @todo Add a compile-time check or static assertion to ensure that only
 *       supported types are passed to these functions.
 */

#ifndef LINEAR_SCALAR_H
#define LINEAR_SCALAR_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "floating_point.h"

// Scalar operations

/**
 * @brief Perform arithmetic operations
 *
 * These helper functions perform basic arithmetic on two floating-point values
 * and return the result for Element-wise operations.
 *
 * @param x First operand
 * @param y Second operand
 * @param result The resultant data from the operation
 * @param type The data type for the operation
 *
 * @return Result of the operation
 */
typedef void* (*linear_operation_t)(void*, void*, void*, linear_data_t);

/**
 * @brief Add two scalar values
 */
void* scalar_add(void* a, void* b, void* result, linear_data_t type);

/**
 * @brief Subtract two scalar values
 */
void* scalar_subtract(void* a, void* b, void* result, linear_data_t type);

/**
 * @brief Multiply two scalar values
 */
void* scalar_multiply(void* a, void* b, void* result, linear_data_t type);

/**
 * @brief Divide two scalar values
 */
void* scalar_divide(void* a, void* b, void* result, linear_data_t type);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LINEAR_SCALAR_H
