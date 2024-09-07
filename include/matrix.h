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

typedef enum MatrixState {
    MATRIX_TRANSPOSED, // maybe optional?
    MATRIX_SCALED,     // might be useful
    MATRIX_ROTATED,    // probably over-kill/over-engineered
    MATRIX_TRANSLATED, // ditto...
} matrix_state_t;

/**
 * @brief A structure representing an N-dimensional vector.
 *
 * This structure stores the number of dimensions and a dynamic array of
 * floating-point values, which represent the components of the vector in each
 * dimension.
 *
 * @param data   One-dimensional array representing the vector elements.
 * @param columns The width of the matrix.
 * @param rows The height of the matrix.
 * @param is_transposed
 *
 * @note The alignment for the matrix_t is 4-bytes (32-bit) and the size is
 * 4-bytes, so unsigned long (size_t) is also 4-bytes at a 32-bit width, 4 * 3
 * is 12 bytes. The bool is 1-byte, but the alignment adds an overhead of
 * 4-bytes. This becomes a 16-byte structure.
 * @note Matrix operations are at least a^(n*m) operations which can be reduced
 * to O(n^2) in terms of time complexity. It's too early to optimize, but this
 * is a crucial detail that will impact performance as the input size grows,
 * which is -unfortunately- easy to do.
 * @note consider using type int to represents flags, e.g. transposed, scaled,
 * rotated, translated, etc. it still amounts to using 16-bytes, so, idk. it's
 * better than adding a bunch of bools, takes up about the same amount of
 * space, but allows more flags if needed with the same space. seems more
 * efficient.
 */
typedef struct Matrix {
    float*  data; ///< One-dimensional array representing the matrix elements.
    size_t  rows; ///< The number of rows in the matrix.
    size_t  columns; ///< The number of columns in the matrix.
    int32_t state;   ///< Indicates the matrices state
} matrix_t;

// Matrix lifecycle management
matrix_t* matrix_create(const size_t rows, const size_t columns);
void      matrix_free(matrix_t* matrix);

// Element Access
float matrix_get_element(
    const matrix_t* matrix, const size_t row, const size_t column
);
bool matrix_set_element(
    matrix_t* matrix, const size_t row, const size_t column, const float value
);
// total number of elements within the matrix
size_t matrix_elements(const matrix_t* matrix);

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
