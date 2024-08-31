/**
 * Copyright © 2024 Austin Berrio
 *
 * @file tests/test_linear_thread.c
 *
 * @note keep fixtures and related tests as simple as reasonably possible.
 *       The simpler, the better.
 *
 * Manual Build Instructions:
 *     cmake -B build -DCMAKE_BUILD_TYPE=Debug
 *     cmake --build build --config Debug -j $(nproc)
 *     gcc -o vmt src/vector.c tests/test_linear_thread.c -lpthread -lm \
 *         -I./include -I./mods/logger/include -L./build/lib -llogger
 * Manual Run Instruction:
 *     LD_LIBRARY_PATH=build/lib/ ./vmt
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
    pthread_t id;
} thread_data_t;

/**
 * @brief Thread Function: Performs vector addition on a specific chunk of
 * data. Each thread will execute this function and handle its assigned portion
 * of the vectors.
 *
 * @param arg Pointer to a thread_data_t structure containing the data chunk
 *            and operation details.
 */
void* thread_vector_add(void* arg) {
    thread_data_t* data = (thread_data_t*) arg;
    for (size_t i = data->begin; i < data->end; ++i) {
        data->result->data[i] = scalar_add(data->a->data[i], data->b->data[i]);
    }
    return NULL;
}

int main() {
    // Hypothetical large vector sizes
    size_t columns = 1000000; // number of dimensions

    /**
     * Allocate memory
     *
     * @note vectors are zero initialized upon creation. null is returned upon
     * failure.
     */
    vector_t* a      = vector_create(columns);
    vector_t* b      = vector_create(columns);
    vector_t* result = vector_create(columns);

    // Initialize vectors with dummy data
    for (size_t i = 0; i < columns; ++i) {
        a->data[i] = (float) (i + 1);       // shift by 1
        b->data[i] = (float) ((i + 1) * 2); // shift by 1, then double
    }

    pthread_t     threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    /**
     * @brief Divide the Data into Chunks: Split the large vector into smaller
     * chunks. Each thread will handle one chunk, allowing the workload to be
     * distributed evenly.
     */
    size_t chunk_size = columns / NUM_THREADS;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].id     = i;
        thread_data[i].a      = a;
        thread_data[i].b      = b;
        thread_data[i].result = result;
        thread_data[i].begin  = i * chunk_size;
        thread_data[i].end
            = (i == NUM_THREADS - 1) ? columns : (i + 1) * chunk_size;
        pthread_create(
            &threads[i], NULL, thread_vector_add, (void*) &thread_data[i]
        );
    }

    /**
     * @brief Create a Thread Pool: Use a fixed number of threads that wait for
     * tasks to execute. This approach can help manage resources efficiently
     * without the overhead of creating and destroying threads repeatedly.
     */
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    vector_free(a);
    vector_free(b);
    vector_free(result);

    printf("Vector addition complete.\n");
    return 0;
}
