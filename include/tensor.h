/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/tensor.h
 *
 * @brief A simple and easy to use Tensor API
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef LINEAR_TENSOR_H
#define LINEAR_TENSOR_H

#include "matrix.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    float* data; ///< One-dimensional array representing the tensor elements.
    size_t rows; ///< Number of rows (height) of each 2D matrix slice.
    size_t columns; ///< Number of columns (width) of each 2D matrix slice.
    size_t layers;  ///< Number of layers (depth) of the tensor.
} tensor_t;

// lifecycle management
tensor_t* tensor_create(size_t rows, size_t columns, size_t layers);
void      tensor_free(tensor_t* tensor);

// Element Access
float tensor_get_element(
    tensor_t* tensor, size_t row, size_t column, size_t layer
);
void tensor_set_element(
    tensor_t* tensor, size_t row, size_t column, size_t layer, float value
);
// total number of elements within the tensor
size_t tensor_elements(const tensor_t* tensor);

// Initialization Operations
void tensor_fill(tensor_t* tensor, const float value);
void tensor_random_fill(tensor_t* tensor, const float min, const float max);

// Copy Operations
tensor_t* tensor_deep_copy(const tensor_t* tensor);
tensor_t* tensor_shallow_copy(const tensor_t* tensor);

#endif // LINEAR_TENSOR_H
