/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vector.h
 *
 * @brief A simple and easy-to-use Vector API in C
 *
 * This header file provides an implementation for basic mathematical
 * operations on N-dimensional vectors, including creation, destruction,
 * element-wise functions, scalar-based functions, vector-based functions,
 * common vector operations, special vector operations, and conversion between
 * polar and cartesian coordinates.
 *
 * @note Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef LINEAR_VECTOR_H
#define LINEAR_VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

// Lifecycle management

/**
 * @brief A structure representing an N-dimensional vector.
 *
 * This structure stores the number of dimensions and a dynamic array of
 * floating-point values, which represent the components of the vector in each
 * dimension.
 *
 * @param data   One-dimensional array representing the vector elements.
 * @param columns The number of elements (dimensions) in the vector.
 */
typedef struct Vector {
    float* data; ///< One-dimensional array representing the vector elements.
    size_t columns; ///< The number of elements (dimensions) in the vector.
} vector_t;

/**
 * @brief Create a new N-dimensional vector
 *
 * This function dynamically allocates memory and initializes a new vector with
 * the specified number of dimensions. The values in the vector are set to zero
 * by default.
 *
 * @param columns The number of elements (dimensions) in the vector.
 *
 * @return A pointer to the newly created vector
 */
vector_t* vector_create(const size_t columns);

/**
 * @brief Free an allocated N-dimensional vector
 *
 * This function deallocates memory associated with a given vector, releasing
 * any resources used during its creation. It is essential to call this
 * function when you no longer need the vector to avoid memory leaks. After
 * freeing the vector, set the pointer to NULL to prevent double-free errors in
 * future calls or accidental usage of freed memory.
 *
 * @param vector A pointer to the vector to be freed
 *
 * @note - 7.22.3 Memory management functions on page 347
 * @ref <https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf>
 * @note - Calling free on a pointer twice
 * @ref <https://stackoverflow.com/q/34284846/15147156>
 */
void vector_free(vector_t* vector);

// Copy operations

/**
 * @brief Deep copy a given N-dimensional vector
 *
 * This function creates a deep copy of the input vector by allocating new
 * memory and duplicating its contents element-wise.
 *
 * @param vector Input vector
 *
 * @return A pointer to the newly created, deep copied vector
 */
vector_t* vector_deep_copy(const vector_t* vector);

/**
 * @brief Shallow copy an N-dimensional vector
 *
 * This function returns a pointer to the same allocated memory as the input
 * vector. Effectively creating a reference (shallow) copy of the original
 * vector's data, without allocating new memory for it.
 *
 * @param vector Input vector
 *
 * @return A pointer to the shallow copied vector
 */
vector_t* vector_shallow_copy(const vector_t* vector);

// Element-wise operations

/**
 * @brief Element-wise operations
 *
 * These helper functions perform basic arithmetic on two floating-point values
 * and return the result.
 *
 * @param x First operand
 * @param y Second operand
 *
 * @return Result of the operation
 */
float scalar_add(float x, float y);
float scalar_subtract(float x, float y);
float scalar_multiply(float x, float y);
float scalar_divide(float x, float y);

/**
 * @brief Executor for element-wise vector-to-scalar functions
 *
 * This function applies a given operation to each corresponding pair of
 * elements in two vectors and returns the resulting vector.
 *
 * @param a First input vector
 * @param b Second input scalar
 * @param operation A pointer to the function performing the element-wise
 * operation
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_elementwise_operation(
    const vector_t* a, const float b, float (*operation)(float, float)
);

// Scalar based vector operations

/**
 * @brief Add a scalar value to an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to add
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_add(const vector_t* a, const float b);

/**
 * @brief Subtract a scalar value from an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to subtract
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_subtract(const vector_t* a, const float b);

/**
 * @brief Multiply a scalar value with an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to multiply
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_multiply(const vector_t* a, const float b);

/**
 * @brief Divide an N-dimensional vector by a scalar value
 *
 * @param a Input vector
 * @param b Scalar value to divide by
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_divide(const vector_t* a, const float b);

// Vector based operations

/**
 * @brief Executor for element-wise vector-to-vector functions
 *
 * This function applies a given operation to each corresponding pair of
 * elements in two vectors and returns the resulting vector.
 *
 * @param a First input vector
 * @param b Second input vector
 * @param operation A pointer to the function performing the element-wise
 * operation
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_elementwise_operation(
    const vector_t* a, const vector_t* b, float (*operation)(float, float)
);

/**
 * @brief Add two N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_add(const vector_t* a, const vector_t* b);

/**
 * @brief Subtract one N-dimensional vector from another
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_subtract(const vector_t* a, const vector_t* b);

/**
 * @brief Multiply two N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_multiply(const vector_t* a, const vector_t* b);

/**
 * @brief Divide an N-dimensional vector by another N-dimensional vector
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_divide(const vector_t* a, const vector_t* b);

// Common vector operations

/**
 * @brief Determine the magnitude or length of an N-dimensional vector
 *
 * @param vector Input vector
 *
 * @return The magnitude of the vector
 */
