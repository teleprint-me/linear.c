/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vector.c
 *
 * @brief A simple and easy-to-use Vector API in C
 *
 * This header file provides an implementation for basic mathematical
 * operations on N-dimensional vectors, including creation, destruction,
 * element-wise functions, scalar-based functions, vector-based functions,
 * common vector operations, special vector operations, and conversion between
 * polar and cartesian coordinates.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include "vector.h"
#include "logger.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Create a new N-dimensional vector
 *
 * @note This function dynamically allocates memory and initializes a new
 *       vector with the specified number of dimensions. The values in the
 *       vector are set to zero by default.
 *
 * @param columns The number of elements (dimensions) in the vector.
 *
 * @return A pointer to the newly created vector
 */
vector_t* vector_create(const size_t columns) {
    vector_t* vector = (vector_t*) malloc(sizeof(vector_t));
    if (NULL == vector) { // If no memory was allocated
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to struct Vector.\n",
            columns);
        return NULL; // Early return if vector creation failed
    }

    vector->data = (float*) malloc(columns * sizeof(float));
    if (NULL == vector->data) { // Failed to allocate memory for elements
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to vector->data.\n",
            columns);
        free(vector); // Free allocated vector memory to prevent leaks
        return NULL;  // Early return if vector creation failed
    }

    /* After allocating vector->data
     *
     * @note memset may be optimized away (under the as-if rules) if the object
     *       modified by this function is not accessed again for the rest of
     *       its lifetime (e.g., gcc bug 8537). For this reason, we do not
     *       employ its use here.
     */
    for (size_t i = 0; i < columns; i++) {
        vector->data[i] = 0.0f;
    }

    // track the dimensions of the vector to prevent decay.
    vector->columns = columns;

    return vector;
}

/**
 * @brief Copy a given N-dimensional vector
 *
 * @note This function creates a deep copy of the input vector by allocating
 *       new memory and duplicating its contents.
 *
 * @param vector Input vector
 *
 * @return A pointer to the deep copied vector
 */
vector_t* vector_deep_copy(const vector_t* vector) {
    vector_t* deep_copy = vector_create(vector->columns);
    if (NULL == deep_copy) {
        return NULL;
    }

    for (size_t i = 0; i < vector->columns; ++i) {
        deep_copy->data[i] = vector->data[i];
    }

    return deep_copy;
}

/**
 * @brief Create a shallow copy of an N-dimensional vector
 *
 * @note This function returns a pointer to the same allocated memory as the
 *       input vector, effectively creating a reference (shallow) copy.
 *
 * @param vector Input vector
 *
 * @return A pointer to the shallow copied vector
 */
vector_t* vector_shallow_copy(const vector_t* vector) {
    if (NULL == vector) {
        return NULL;
    }

    // Allocate memory for the new Vector structure only, not for its elements
    vector_t* new_vector = (vector_t*) malloc(sizeof(vector_t));
    if (NULL == new_vector) { // If no memory was allocated
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate %zu bytes to struct Vector.\n",
            sizeof(vector_t));
        return NULL; // Early return if vector creation failed
    }

    // Copy all fields except elements (pointer to an array)
    new_vector->columns = vector->columns;

    // Assign the existing pointer to the new Vector structure
    new_vector->data = vector->data;

    return new_vector;
}

/**
 * @brief Free an allocated N-dimensional vector
 *
 * @note This function deallocates memory associated with a given vector,
 *       releasing any resources used during its creation.
 *
 * @note This function does not handle double freeing directly. To avoid
 *       double freeing, ensure pointers are set to NULL after freeing.
 *
 * @ref 7.22.3 Memory management functions on page 347
 * - <https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf>
 * @ref Rationale for setting the pointer to NULL
 * - <https://stackoverflow.com/q/34284846/15147156>
 *
 * @param vector A pointer to the vector to be freed
 */
void vector_free(vector_t* vector) {
    if (vector == NULL) {
        return;
    }

    if (vector->data) {
        free(vector->data);
    }

    free(vector);
    // Note: Setting the pointer to NULL here would only affect the local copy
}

