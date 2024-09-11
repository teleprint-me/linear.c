/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/thread.c
 *
 * @brief A simple and easy-to-use Thread Pool API in C
 *
 * This header file provides functions for creating, managing, and freeing a
 * thread pool to perform mathematical operations on N-dimensional tensors in
 * parallel using pthreads library.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include "thread.h"
#include "logger.h"

// @todo

// Function prototypes for thread pool API
thread_pool_t* thread_pool_create(uint32_t num_threads) {
    LOG_ERROR("Not implemented.\n");
    return NULL;
}

void thread_pool_destroy(thread_pool_t* pool) {
    return;
}

void thread_pool_submit(thread_pool_t* pool, thread_data_t task) {
    return;
}

void thread_pool_wait(thread_pool_t* pool) {
    return;
}

// @note These may be API specific, though, in most cases, there are only a few
// minute differences. What may be apparent for one operation may not be for
// another. e.g. vector-to-scalar and vector-to-vector operations will differ
// slightly. matrix operations introduce a new dimension and added operation,
// e.g. matrix-to-scalar, matrix-to-vector, and matrix-to-matrix operations.
// this doesn't account for the tensor implementation which only has a drafted
// api.

// Additional utilities and operations
thread_data_t* thread_create(uint32_t num_threads) {
    LOG_ERROR("Not implemented.\n");
    return NULL;
}

void thread_free(thread_data_t* thread) {
    return;
}
