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

// not sure if this should be here or in thread.h?
typedef void* (*linear_operation_t)(void*, void*, linear_data_t type);

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

/**
 * @brief Add two floating-point numbers
 */
void* scalar_add(void* x, void* y, linear_data_t type);

/**
 * @brief Subtract two floating-point numbers
 */
void* scalar_subtract(void* x, void* y, linear_data_t type);

/**
 * @brief Multiply two floating-point numbers
 */
void* scalar_multiply(void* x, void* y, linear_data_t type);

/**
 * @brief Divide two floating-point numbers
 */
void* scalar_divide(void* x, void* y, linear_data_t type);

#endif // LINEAR_SCALAR_H
