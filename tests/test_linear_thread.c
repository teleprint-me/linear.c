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

#include "logger.h"
#include "vector.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

// Structure to pass data to threads
typedef struct {
    const vector_t* a;
    const vector_t* b;
    vector_t*       result;
    size_t          begin;
    size_t          end;
} thread_data_t;

/**
 * @brief Thread Function: Performs vector addition on a specific chunk of
 * data. Each thread will execute this function and handle its assigned portion
 * of the vectors.
 *
 * @param arg Pointer to a thread_data_t structure containing the data chunk
 *            and operation details.
 */
void* thread_vector_add_mt(void* arg) {
    thread_data_t* data = (thread_data_t*) arg;
    for (size_t i = data->begin; i < data->end; ++i) {
        data->result->data[i] = scalar_add(data->a->data[i], data->b->data[i]);
    }
    return NULL;
}

vector_t* vector_vector_add_mt(
    const vector_t* a, const vector_t* b, size_t num_threads
) {
    if (a->columns != b->columns) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Vector dimensions do not match. Cannot perform addition on "
            "vectors of size %zu and %zu.\n",
            a->columns,
            b->columns);
        return NULL;
    }

    vector_t* result = vector_create(a->columns);
    if (NULL == result) {
        LOG(&global_logger,
            LOG_LEVEL_ERROR,
            "Failed to allocate memory for the resultant vector.\n");
        return NULL;
    }

    pthread_t     threads[num_threads];
    thread_data_t thread_data[num_threads];

    /**
     * @brief Divide the Data into Chunks
     *
     * Split the large vector into smaller chunks. Each thread will handle one
     * chunk, allowing the workload to be distributed evenly.
     */
    size_t chunk_size = a->columns / num_threads;

    // Create threads to perform addition
    for (size_t i = 0; i < num_threads; ++i) {
        thread_data[i].a      = a;
        thread_data[i].b      = b;
        thread_data[i].result = result;
        thread_data[i].begin  = i * chunk_size;
        thread_data[i].end
            = (i == num_threads - 1) ? a->columns : (i + 1) * chunk_size;

        pthread_create(
            &threads[i], NULL, thread_vector_add_mt, (void*) &thread_data[i]
        );
    }

    /**
     * @brief Join threads after execution
     *
     * Create a Thread Pool: Use a fixed number of threads that wait for
     * tasks to execute. This approach can help manage resources efficiently
     * without the overhead of creating and destroying threads repeatedly.
     */
    for (size_t i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    return result;
}

int main() {
    printf("Loading...\n");

    // @note See `man 3 get_nprocs` for more information
    int available_threads = get_nprocs();
    printf("Using %d threads...\n", available_threads);

    /**
     * @brief Hypothetical large vector sizes
     *
     * @note A large number of dimensions consumes a large amount of RAM in
     * kind. For example, one-billion elements will allocate roughly 7GB of
     * RAM. Floats are about 4 bytes, plus size_t which is unsigned long, plus
     * the structure itself. So, (4 * 1,000,000,000) + 4 + 4, if my math is
     * right. Loading time increases, but computation is parallelized, speeding
     * up computation as a result.
     *
     * $ python
     * >>> (4 * 1_000_000_000) + 4 + 4 # raw bytes
     * 4000000008
     * >>> ((4 * 1_000_000_000) + 4 + 4) / 1024 # kilobytes
     * 3906250.0078125
     * >>> ((4 * 1_000_000_000) + 4 + 4) / 1024 / 1024 # megabytes
     * 3814.6972732543945
     * >>> ((4 * 1_000_000_000) + 4 + 4) / 1024 / 1024 / 1024 # gigabytes
     * 3.7252903059124947
     *
     * Yeah, my math is off here. RAM usage maxes out at about 11.2 GB
     */
    size_t columns = 1000000000; // number of dimensions

    /**
     * Allocate memory
     *
     * @note vectors are zero initialized upon creation. NULL is returned upon
     * failure.
     */
    vector_t* a = vector_create(columns);
    vector_t* b = vector_create(columns);

    printf("Initializing...\n");
    printf("vector is a size of %d bytes\n", sizeof(a));
    printf("vector->data is a size of %d bytes\n", sizeof(a->data));
    printf("vector->columns is a size of %d bytes\n", sizeof(a->columns));

    /**
     * @brief Initialize vectors with dummy data
     *
     * @note This is in linear time, so time complexity grows alongside the
     * input size.
     */
    for (size_t i = 0; i < columns; ++i) {
        float shift = i + 1;               // calculate once, uses less RAM
        a->data[i]  = (float) shift;       // shift by 1
        b->data[i]  = (float) (shift * 2); // shift by 1, then double
    }

    printf("Computing...\n");
    vector_t* result = vector_vector_add_mt(a, b, available_threads);

    // Cleanup
    vector_free(a);
    vector_free(b);
    vector_free(result);

    printf("Vector addition complete.\n");
    return 0;
}
