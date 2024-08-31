/**
 * Copyright © 2024 Austin Berrio
 *
 * @file tests/test_linear_thread.c
 *
 * @note keep fixtures and related tests as simple as reasonably possible.
 *       The simpler, the better.
 */

#include "vector.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4 // Number of threads to use

// Structure to pass data to threads
typedef struct {
    vector_t* a;
    vector_t* b;
    vector_t* result;
    size_t    begin;
    size_t    end;
} thread_data_t;

// Function for thread to execute
void* thread_vector_add(void* arg) {
    thread_data_t* data = (thread_data_t*) arg;
    for (size_t i = data->begin; i < data->end; ++i) {
        data->result->data[i] = data->a->data[i] + data->b->data[i];
    }
    return NULL;
}

int main() {
    // Hypothetical large vector sizes
    size_t   columns = 1000000; // number of dimensions
    vector_t a, b, result;
    a.columns = b.columns = result.columns = columns;

    // Allocate memory
    a.data      = (float*) malloc(columns * sizeof(float));
    b.data      = (float*) malloc(columns * sizeof(float));
    result.data = (float*) malloc(columns * sizeof(float));

    // Initialize vectors with dummy data
    for (size_t i = 0; i < columns; ++i) {
        a.data[i] = (float) i;
        b.data[i] = (float) (i * 2);
    }

    pthread_t     threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    size_t        chunk_size = columns / NUM_THREADS;

    // Create threads
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].a      = &a;
        thread_data[i].b      = &b;
        thread_data[i].result = &result;
        thread_data[i].begin  = i * chunk_size;
        thread_data[i].end
            = (i == NUM_THREADS - 1) ? columns : (i + 1) * chunk_size;

        pthread_create(
            &threads[i], NULL, thread_vector_add, (void*) &thread_data[i]
        );
    }

    // Wait for threads to complete
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    free(a.data);
    free(b.data);
    free(result.data);

    printf("Vector addition complete.\n");
    return 0;
}