/**
 * @brief Element-wise operations
 *
 * @note These helper functions perform basic arithmetic on two floating-point
 *       values and return the result.
 *
 * @param x First operand
 * @param y Second operand
 *
 * @return Result of the operation
 */

float scalar_add(float x, float y) {
    return x + y;
}

float scalar_subtract(float x, float y) {
    return x - y;
}

float scalar_multiply(float x, float y) {
    return x * y;
}

float scalar_divide(float x, float y) {
    if (y == 0) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Division by zero is undefined. Cannot divide x (%f) by y (%f).\n",
            x,
            y);
        return NAN; // Division by zero is undefined
    }
    return x / y;
}

/**
 * @brief Executor for element-wise vector-to-scalar functions
 *
 * @note This function applies a given operation to each corresponding pair of
 *       elements in two vectors and returns the resulting vector.
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
) {
    vector_t* c = vector_create(a->columns);
    if (NULL == c) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // Perform element-wise operation
    for (size_t i = 0; i < a->columns; i++) {
        c->data[i] = operation(a->data[i], b);
    }

    return c;
}

/**
 * @brief Add a scalar value to an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to add
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_add(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_add);
}

/**
 * @brief Subtract a scalar value from an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to subtract
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_subtract(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_subtract);
}

/**
 * @brief Multiply a scalar value with an N-dimensional vector
 *
 * @param a Input vector
 * @param b Scalar value to multiply
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_multiply(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_multiply);
}

/**
 * @brief Divide an N-dimensional vector by a scalar value
 *
 * @param a Input vector
 * @param b Scalar value to divide by
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_scalar_divide(const vector_t* a, const float b) {
    return vector_scalar_elementwise_operation(a, b, scalar_divide);
}

/**
 * @brief Executor for element-wise vector-to-vector functions
 *
 * @note This function applies a given operation to each corresponding pair of
 *       elements in two vectors and returns the resulting vector.
 *
 * @param a First input vector
 * @param b Second input vector
 * @param operation A pointer to the function performing the element-wise
 *                  operation
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_elementwise_operation(
    const vector_t* a, const vector_t* b, float (*operation)(float, float)
) {
    if (a->columns != b->columns) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->columns,
            b->columns);
        return NULL;
    }

    vector_t* c = vector_create(a->columns);
    if (NULL == c) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // Perform element-wise operation
    for (size_t i = 0; i < a->columns; i++) {
        c->data[i] = operation(a->data[i], b->data[i]);
    }

    return c;
}

/**
 * @brief Add two N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_add(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_add);
}

/**
 * @brief Subtract one N-dimensional vector from another
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_subtract(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_subtract);
}

/**
 * @brief Multiply two N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_multiply(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_multiply);
}

/**
 * @brief Divide an N-dimensional vector by another N-dimensional vector
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return A pointer to the resulting vector
 */
vector_t* vector_vector_divide(const vector_t* a, const vector_t* b) {
    return vector_vector_elementwise_operation(a, b, scalar_divide);
}

/**
 * @brief Common vector operations
 */

/**
 * @brief Determine the magnitude or length of an N-dimensional vector
 *
 * @param vector Input vector
 *
 * @return The magnitude of the vector
 */
float vector_magnitude(const vector_t* vector) {
    float sum = 0;

    // sum the square of the elements for n-dimensional vectors
    for (size_t i = 0; i < vector->columns; i++) {
        sum += vector->data[i] * vector->data[i];
    }

    return sqrt(sum);
}

/**
 * @brief Calculate the distance between two given N-dimensional vectors
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return The distance between the two vectors
 */
float vector_distance(const vector_t* a, const vector_t* b) {
    float distance_squared = 0.0f;

    if (a->columns != b->columns) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->columns,
            b->columns);
        return NAN;
    }

    for (size_t i = 0; i < a->columns; ++i) {
        distance_squared
            += (a->data[i] - b->data[i]) * (a->data[i] - b->data[i]);
    }

    return sqrtf(distance_squared);
}

