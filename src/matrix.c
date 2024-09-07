/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/matrix.c
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

#include "matrix.h"
#include "lehmer.h"
#include "logger.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

matrix_t* matrix_create(const uint32_t rows, const uint32_t columns) {
    matrix_t* matrix = (matrix_t*) malloc(sizeof(matrix_t));
    if (NULL == matrix) {
        LOG_ERROR("Failed to allocate memory for matrix_t.\n");
        return NULL;
    }

    // Allocate a single block of memory for the matrix elements
    matrix->data = (float*) malloc(rows * columns * sizeof(float));
    if (NULL == matrix->data) {
        LOG_ERROR("Failed to allocate memory for matrix elements.\n");
        free(matrix);
        return NULL;
    }

    /**
     * Initialize all elements to zero
     *
     * @note memset may be optimized away (under the as-if rules) if the object
     *       modified by this function is not accessed again for the rest of
     *       its lifetime (e.g., gcc bug 8537). For this reason, we do not
     *       employ its use here.
     */
    for (uint32_t i = 0; i < rows * columns; i++) {
        matrix->data[i] = 0.0f;
    }

    matrix->rows    = rows;
    matrix->columns = columns;

    return matrix;
}

void matrix_free(matrix_t* matrix) {
    if (NULL == matrix) {
        LOG_ERROR("Cannot free a NULL matrix.\n");
        return;
    }

    if (matrix->data) {
        free(matrix->data);
    }

    free(matrix);
}

float matrix_get_element(
    const matrix_t* matrix, const uint32_t row, const uint32_t column
) {
    if (row >= matrix->rows || column >= matrix->columns) {
        LOG_ERROR("Index out of bounds.\n");
        return NAN;
    }
    return matrix->data[row * matrix->columns + column];
}

bool matrix_set_element(
    matrix_t* matrix, uint32_t row, uint32_t column, float value
) {
    if (row >= matrix->rows || column >= matrix->columns) {
        LOG_ERROR("Index out of bounds.\n");
        return false;
    }
    matrix->data[row * matrix->columns + column] = value;
    return true;
}

uint32_t matrix_elements(const matrix_t* matrix) {
    return matrix->rows * matrix->columns;
}

// Initialization Operations

void matrix_fill(matrix_t* matrix, const float value) {
    uint32_t max_elements = matrix_elements(matrix);
    for (uint32_t i = 0; i < max_elements; i++) {
        matrix->data[i] = value;
    }
}

static void matrix_lehmer_initialize(
    lehmer_state_t* state,
    matrix_t*       matrix,
    double (*lehmer_callback)(lehmer_state_t*)
) {
    uint32_t max_elements = matrix_elements(matrix);
    for (uint32_t i = 0; i < max_elements; i++) {
        // Cast from double to float, as the vector uses float values
        float n         = (float) lehmer_callback(state);
        matrix->data[i] = n;
    }
}

void matrix_lehmer_modulo(lehmer_state_t* state, matrix_t* matrix) {
    matrix_lehmer_initialize(state, matrix, lehmer_random_modulo);
}

void matrix_lehmer_gamma(lehmer_state_t* state, matrix_t* matrix) {
    matrix_lehmer_initialize(state, matrix, lehmer_random_gamma);
}

void matrix_lehmer_delta(lehmer_state_t* state, matrix_t* matrix) {
    matrix_lehmer_initialize(state, matrix, lehmer_random_delta);
}

matrix_t* matrix_deep_copy(const matrix_t* matrix) {
    LOG_ERROR("Copying matrices is not implemented.\n");
    return NULL;
}

matrix_t* matrix_shallow_copy(const matrix_t* matrix) {
    LOG_ERROR("Copying matrices is not implemented.\n");
    return NULL;
}

bool matrix_is_zero(const matrix_t* matrix) {
    for (uint32_t i = 0; i < matrix->rows * matrix->columns; i++) {
        if (matrix->data[i] != 0.0f) {
            return false;
        }
    }
    return true;
}

bool matrix_is_square(const matrix_t* matrix) {
    return matrix->rows == matrix->columns;
}

bool matrix_is_transposed(const matrix_t* matrix) {
    return matrix->state & MATRIX_TRANSPOSED;
}

bool matrix_is_identity(const matrix_t* matrix) {
    if (!matrix_is_square(matrix)) {
        return false;
    }

    for (uint32_t i = 0; i < matrix->rows; i++) {
        for (uint32_t j = 0; j < matrix->columns; j++) {
            float value = matrix->data[i * matrix->columns + j];
            if ((i == j && value != 1.0f) || (i != j && value != 0.0f)) {
                return false;
            }
        }
    }
    return true;
}

// Matrix-Scalar Operations

/**
 * @brief Perform an element-wise scalar operation on a matrix.
 *
 * @param matrix A pointer to the matrix to operate on.
 * @param scalar The scalar value to apply.
 * @param operation The operation to apply element-wise.
 *
 * @return A new matrix containing the results of the operation.
 */
matrix_t* matrix_scalar_operation(
    const matrix_t* matrix, float scalar, float (*operation)(float, float)
) {
    // Allocate a new matrix for the result
    matrix_t* result = matrix_create(matrix->rows, matrix->columns);
    if (NULL == result) {
        LOG_ERROR("Failed to allocate memory to resulting matrix.");
        return NULL; // Handle memory allocation failure
    }

    uint32_t max_elements = matrix_elements(matrix);

    for (uint32_t i = 0; i < max_elements; i++) {
        result->data[i] = operation(matrix->data[i], scalar);
    }

    return result;
}

/**
 * @brief Add a scalar to each element of the matrix.
 */
matrix_t* matrix_scalar_add(const matrix_t* matrix, float scalar) {
    return matrix_scalar_operation(matrix, scalar, scalar_add);
}

/**
 * @brief Subtract a scalar from each element of the matrix.
 */
matrix_t* matrix_scalar_subtract(const matrix_t* matrix, float scalar) {
    return matrix_scalar_operation(matrix, scalar, scalar_subtract);
}

/**
 * @brief Multiply each element of the matrix by a scalar.
 */
matrix_t* matrix_scalar_multiply(const matrix_t* matrix, float scalar) {
    return matrix_scalar_operation(matrix, scalar, scalar_multiply);
}

/**
 * @brief Divide each element of the matrix by a scalar.
 */
matrix_t* matrix_scalar_divide(const matrix_t* matrix, float scalar) {
    return matrix_scalar_operation(matrix, scalar, scalar_divide);
}
