/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/matrix.h
 *
 * @brief A simple and easy to use Matrix API
 *
 * This library provides a Matrix API designed for high performance
 * and ease of use in scientific computing, data analysis, and machine learning
 * applications. It leverages pure C for core operations to ensure maximum
 * compatibility and performance with a focus on simplicity, speed, and
 * flexibility.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef LINEAR_MATRIX_H
#define LINEAR_MATRIX_H

#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>

// @note combine multiple states, e.g.
//    matrix.state = MATRIX_TRANSPOSED | MATRIX_SCALED;
// @note check for specific state, e.g.
//    (matrix.state & MATRIX_TRANSPOSED) ? x : y;
typedef enum MatrixState {
    MATRIX_NONE       = 0,      // Default state, no transformation applied.
    MATRIX_TRANSPOSED = 1 << 0, // 0b0001
    MATRIX_SCALED     = 1 << 1, // 0b0010
    MATRIX_ROTATED    = 1 << 2, // 0b0100
    MATRIX_TRANSLATED = 1 << 3  // 0b1000
} matrix_state_t;

/**
 * @brief A structure representing an N-dimensional matrix.
 *
 * This structure stores the number of dimensions and a dynamic array of
 * floating-point values, which represent the components of the matrix in each
 * dimension.
 *
 * @param data   One-dimensional array representing the matrix elements.
 * @param columns The width of the matrix.
 * @param rows The height of the matrix.
 * @param state Indicates the matrix's state using bitwise flags.
 *
 * @note Using size_t creates an undesirable alignment of 8-bytes!
 * - size_t -> unsigned long -> 8 bytes (this is big; undesirable)
 * - uint32_t -> unsigned int -> 4 bytes (this is small; desirable)
 * - int32_t -> signed int -> int -> 4 bytes (this is okay too)
 *
 * @note The alignment for matrix_t should be 4-bytes to keep its size small.
 * size_t is an unsigned long at a whopping 8-bytes while unsigned int is only
 * 4-bytes. This reduces the size significantly and uses 16-bytes instead of a
 * whole 32-bytes. Using a type double is also 8-bytes. Bigger is not
 * necessarily better.
 *
 * @note Matrix operations are at least some matrix a^(n*m) which is O(n^3) in
 * terms of time complexity in operations which can be reduced to O(n^2) in
 * terms of time complexity depending on the algorithm used. It's too early to
 * optimize, but this is a crucial detail that will impact performance as the
 * input size grows, which is, unfortunately, easy to do.
 */
typedef struct Matrix {
    float*   data; ///< One-dimensional array representing the matrix elements.
    uint32_t columns; ///< The number of columns in the matrix.
    uint32_t rows;    ///< The number of rows in the matrix.
    uint32_t state;   ///< Indicates the matrix's state using bitwise flags.
} matrix_t;

// Matrix lifecycle management

matrix_t* matrix_create(const uint32_t rows, const uint32_t columns);
void      matrix_free(matrix_t* matrix);

// Element Access

float matrix_element_get(
    const matrix_t* matrix, const uint32_t row, const uint32_t column
);

bool matrix_element_set(
    matrix_t*      matrix,
    const uint32_t row,
    const uint32_t column,
    const float    value
);

// total number of elements within the matrix
uint32_t matrix_element_count(const matrix_t* matrix);

// Initialization Operations

/**
 * @brief Fills a matrix with a constant value.
 *
 * Sets all elements of the matrix to a specified value.
 *
 * @param matrix A pointer to the matrix to fill.
 * @param value The constant value to assign to each element of the matrix.
 */
void matrix_fill(matrix_t* matrix, const float value);

/**
 * @brief Helper function to initialize a matrix using a specified random
 *        generator.
 *
 * This function serves as a helper to initialize a matrix's data using a given
 * random number generation method from the Lehmer family of algorithms.
 *
 * @param state A pointer to the Lehmer state used for random number
 *              generation.
 * @param matrix A pointer to the matrix to initialize.
 * @param lehmer_callback A function pointer to the specific Lehmer random
 *                        number generator variant to use (e.g.,
 *                        lehmer_random_modulo, lehmer_random_gamma,
 *                        lehmer_random_delta).
 */
static void matrix_lehmer_initialize(
    lehmer_state_t* state,
    matrix_t*       matrix,
    double (*lehmer_callback)(lehmer_state_t*)
);

/**
 * @brief Initialize a matrix using the Lehmer-Park modulo method.
 *
 * @param state A pointer to the Lehmer state used for random number
 *              generation.
 * @param matrix A pointer to the matrix to initialize.
 */
void matrix_lehmer_modulo(lehmer_state_t* state, matrix_t* matrix);

/**
 * @brief Initialize a matrix using the Lehmer-Park gamma method.
 *
 * @param state A pointer to the Lehmer state used for random number
 *              generation.
 * @param matrix A pointer to the matrix to initialize.
 */
void matrix_lehmer_gamma(lehmer_state_t* state, matrix_t* matrix);

/**
 * @brief Initialize a matrix using the Lehmer-Park delta method.
 *
 * @param state A pointer to the Lehmer state used for random number
 *              generation.
 * @param matrix A pointer to the matrix to initialize.
 */
void matrix_lehmer_delta(lehmer_state_t* state, matrix_t* matrix);

// @todo Initialize using mersenne twister
// @note Planned, but currently unavailable.
//       See teleprint-me/lehmer.c for more information.

// Copy Operations
matrix_t* matrix_deep_copy(const matrix_t* matrix);
matrix_t* matrix_shallow_copy(const matrix_t* matrix);

// Matrix Properties
bool matrix_is_zero(const matrix_t* matrix);
bool matrix_is_square(const matrix_t* matrix);
bool matrix_is_transposed(const matrix_t* matrix);
bool matrix_is_identity(const matrix_t* matrix);

// Matrix-Scalar Operations
matrix_t* matrix_scalar_operation(
    const matrix_t* matrix, float scalar, float (*operation)(float, float)
);
matrix_t* matrix_scalar_add(const matrix_t* matrix, float scalar);
matrix_t* matrix_scalar_subtract(const matrix_t* matrix, float scalar);
matrix_t* matrix_scalar_multiply(const matrix_t* matrix, float scalar);
matrix_t* matrix_scalar_divide(const matrix_t* matrix, float scalar);

// Matrix-Vector Operations
matrix_t* matrix_vector_operation(
    const matrix_t* matrix,
    const vector_t* vector,
    float (*operation)(float, float)
);
matrix_t* matrix_vector_add(const matrix_t* matrix, const vector_t* vector);
matrix_t*
matrix_vector_subtract(const matrix_t* matrix, const vector_t* vector);
matrix_t*
matrix_vector_multiply(const matrix_t* matrix, const vector_t* vector);
matrix_t* matrix_vector_divide(const matrix_t* matrix, const vector_t* vector);

// Matrix-Matrix Operations
matrix_t* matrix_matrix_operation(
    const matrix_t* a, const matrix_t* b, float (*operation)(float, float)
);
matrix_t* matrix_matrix_add(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_subtract(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_multiply(const matrix_t* a, const matrix_t* b);
matrix_t* matrix_matrix_divide(const matrix_t* a, const matrix_t* b);

// Matrix Transformations
matrix_t* matrix_transpose(matrix_t* matrix);
float     matrix_dot_product(const matrix_t* a, const matrix_t* b);

#endif // LINEAR_MATRIX_H
