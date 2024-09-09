/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/scalar.h
 *
 * @brief A simple and easy-to-use Scalar API in C
 *
 * @note Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef LINEAR_SCALAR_H
#define LINEAR_SCALAR_H

// Scalar operations

// Define the linear data type
typedef enum LinearData {
    LINEAR_FLOAT,
    LINEAR_INT,
} linear_data_t;

/**
 * @brief Perform arithmetic operations
 *
 * These helper functions perform basic arithmetic on two floating-point values
 * and return the result for Element-wise operations.
 *
 * @param x First operand
 * @param y Second operand
 *
 * @return Result of the operation
 */
typedef void* (*linear_operation_t)(void*, void*, void*, linear_data_t);

/**
 * @brief Add two floating-point numbers
 */
void* scalar_add(void* a, void* b, void* result, linear_data_t type);

/**
 * @brief Subtract two floating-point numbers
 */
void* scalar_subtract(void* a, void* b, void* result, linear_data_t type);

/**
 * @brief Multiply two floating-point numbers
 */
void* scalar_multiply(void* a, void* b, void* result, linear_data_t type);

/**
 * @brief Divide two floating-point numbers
 */
void* scalar_divide(void* a, void* b, void* result, linear_data_t type);

#endif // LINEAR_SCALAR_H
