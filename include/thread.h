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

// There are 3 types of structures:
//     vector_t, matrix_t, and tensor_t
// We need to avoid potential future conflicts with signal processing
// and disassociate the name schema from referencing data types to
// mitigate potential confusion. Declaring this as some vague linear
// struct type is probably not any better, but I don't know what else
// to name this at the current moment and this is better than nothing.
// Consider whether this is an appropriate naming schema and tread carefully.
typedef enum LinearStructure {
    LINEAR_VECTOR,
    LINEAR_MATRIX,
    LINEAR_TENSOR
} linear_struct_t;

// @note Don't know what data types are being used in advance
typedef struct {
    void*  a; ///< Pointer to the first operand (vector, matrix, or tensor).
    void*  b; ///< Pointer to the second operand (vector, matrix, or tensor).
    void*  result;        ///< Pointer to the result data structure.
    size_t begin;         ///< Starting index for the thread to operate.
    size_t end;           ///< Ending index for the thread to operate.
    linear_struct_t type; ///< Type of struct (e.g. vector, matrix, tensor).
    float (*operation)(float, float); ///< Pointer to the operation function.
} linear_thread_t;

// @note Could pass the number of threads to the structure?
//       Doesn't feel right, though.
linear_thread_t* linear_thread_create(/* todo: void for now */ void);
void             linear_thread_free(linear_thread_t* thread);

// Initialize a thread pool (optional)
void linear_thread_pool_create(size_t num_threads);
void linear_thread_pool_free(void);

// Function to perform an operation in parallel
void thread_parallel_operation(linear_thread_t* thread);

#endif // LINEAR_THREAD_H
