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

#include "numeric_types.h"

// Scalar operations

/**
 * @brief Execute the literal arithmetic operation
 *
 * These helper functions perform basic arithmetic on two values and return the
 * result for Element-wise operations. This function applies the literal type
 * casting based on the selected data type.
 *
 * @param a First operand
 * @param b Second operand
 * @param result The resultant data from the operation
 *
 * @return Result of the operation
 *
 * @note This is for internal use only - private.
 */
typedef void* (*scalar_arithmetic_t)(void*, void*, void*);

/**
 * @brief Execute the representative arithmetic operation
 *
 * These helper functions perform basic arithmetic on two values and return the
 * result for Element-wise operations. This function abstracts the literal
 * operation based on the data type and automates selection, type casting,
 * and operational execution.
 *
 * @param a First operand
 * @param b Second operand
 * @param result The resultant data from the operation
 * @param type The data type for the operation
 *
 * @return Result of the operation
 *
 * @note This is for external use only - public.
 */
typedef void* (*scalar_operation_t)(void*, void*, void*, numeric_data_t);

/**
 * @brief Add two scalar values
 */
void* scalar_add(void* a, void* b, void* result, numeric_data_t type);

/**
 * @brief Subtract two scalar values
 */
void* scalar_subtract(void* a, void* b, void* result, numeric_data_t type);

/**
 * @brief Multiply two scalar values
 */
void* scalar_multiply(void* a, void* b, void* result, numeric_data_t type);

/**
 * @brief Divide two scalar values
 */
void* scalar_divide(void* a, void* b, void* result, numeric_data_t type);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LINEAR_SCALAR_H
