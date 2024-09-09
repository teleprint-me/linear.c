/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/thread.h
 *
 * @brief A simple and easy-to-use Thread Pool API in C
 *
 * This header file provides functions for creating, managing, and freeing a
 * thread pool to perform mathematical operations on N-dimensional tensors in
 * parallel using pthreads library.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 *
 * @note linear_data_t is reserved for signal processing.
 * @ref See teleprint-me/fixed-point for more information
 * https://github.com/teleprint-me/fixed-point.git
 *
 * @note The thread could should always be configurable.
 * @ref See GNU C Extensions for more information
 * https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/C-Extensions.html
 */

#ifndef LINEAR_THREAD_H
#define LINEAR_THREAD_H

#include "vector.h"

#include <pthread.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

// Define the maximum number of threads if not provided
#ifndef LINEAR_THREAD_COUNT
    #ifdef __GNUC__
        #define LINEAR_THREAD_COUNT get_nprocs_conf()
    #elif
        #define LINEAR_THREAD_COUNT 8
    #endif // __GNUC__
#endif     // LINEAR_THREAD_COUNT

// Define the types of Linear structures
typedef enum LinearStructure {
    LINEAR_VECTOR, ///< vector_t
    LINEAR_MATRIX, ///< matrix_t
    LINEAR_TENSOR  ///< tensor_t
} linear_struct_t;

// Define the thread structure
typedef struct LinearThread {
    void*    a; ///< Pointer to the first operand (vector, matrix, or tensor).
    void*    b; ///< Pointer to the second operand (vector, matrix, or tensor).
    void*    result;      ///< Pointer to the result data structure.
    uint32_t begin;       ///< Starting index for the thread to operate.
    uint32_t end;         ///< Ending index for the thread to operate.
    linear_struct_t type; ///< Type of struct (e.g., vector, matrix, tensor).
    float (*operation)(float, float); ///< Pointer to the operation function.
} linear_thread_t;

// Function prototypes
linear_thread_t* linear_thread_create(uint32_t num_threads);
void             linear_thread_free(linear_thread_t* thread);

// Initialize a thread pool (optional)
void linear_thread_pool_create(uint32_t num_threads);
void linear_thread_pool_free(void);

// Function to perform an operation in parallel
void thread_parallel_operation(linear_thread_t* thread);

#endif // LINEAR_THREAD_H
