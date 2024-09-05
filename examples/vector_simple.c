/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/vector_simple.c
 *
 * Manual Build:
 *     # binary output
 *     gcc -o examples/vector_simple \
 *         # dependencies
 *         mods/logger/src/logger.c \
 *         mods/logger/src/lehmer.c \
 *         # source
 *         src/vector.c \
 *         # this file
 *         examples/vector_simple.c
 *         # link libraries
 *         -lpthread -lm \
 *         # required include paths
 *         -I./include \
 *         -I./mods/logger/include \
 *         -I./mods/lehmer/include
 */

#include "lehmer.h"
#include "vector.h"

#include <stdio.h>

// @brief initialize a vector with randomly generated values
void vector_initialize(lehmer_state_t* state, vector_t* vector) {
    for (size_t i = 0; i < vector->columns; i++) {
        // typecast from double to float because lehmer returns double
        float n         = (float) lehmer_random_modulo(state);
        vector->data[i] = n; // this must be a float value
    }
}

int main(void) {
    // initialize the rng state
    size_t          streams = 2;
    int64_t         seed    = 1337;
    lehmer_state_t* state   = lehmer_state_create(streams, seed);

    // allocate memory to vectors
    size_t    columns = 4;
    vector_t* a       = vector_create(columns);
    vector_t* b       = vector_create(columns);

    // @note Do not allocate memory to the result.
    vector_t* result; // ops allocate memory for us

    // initialize vector a
    lehmer_state_select(state, 0); // use primary stream
    vector_initialize(state, a);

    // initialize vector b
    lehmer_state_select(state, 1); // use secondary stream
    vector_initialize(state, b);

    // @note Result is automatically allocated memory
    result = vector_vector_add(a, b);

    // output the results for vector addition
    for (size_t i = 0; i < result->columns; i++) {
        printf("result->data[%zu] = %f\n", i, result->data[i]);
    }

    // free all vectors
    vector_free(a);
    vector_free(b);
    vector_free(result);

    // free the rng state
    lehmer_state_free(state);

    return 0;
}
