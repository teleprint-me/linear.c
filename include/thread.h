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
 */

#ifndef LINEAR_THREAD_H
#define LINEAR_THREAD_H

#include "numeric_types.h"
#include "scalar.h"

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

/**
 * @brief Define the maximum number of threads if not provided
 *
 * @param LINEAR_THREAD_COUNT The number of CPU threads to utilize
 *
 * @note The thread count should always be configurable.
 *
 * @note get_nprocs_conf() is GNU specific is not available if GNU C is
 * unavailable. Testing for __linux__ becomes moot as a result.
 *
 * @ref See GNU C Extensions for more information
 * - https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/C-Extensions.html
 */
#ifndef LINEAR_THREAD_COUNT
    #ifdef __GNUC__
        #define LINEAR_THREAD_COUNT get_nprocs_conf()
    #elif
        #define LINEAR_THREAD_COUNT 8
    #endif // __GNUC__
#endif     // LINEAR_THREAD_COUNT

/**
 * @brief Define the linear device type
 *
 * @param BACKEND_CPU Enable POSIX multi-threading
 * @param BACKEND_GPU Enable Vulkan parallel processing
 *
 * @note All code must be completely portable. Utilized GPU backends must
 * support compute shaders and must be vendor agnostic. I plan to support
 * Vulkan in isolation as it abstracts vendor specific implementations.
 */
typedef enum LinearBackend {
    BACKEND_CPU, ///< CPU Backend (multi-threading)
    BACKEND_GPU  ///< GPU Backend OpenGL/Vulkan-only
} linear_backend_t;

/**
 * @brief Generalized thread structure using void pointers
 */
typedef struct LinearThread {
    void*          a;      ///< Pointer to the first operand.
    void*          b;      ///< Pointer to the second operand.
    void*          result; ///< Pointer to the resultant data.
    uint32_t       begin;  ///< Starting index for the thread to operate.
    uint32_t       end;    ///< Ending index for the thread to operate.
    numeric_data_t type;   ///< The data type for the operation
    scalar_operation_t
        operation; ///< Pointer to the generalized operation function.
} linear_thread_t;

// @todo Pinned: Add proper support for thread pooling

// linear_thread_t* linear_thread_create(uint32_t num_threads);
// void             linear_thread_free(linear_thread_t* thread);

// Initialize a thread pool (optional)
// void linear_thread_pool_create(uint32_t num_threads);
// void linear_thread_pool_free(void);

// Function to perform an operation in parallel
// void thread_parallel_operation(linear_thread_t* thread);

#endif // LINEAR_THREAD_H
