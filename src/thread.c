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

#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>
#include <sys/stat.h>

// Function to initialize the thread pool
thread_pool_t* thread_pool_create(uint32_t num_threads) {
    thread_pool_t* pool = malloc(sizeof(thread_pool_t));
    if (!pool) {
        LOG_ERROR("Failed to allocate memory for thread pool.\n");
        return NULL;
    }

    pool->thread_count = (num_threads) ? num_threads : LINEAR_THREAD_COUNT;
    pool->queue_size   = LINEAR_THREAD_COUNT;
    pool->task_count   = 0;
    pool->head         = 0;
    pool->tail         = 0;
    pool->stop         = 0;

    pool->threads    = malloc(sizeof(pthread_t) * pool->thread_count);
    pool->task_queue = malloc(sizeof(thread_data_t) * pool->queue_size);

    if (!pool->threads || !pool->task_queue) {
        LOG_ERROR("Failed to allocate memory for threads or task queue.\n");
        free(pool->threads);
        free(pool->task_queue);
        free(pool);
        return NULL;
    }

    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->task_available, NULL);
    pthread_cond_init(&pool->task_done, NULL);

    // Create worker threads
    for (uint32_t i = 0; i < pool->thread_count; ++i) {
        int thread_status = pthread_create(
            &pool->threads[i], NULL, worker_thread, (void*) pool
        );

        if (0 != thread_status) {
            LOG_ERROR("Failed to create thread %u.\n", i);
            thread_pool_free(pool);
            return NULL;
        }
    }

    return pool;
}

// Function to destroy the thread pool
void thread_pool_free(thread_pool_t* pool) {
    if (NULL == pool) {
        return;
    }

    pthread_mutex_lock(&pool->queue_mutex);
    pool->stop = 1;
    pthread_cond_broadcast(&pool->task_available);
    pthread_mutex_unlock(&pool->queue_mutex);

    for (uint32_t i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->task_queue);
    pthread_mutex_destroy(&pool->queue_mutex);
    pthread_cond_destroy(&pool->task_available);
    pthread_cond_destroy(&pool->task_done);
    free(pool);
}

// Worker thread function
void* worker_thread(void* arg) {
    thread_pool_t* pool = (thread_pool_t*) arg;

    while (1) {
        pthread_mutex_lock(&pool->queue_mutex);
        while (pool->task_count == 0 && !pool->stop) {
            pthread_cond_wait(&pool->task_available, &pool->queue_mutex);
        }

        if (pool->stop) {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }

        thread_data_t task = pool->task_queue[pool->head];
        pool->head         = (pool->head + 1) % pool->queue_size;
        pool->task_count--;

        pthread_cond_signal(&pool->task_done);
        pthread_mutex_unlock(&pool->queue_mutex);

        // Perform the task
        task.operation(task.a, task.b, task.result, task.type);
    }

    return NULL;
}

// Submit a task to the thread pool
void thread_pool_submit(thread_pool_t* pool, thread_data_t task) {
    pthread_mutex_lock(&pool->queue_mutex);

    pool->task_queue[pool->tail] = task;
    pool->tail                   = (pool->tail + 1) % pool->queue_size;
    pool->task_count++;

    pthread_cond_signal(&pool->task_available);
    pthread_mutex_unlock(&pool->queue_mutex);
}

// Wait for all tasks to complete
void thread_pool_wait(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->queue_mutex);
    while (pool->task_count > 0) {
        pthread_cond_wait(&pool->task_done, &pool->queue_mutex);
    }
    pthread_mutex_unlock(&pool->queue_mutex);
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