float vector_magnitude(const vector_t* vector);

/**
 * @brief Calculate the distance between two given N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return The distance between the two vectors
 */
float vector_distance(const vector_t* a, const vector_t* b);

/**
 * @brief Calculate the mean of an N-dimensional vector
 *
 * This function estimates the mean by weighted averaging.
 *
 * m(n) = [x(1) + x(2) + ... + x(n)] / n
 *
 * @param vector Input vector
 *
 * @return The mean of the vector
 */
float vector_mean(const vector_t* vector);

/**
 * @brief Low pass filter on an N-dimensional vector
 *
 * @note This function estimates the mean by low-pass filtering rather than
 *       averaging.
 *
 * m(n + 1) = ((1 - α) * m(n)) + (α * x(n + 1))
 *
 * @param vector Input vector
 * @param alpha Smoothing factor for the low-pass filter
 *
 * @return The low-pass filtered mean of the vector
 *
 * @ref low-pass filter mean of a vector
 * <https://www.cs.princeton.edu/courses/archive/fall08/cos436/Duda/PR_learn/mean.htm>
 */
float vector_low_pass_filter(const vector_t* vector, float alpha);

/**
 * @brief Normalize a given N-dimensional vector in place
 *
 * @param vector Input vector
 * @param inplace Boolean flag indicating whether to modify the input vector or
 * return a new vector
 *
 * @return A pointer to the normalized vector
 */
vector_t* vector_normalize(vector_t* vector, bool inplace);

/**
 * @brief Scale an N-dimensional vector by the specified factor
 *
 * @param vector Input vector
 * @param scalar Scaling factor
 * @param inplace Boolean flag indicating whether to modify the input vector or
 * return a new vector
 *
 * @return A pointer to the scaled vector
 */
vector_t* vector_scale(vector_t* vector, float scalar, bool inplace);

/**
 * @brief Clip an N-dimensional vector within a given range
 *
 * @param vector Input vector
 * @param min Minimum value for clipping
 * @param max Maximum value for clipping
 * @param inplace Boolean flag indicating whether to modify the input vector or
 * return a new vector
 *
 * @return A pointer to the clipped vector
 */
vector_t* vector_clip(vector_t* vector, float min, float max, bool inplace);

// Special vector operations

/**
 * @brief Calculate the dot product of two N-dimensional vectors
 *
 * Dot product is n-dimensional
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return The dot product of the two vectors
 */
float vector_dot_product(const vector_t* a, const vector_t* b);

/**
 * @brief Return the cross product of two 3D vectors
 *
 * Cross product is 3-dimensional
 *
 * @param a First input vector (3D vector)
 * @param b Second input vector (3D vector)
 *
 * @return A pointer to the cross product vector
 */
vector_t* vector_cross_product(const vector_t* a, const vector_t* b);

// Special coordinates

/**
 * @brief Convert polar coordinates to cartesian coordinates
 *
 * Polar coordinates are defined as the ordered pair (r, θ) names a point r
 * units from origin along the terminal side of angle θ in standard position
 * (origin to elements).
 *
 * x = r cos θ and y = r sin θ
 *
 * @param polar_vector Input vector in polar coordinates
 *
 * @return A pointer to the vector in cartesian coordinates
 */
vector_t* vector_polar_to_cartesian(const vector_t* polar_vector);

/**
 * @brief Convert cartesian coordinates to polar coordinates
 *
 * The derivation between polar and cartesian coordinates is to consider a
 * point P with the rectangular (x, y) and polar (r, θ) coordinates.
 *
 * r = ± √(x^2 + y^2) and tan θ = y / x
 *
 * @param cartesian_vector Input vector in cartesian coordinates
 *
 * @return A pointer to the vector in polar coordinates
 */
vector_t* vector_cartesian_to_polar(const vector_t* cartesian_vector);

#endif // LINEAR_VECTOR_H
