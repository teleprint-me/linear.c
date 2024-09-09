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

#include "thread.h"

// Scalar operations

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
float scalar_add(float x, float y);

/**
 * @brief Subtract two floating-point numbers
 */
float scalar_subtract(float x, float y);

/**
 * @brief Multiply two floating-point numbers
 */
float scalar_multiply(float x, float y);

/**
 * @brief Divide two floating-point numbers
 */
float scalar_divide(float x, float y);

#endif // LINEAR_SCALAR_H
