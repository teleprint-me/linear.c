/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/thread.h
 *
 * @brief A simple and easy-to-use Thread API in C
 *
 * This header file provides an implementation for basic mathematical
 * operations on N-dimensional tensors in N-dimensional space within a threaded
 * pool.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#ifndef LINEAR_THREAD_H
#define LINEAR_THREAD_H

#include "vector.h"

#include <pthread.h>
#include <stdlib.h>

#ifndef LINEAR_THREAD_COUNT
    #define LINEAR_THREAD_COUNT 4
#endif // LINEAR_THREAD_COUNT

typedef enum LinearData {
    TYPE_VECTOR,
    TYPE_MATRIX,
    TYPE_TENSOR
} linear_data_t;

// @note Don't know what data types are being used in advance
typedef struct {
    void*  a; ///< Pointer to the first operand (vector, matrix, or tensor).
    void*  b; ///< Pointer to the second operand (vector, matrix, or tensor).
    void*  result;      ///< Pointer to the result data structure.
    size_t begin;       ///< Starting index for the thread to operate.
    size_t end;         ///< Ending index for the thread to operate.
    linear_data_t type; ///< Type of data (vector, matrix, tensor).
    float (*operation)(float, float); ///< Pointer to the operation function.
} linear_thread_t;

// @note Could pass the number of threads to the structure?
//       Doesn't feel right, though.
linear_thread_t* linear_thread_create();
void             linear_thread_free(linear_thread_t* thread);

// Initialize a thread pool (optional)
void linear_thread_pool_create(size_t num_threads);
void linear_thread_pool_free(void);

// Function to perform an operation in parallel
void thread_parallel_operation(linear_thread_t* thread);

#endif // LINEAR_THREAD_H
