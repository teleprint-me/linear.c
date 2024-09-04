/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/thread.c
 *
 * @brief A simple and easy-to-use Thread API in C
 *
 * This header file provides an implementation for basic mathematical
 * operations on N-dimensional tensors in N-dimensional space within a threaded
 * pool.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 */

#include "thread.h"

void* thread_worker(void* data) {
    linear_thread_t* thread = (linear_thread_t*) data;
    for (size_t i = thread->begin; i < thread->end; ++i) {
        thread->result->data[i]
            = thread->operation(thread->a->data[i], thread->b->data[i]);
    }
    return NULL;
}

void vector_parallel_operation(linear_thread_t* thread) {
    size_t        num_threads = 4; // You can adjust the number of threads
    pthread_t     threads[num_threads];
    thread_data_t thread_data[num_threads];

    size_t chunk_size = vec1->dimensions / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        thread_data[i].vec1
            = (vector_t*) vec1; // Cast away const for simplicity
        thread_data[i].vec2   = (vector_t*) vec2;
        thread_data[i].result = result;
        thread_data[i].start  = i * chunk_size;
        thread_data[i].end
            = (i == num_threads - 1) ? vec1->dimensions : (i + 1) * chunk_size;
        thread_data[i].operation = operation;

        pthread_create(&threads[i], NULL, thread_worker, &thread_data[i]);
    }

    for (size_t i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}