/**
 * @brief Calculate the mean of an N-dimensional vector
 *
 * @note This function estimates the mean by weighted averaging.
 *
 * m(n) = [x(1) + x(2) + ... + x(n)] / n
 *
 * @param vector Input vector
 *
 * @return The mean of the vector
 */
float vector_mean(const vector_t* vector) {
    if (NULL == vector || 0 == vector->columns) {
        return NAN; // Return NAN for invalid input
    }

    float sum = 0.0f;
    for (size_t i = 0; i < vector->columns; i++) {
        if (isnan(vector->data[i])) {
            // Log error and return NAN if any element is NaN
            LOG(&global_logger,
                LOG_LEVEL_ERROR,
                "NaN element found at index %zu.\n",
                i);
            return NAN;
        }
        sum += vector->data[i];
    }

    return sum / vector->columns; // Return the mean
}

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
 *
 */
float vector_low_pass_filter(const vector_t* vector, float alpha) {
    LOG(&global_logger,
        LOG_LEVEL_ERROR,
        "Vector low pass filter is not currently implemented.\n");
    return -1.0f;
}

/**
 * @brief Normalize a given N-dimensional vector in place
 *
 * @param vector Input vector
 * @param inplace Boolean flag indicating whether to modify the input vector or
 *                return a new vector
 *
 * @return A pointer to the normalized vector
 */
vector_t* vector_normalize(vector_t* vector, bool inplace) {
    float magnitude = vector_magnitude(vector);

    if (0 == magnitude) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Cannot normalize a zero-length vector.\n");
        return NULL;
    }

    if (inplace) {
        // scale the elements down by the magnitude to produce a unit vector
        for (size_t i = 0; i < vector->columns; i++) {
            vector->data[i] /= magnitude;
        }

        return vector;
    }

    vector_t* unit = vector_create(vector->columns);
    if (NULL == unit) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the normalized unit vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->columns; i++) {
        unit->data[i] = vector->data[i] / magnitude;
    }

    return unit;
}

/**
 * @brief Scale an N-dimensional vector by the specified factor
 *
 * @param vector Input vector
 * @param scalar Scaling factor
 * @param inplace Boolean flag indicating whether to modify the input vector or
 *                return a new vector
 *
 * @return A pointer to the scaled vector
 */
vector_t* vector_scale(vector_t* vector, float scalar, bool inplace) {
    if (vector == NULL) {
        return NULL;
    }

    if (inplace) { // block out-of-place vector scaling if in-place is true
        for (size_t i = 0; i < vector->columns; ++i) {
            vector->data[i] *= scalar; // scale the vector in-place
        }
        return vector; // return the scaled vector
    }

    // perform out-of-place vector scaling
    vector_t* scaled_vector = vector_create(vector->columns);
    if (scaled_vector == NULL) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for scaled vector.\n");
        return NULL;
    }

    for (size_t i = 0; i < vector->columns; ++i) {
        scaled_vector->data[i] = vector->data[i] * scalar;
    }

    return scaled_vector;
}

/**
 * @brief Clip an N-dimensional vector within a given range
 *
 * @param vector Input vector
 * @param min Minimum value for clipping
 * @param max Maximum value for clipping
 * @param inplace Boolean flag indicating whether to modify the input vector or
 *                return a new vector
 *
 * @return A pointer to the clipped vector
 */
vector_t* vector_clip(vector_t* vector, float min, float max, bool inplace) {
    if (NULL == vector || 0 == vector->columns) {
        return NULL;
    }

    if (inplace) {
        for (size_t i = 0; i < vector->columns; i++) {
            if (vector->data[i] < min) {
                vector->data[i] = min;
            }
            if (vector->data[i] > max) {
                vector->data[i] = max;
            }
        }

        return vector; // return as soon as possible
    }

    // create a vector if !inplace
    vector_t* clipped_vector = vector_create(vector->columns);
    if (NULL == clipped_vector) {
        return NULL; // NOTE: we can return and not log because vector_create
                     // logs the error for us
    }

    for (size_t i = 0; i < vector->columns; i++) {
        if (vector->data[i] < min) {
            clipped_vector->data[i] = min;
        } else if (vector->data[i] > max) {
            clipped_vector->data[i] = max;
        } else {
            clipped_vector->data[i] = vector->data[i];
        }
    }

    // Return the newly created clipped vector
    return clipped_vector;
}

