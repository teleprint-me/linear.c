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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "numeric_types.h"
#include "scalar.h"

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Define the maximum number of threads if not provided
 *
 * @param LINEAR_THREAD_COUNT The number of CPU threads to utilize
 *
 * @note The thread count should always be configurable.
 * @note `get_nprocs_conf()` is GNU-specific and not available if GNU C is
 * unavailable. Testing for __linux__ becomes moot as a result.
 *
 * @ref See GNU C Extensions for more information
 * - https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/C-Extensions.html
 */
#ifndef LINEAR_THREAD_COUNT
    #ifdef __GNUC__ // GNU C Only
        #include <sys/sysinfo.h>
        #define LINEAR_THREAD_COUNT get_nprocs_conf()
    #elif _POSIX_VERSION // POSIX Only
        #include <unistd.h>
        #define LINEAR_THREAD_COUNT sysconf(_SC_NPROCESSORS_ONLN)
    #else // Unknown/Unsupported
        #define LINEAR_THREAD_COUNT 8
    #endif // __GNUC__
#endif     // LINEAR_THREAD_COUNT

/**
 * @brief Define the linear device type
 *
 * Data structure representing supported physical backend devices used for
 * intensive parallel processing.
 *
 * @param BACKEND_CPU Enable POSIX (CPU multi-threading)
 * @param BACKEND_VULKAN Enable Vulkan (GPU parallel-processing)
 *
 * @note Supported backends must be vendor agnostic.
 */
typedef enum ThreadBackend {
    BACKEND_CPU,    ///< Enable POSIX (CPU multi-threading)
    BACKEND_VULKAN, ///< Enable Vulkan (GPU parallel-processing)
    BACKEND_COUNT   ///< Number of supported devices
} thread_backend_t;

/**
 * @brief Generalized thread structure using void pointers
 *
 * @param a Pointer to the first operand
 * @param b Pointer to the second operand
 * @param result Pointer to the resultant data
 * @param begin Starting index for the thread to operate
 * @param end Ending index for the thread to operate
 * @param type The data type for the operation
 * @param operation Pointer to the generalized operation function
 */
typedef struct ThreadData {
    void*              a;         ///< Pointer to the first operand
    void*              b;         ///< Pointer to the second operand
    void*              result;    ///< Pointer to the resultant data
    uint32_t           begin;     ///< Threads starting index
    uint32_t           end;       ///< Threads ending index
    numeric_data_t     type;      ///< The operations data type
    scalar_operation_t operation; ///< Pointer to the operation function
} thread_data_t;

/**
 * @brief Thread pool structure
 *
 * Manages a pool of threads and tasks.
 *
 * @param threads Array of threads
 * @param num_threads Number of threads in the pool
 * @param task_queue Queue of tasks
 * @param queue_mutex Mutex for queue synchronization
 * @param task_cond Condition variable for task availability
 * @param queue_size Size of the task queue
 * @param task_count Current number of tasks in the queue
 * @param shutdown Flag to indicate if the pool should shutdown
 */
typedef struct ThreadPool {
    pthread_t*      threads;     ///< Array of threads
    uint32_t        num_threads; ///< Number of threads in the pool
    thread_data_t*  task_queue;  ///< Queue of tasks
    pthread_mutex_t queue_mutex; ///< Mutex for queue synchronization
    pthread_cond_t  task_cond;   ///< Condition variable for task availability
    uint32_t        queue_size;  ///< Size of the task queue
    uint32_t        task_count;  ///< Current number of tasks in the queue
    uint32_t        shutdown; ///< Flag to indicate if the pool should shutdown
} thread_pool_t;

// Function prototypes for thread pool API
thread_pool_t* thread_pool_create(uint32_t num_threads);
void           thread_pool_destroy(thread_pool_t* pool);
void           thread_pool_submit(thread_pool_t* pool, thread_data_t task);
void           thread_pool_wait(thread_pool_t* pool);

// Additional utilities and operations
thread_data_t* thread_create(uint32_t num_threads);
void           thread_free(thread_data_t* thread);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LINEAR_THREAD_H
