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
#include "thread.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lifecycle management

vector_t* vector_create(const uint32_t columns) {
    vector_t* vector = (vector_t*) malloc(sizeof(vector_t));
    if (NULL == vector) { // If no memory was allocated
        LOG_ERROR("Failed to allocate %zu bytes to struct Vector.\n", columns);
        return NULL; // Early return if vector creation failed
    }

    vector->data = (float*) malloc(columns * sizeof(float));
    if (NULL == vector->data) { // Failed to allocate memory for elements
        LOG_ERROR("Failed to allocate %zu bytes to vector->data.\n", columns);
        free(vector); // Free allocated vector memory to prevent leaks
        return NULL;  // Early return if vector creation failed
    }

    /**
     * Initialize all elements to zero
     *
     * @note memset may be optimized away (under the as-if rules) if the object
     *       modified by this function is not accessed again for the rest of
     *       its lifetime (e.g., gcc bug 8537). For this reason, we do not
     *       employ its use here.
     */
    for (uint32_t i = 0; i < columns; i++) {
        vector->data[i] = 0.0f;
    }

    // track the dimensions of the vector to prevent decay.
    vector->columns = columns;

    return vector;
}

void vector_free(vector_t* vector) {
    if (NULL == vector) {
        return;
    }

    if (vector->data) {
        free(vector->data);
    }

    free(vector);
    // Note: Setting the pointer to NULL here would only affect the local copy
}

// Initialization Operations

void vector_fill(vector_t* vector, const float value) {
    for (uint32_t i = 0; i < vector->columns; i++) {
        vector->data[i] = value;
    }
}

static void vector_lehmer_initialize(
    lehmer_state_t* state,
    vector_t*       vector,
    double (*lehmer_callback)(lehmer_state_t*)
) {
    for (uint32_t i = 0; i < vector->columns; i++) {
        // Cast from double to float, as the vector uses float values
        float n         = (float) lehmer_callback(state);
        vector->data[i] = n;
    }
}

void vector_lehmer_modulo(lehmer_state_t* state, vector_t* vector) {
    vector_lehmer_initialize(state, vector, lehmer_random_modulo);
}

void vector_lehmer_gamma(lehmer_state_t* state, vector_t* vector) {
    vector_lehmer_initialize(state, vector, lehmer_random_gamma);
}

void vector_lehmer_delta(lehmer_state_t* state, vector_t* vector) {
    vector_lehmer_initialize(state, vector, lehmer_random_delta);
}

// @todo Initialize using mersenne twister
// @note Not currently available

// Copy operations

vector_t* vector_deep_copy(const vector_t* vector) {
    vector_t* deep_copy = vector_create(vector->columns);
    if (NULL == deep_copy) {
        return NULL;
    }

    for (uint32_t i = 0; i < vector->columns; ++i) {
        deep_copy->data[i] = vector->data[i];
    }

    return deep_copy;
}

vector_t* vector_shallow_copy(const vector_t* vector) {
    if (NULL == vector) {
        return NULL;
    }

    // Allocate memory for the new Vector structure only, not for its elements
    vector_t* new_vector = (vector_t*) malloc(sizeof(vector_t));
    if (NULL == new_vector) { // If no memory was allocated
        LOG_ERROR(
            "Failed to allocate %zu bytes to struct Vector.\n",
            sizeof(vector_t)
        );
        return NULL; // Early return if vector creation failed
    }

    // Copy all fields except elements (pointer to an array)
    new_vector->columns = vector->columns;

    // Assign the existing pointer to the new Vector structure
    new_vector->data = vector->data;

    return new_vector;
}

// Scalar operations

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
        LOG_ERROR(
            "Division by zero is undefined. Cannot divide x (%f) by y (%f).\n",
            x,
            y
        );
        return NAN; // Division by zero is undefined
    }
    return x / y;
}

// Element-wise operations

// Vector-Scalar Operations

// Worker function for multi-threaded vector-scalar operation
void* vector_scalar_thread_worker(void* arg) {
    linear_thread_t* data   = (linear_thread_t*) arg;
    vector_t*        a      = (vector_t*) data->a;
    float            b      = *(float*) data->b;
    vector_t*        result = (vector_t*) data->result;

    for (uint32_t i = data->begin; i < data->end; i++) {
        result->data[i] = data->operation(a->data[i], b);
    }

    return NULL;
}