/**
 * @brief Special vector operations
 */

/**
 * @brief Calculate the dot product of two N-dimensional vectors
 *
 * @note Dot product is n-dimensional
 *
 * @param a First input vector
 * @param b Second input vector
 *
 * @return The dot product of the two vectors
 */
float vector_dot_product(const vector_t* a, const vector_t* b) {
    if (a->columns != b->columns) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->columns,
            b->columns);
        return NAN;
    }

    float product = 0.0f;

    for (size_t i = 0; i < a->columns; i++) {
        product += a->data[i] * b->data[i];
    }

    return product;
}

/**
 * @brief Return the cross product of two 3D vectors
 *
 * @note Cross product is 3-dimensional
 *
 * @param a First input vector (3D vector)
 * @param b Second input vector (3D vector)
 *
 * @return A pointer to the cross product vector
 */
vector_t* vector_cross_product(const vector_t* a, const vector_t* b) {
    // Ensure both vectors are 3-dimensional.
    if (a->columns != 3 || b->columns != 3) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Cross product is only defined for 3-dimensional vectors.\n");
        return NULL;
    }

    vector_t* result = vector_create(3);
    if (result == NULL) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for cross product vector.\n");
        return NULL;
    }

    // Calculate the components of the cross product vector.
    result->data[0] = a->data[1] * b->data[2] - a->data[2] * b->data[1];
    result->data[1] = a->data[2] * b->data[0] - a->data[0] * b->data[2];
    result->data[2] = a->data[0] * b->data[1] - a->data[1] * b->data[0];

    return result;
}

/**
 * @brief Special coordinates
 */

/**
 * @brief Convert polar coordinates to cartesian coordinates
 *
 * @note Polar coordinates are defined as the ordered pair (r, θ) names a point
 *       r units from origin along the terminal side of angle θ in standard
 *       position (origin to elements).
 *
 * x = r cos θ and y = r sin θ
 *
 * @param polar_vector Input vector in polar coordinates
 *
 * @return A pointer to the vector in cartesian coordinates
 */
vector_t* vector_polar_to_cartesian(const vector_t* polar_vector) {
    if (NULL == polar_vector || polar_vector->columns != 2) {
        return NULL; // Return NULL if input is invalid
    }

    vector_t* cartesian_vector = vector_create(2);
    if (NULL == cartesian_vector) {
        return NULL; // Return NULL if memory allocation fails
    }

    // radii/radius/ray all seem equivalently apropos
    // perhaps ray is best suited?
    float r     = polar_vector->data[0];
    float theta = polar_vector->data[1];

    cartesian_vector->data[0] = r * cosf(theta); // x = r * cos(θ)
    cartesian_vector->data[1] = r * sinf(theta); // y = r * sin(θ)

    return cartesian_vector;
}

/**
 * @brief Convert cartesian coordinates to polar coordinates
 *
 * @note The derivation between polar and cartesian coordinates is to consider
 *       a point P with the rectangular (x, y) and polar (r, θ) coordinates.
 *
 * r = ± √(x^2 + y^2) and tan θ = y / x
 *
 * @param cartesian_vector Input vector in cartesian coordinates
 *
 * @return A pointer to the vector in polar coordinates
 */
vector_t* vector_cartesian_to_polar(const vector_t* cartesian_vector) {
    if (NULL == cartesian_vector || cartesian_vector->columns != 2) {
        return NULL; // Return NULL if input is invalid
    }

    vector_t* polar_vector = vector_create(2);
    if (NULL == polar_vector) {
        return NULL; // Return NULL if memory allocation fails
    }

    float x = cartesian_vector->data[0];
    float y = cartesian_vector->data[1];

    polar_vector->data[0] = sqrtf(x * x + y * y); // r = √(x^2 + y^2)
    polar_vector->data[1] = atan2f(y, x);         // θ = atan (y, x)

    return polar_vector;
}
