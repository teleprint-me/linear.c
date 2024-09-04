/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/vector_simple.c
 *
 * Manual Build:
 *     gcc -o examples/vector_simple \
 *         mods/logger/src/logger.c src/vector.c \
 *         examples/vector_simple.c
 *         -lpthread -lm \
 *         -I./include -I./mods/logger/include
 */

#include "vector.h"

#include <stdio.h>

#define MODULUS    2147483647
#define MULTIPLIER 48271

int lehmer_generate_modulo(int seed) {
    // Scale the seed and return the remainder.
    return (MULTIPLIER * seed) % MODULUS;
}

// NOTE: Normalization "generates" the pseudo random number
float lehmer_seed_normalize(int seed) {
    // Normalize the seed as a ratio of the modulus.
    return (float) seed / (float) MODULUS;
}

void lehmer_initialize(int seed, vector_t* vector) {
    for (size_t i = 0; i < vector->columns; i++) {
        seed            = lehmer_generate_modulo(seed);
        float n         = lehmer_seed_normalize(seed);
        vector->data[i] = n;
    }
}

int main(void) {
    int    seed    = 1337;
    size_t columns = 4;

    vector_t* a = vector_create(columns);
    vector_t* b = vector_create(columns);

    // @note Do not allocate memory to the result.
    vector_t* result;

    lehmer_initialize(seed, a);
    lehmer_initialize(seed + 1, b);

    // @note Result is automatically allocated memory
    result = vector_vector_add(a, b);

    for (size_t i = 0; i < result->columns; i++) {
        printf("result->data[%zu] = %f\n", i, result->data[i]);
    }

    vector_free(a);
    vector_free(b);
    vector_free(result);

    return 0;
}