vector_t* vector_scalar_cpu_operation(
    const vector_t* a, const float b, float (*operation)(float, float)
) {
    vector_t* result = vector_create(a->columns);
    if (NULL == result) {
        LOG_ERROR("Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

// Perform multi-threaded execution or single-threaded operation
#ifdef LINEAR_THREAD
    // Initialize threads and distribute work among them
    pthread_t       threads[LINEAR_THREAD_COUNT];
    linear_thread_t thread_data[LINEAR_THREAD_COUNT];
    uint32_t        chunk_size = a->columns / LINEAR_THREAD_COUNT;

    for (uint32_t i = 0; i < LINEAR_THREAD_COUNT; i++) {
        thread_data[i].a         = (void*) a;
        thread_data[i].b         = &b; // scalar operation
        thread_data[i].result    = result;
        thread_data[i].begin     = i * chunk_size;
        thread_data[i].end       = (i + 1) * chunk_size;
        thread_data[i].operation = operation;

        pthread_create(
            &threads[i], NULL, vector_scalar_thread_worker, &thread_data[i]
        );
    }

    // Join threads
    for (uint32_t i = 0; i < LINEAR_THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
#else
    // Single-threaded fallback
    for (uint32_t i = 0; i < a->columns; i++) {
        result->data[i] = operation(a->data[i], b);
    }
#endif

    return result;
}

vector_t* vector_scalar_operation(
    const vector_t* a, const float b, float (*operation)(float, float)
) {
    vector_t* c = vector_create(a->columns);
    if (NULL == c) {
        LOG_ERROR("Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // Perform element-wise operation
    for (uint32_t i = 0; i < a->columns; i++) {
        c->data[i] = operation(a->data[i], b);
    }

    return c;
}

vector_t* vector_scalar_add(const vector_t* a, const float b) {
    return vector_scalar_operation(a, b, scalar_add);
}

vector_t* vector_scalar_subtract(const vector_t* a, const float b) {
    return vector_scalar_operation(a, b, scalar_subtract);
}

vector_t* vector_scalar_multiply(const vector_t* a, const float b) {
    return vector_scalar_operation(a, b, scalar_multiply);
}

vector_t* vector_scalar_divide(const vector_t* a, const float b) {
    return vector_scalar_operation(a, b, scalar_divide);
}

// Vector-Vector operations

void* vector_vector_thread_worker(void* arg) {
    linear_thread_t* data   = (linear_thread_t*) arg;
    vector_t*        a      = (vector_t*) data->a;
    vector_t*        b      = (vector_t*) data->b;
    vector_t*        result = (vector_t*) data->result;

    for (size_t i = data->begin; i < data->end; ++i) {
        result->data[i] = data->operation(a->data[i], b->data[i]);
    }
    return NULL;
}

vector_t* vector_vector_cpu_operation(
    const vector_t* a, const vector_t* b, float (*operation)(float, float)
) {
    vector_t* result = vector_create(a->columns);
    if (NULL == result) {
        LOG_ERROR("Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

// Perform multi-threaded execution or single-threaded operation
#ifdef LINEAR_THREAD
    // Initialize threads and distribute work among them
    pthread_t       threads[LINEAR_THREAD_COUNT];
    linear_thread_t thread_data[LINEAR_THREAD_COUNT];
    uint32_t        chunk_size = a->columns / LINEAR_THREAD_COUNT;

    for (uint32_t i = 0; i < LINEAR_THREAD_COUNT; i++) {
        thread_data[i].a         = (void*) a;
        thread_data[i].b         = (void*) b; // vector operation
        thread_data[i].result    = result;
        thread_data[i].begin     = i * chunk_size;
        thread_data[i].end       = (i + 1) * chunk_size;
        thread_data[i].operation = operation;

        pthread_create(
            &threads[i], NULL, vector_vector_thread_worker, &thread_data[i]
        );
    }

    // Join threads
    for (uint32_t i = 0; i < LINEAR_THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
#else
    // Single-threaded fallback
    for (uint32_t i = 0; i < a->columns; i++) {
        result->data[i] = operation(a->data[i], b->data[i]);
    }
#endif

    return result;
}

vector_t* vector_vector_operation(
    const vector_t* a, const vector_t* b, float (*operation)(float, float)
) {
    if (a->columns != b->columns) {
        LOG_ERROR(
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->columns,
            b->columns
        );
        return NULL;
    }

    vector_t* c = vector_create(a->columns);
    if (NULL == c) {
        LOG_ERROR("Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    // Perform element-wise operation
    for (uint32_t i = 0; i < a->columns; i++) {
        c->data[i] = operation(a->data[i], b->data[i]);
    }

    return c;
}

vector_t* vector_vector_add(const vector_t* a, const vector_t* b) {
    return vector_vector_operation(a, b, scalar_add);
}

vector_t* vector_vector_subtract(const vector_t* a, const vector_t* b) {
    return vector_vector_operation(a, b, scalar_subtract);
}

vector_t* vector_vector_multiply(const vector_t* a, const vector_t* b) {
    return vector_vector_operation(a, b, scalar_multiply);
}

vector_t* vector_vector_divide(const vector_t* a, const vector_t* b) {
    return vector_vector_operation(a, b, scalar_divide);
}

// Common vector operations

float vector_magnitude(const vector_t* vector) {
    float sum = 0;

    // sum the square of the elements for n-dimensional vectors
    for (uint32_t i = 0; i < vector->columns; i++) {
        sum += vector->data[i] * vector->data[i];
    }

    return sqrt(sum);
}

float vector_distance(const vector_t* a, const vector_t* b) {
    float distance_squared = 0.0f;

    if (a->columns != b->columns) {
        LOG_ERROR(
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->columns,
            b->columns
        );
        return NAN;
    }

    for (uint32_t i = 0; i < a->columns; ++i) {
        distance_squared
            += (a->data[i] - b->data[i]) * (a->data[i] - b->data[i]);
    }

    return sqrtf(distance_squared);
}

float vector_mean(const vector_t* vector) {
    if (NULL == vector || 0 == vector->columns) {
        return NAN; // Return NAN for invalid input
    }

    float sum = 0.0f;
    for (uint32_t i = 0; i < vector->columns; i++) {
        if (isnan(vector->data[i])) {
            // Log error and return NAN if any element is NaN
            LOG_ERROR("NaN element found at index %zu.\n", i);
            return NAN;
        }
        sum += vector->data[i];
    }

    return sum / vector->columns; // Return the mean
}

/**
 * @todo implement low-pass filter for calculating low-pass filtered mean of
 * the vector
 *
 * @brief Low pass filter on an N-dimensional vector
 *
 * @note This function estimates the mean by low-pass filtering rather than
 *       averaging.
 *
 * m(n + 1) = ((1 - α) * m(n)) + (α * x(n + 1))
 *
 * @note not sure about order of operations. need to double check.
 *
 * @param vector Input vector
 * @param alpha Smoothing factor for the low-pass filter
 */
float vector_low_pass_filter(const vector_t* vector, float alpha) {
    LOG_ERROR("Vector low pass filter is not currently implemented.\n");
    return -1.0f;
}

vector_t* vector_normalize(vector_t* vector, bool inplace) {
    float magnitude = vector_magnitude(vector);

    if (0 == magnitude) {
        LOG_ERROR("Cannot normalize a zero-length vector.\n");
        return NULL;
    }

    if (inplace) {
        // scale the elements down by the magnitude to produce a unit vector
        for (uint32_t i = 0; i < vector->columns; i++) {
            vector->data[i] /= magnitude;
        }

        return vector;
    }

    vector_t* unit = vector_create(vector->columns);
    if (NULL == unit) {
        LOG_ERROR("Failed to allocate memory for the normalized unit vector.\n"
        );
        return NULL;
    }

    for (uint32_t i = 0; i < vector->columns; i++) {
        unit->data[i] = vector->data[i] / magnitude;
    }

    return unit;
}

vector_t* vector_scale(vector_t* vector, float scalar, bool inplace) {
    if (vector == NULL) {
        return NULL;
    }

    if (inplace) { // block out-of-place vector scaling if in-place is true
        for (uint32_t i = 0; i < vector->columns; ++i) {
            vector->data[i] *= scalar; // scale the vector in-place
        }
        return vector; // return the scaled vector
    }

    // perform out-of-place vector scaling
    vector_t* scaled_vector = vector_create(vector->columns);
    if (scaled_vector == NULL) {
        LOG_ERROR("Failed to allocate memory for scaled vector.\n");
        return NULL;
    }

    for (uint32_t i = 0; i < vector->columns; ++i) {
        scaled_vector->data[i] = vector->data[i] * scalar;
    }

    return scaled_vector;
}

vector_t* vector_clip(vector_t* vector, float min, float max, bool inplace) {
    if (NULL == vector || 0 == vector->columns) {
        return NULL;
    }

    if (inplace) {
        for (uint32_t i = 0; i < vector->columns; i++) {
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

    for (uint32_t i = 0; i < vector->columns; i++) {
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

// Special vector operations

float vector_dot_product(const vector_t* a, const vector_t* b) {
    if (a->columns != b->columns) {
        LOG_ERROR(
            "Vector dimensions do not match. Cannot perform operation on "
            "vectors of size %zu and "
            "%zu.\n",
            a->columns,
            b->columns
        );
        return NAN;
    }

    float product = 0.0f;

    for (uint32_t i = 0; i < a->columns; i++) {
        product += a->data[i] * b->data[i];
    }

    return product;
}

vector_t* vector_cross_product(const vector_t* a, const vector_t* b) {
    // Ensure both vectors are 3-dimensional.
    if (a->columns != 3 || b->columns != 3) {
        LOG_ERROR("Cross product is only defined for 3-dimensional vectors.\n"
        );
        return NULL;
    }

    vector_t* result = vector_create(3);
    if (result == NULL) {
        LOG_ERROR("Failed to allocate memory for cross product vector.\n");
        return NULL;
    }

    // Calculate the components of the cross product vector.
    result->data[0] = a->data[1] * b->data[2] - a->data[2] * b->data[1];
    result->data[1] = a->data[2] * b->data[0] - a->data[0] * b->data[2];
    result->data[2] = a->data[0] * b->data[1] - a->data[1] * b->data[0];

    return result;
}

// Special coordinates

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
