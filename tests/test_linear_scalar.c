/**
 * Copyright © 2024 Austin Berrio
 *
 * @file tests/test_linear_scalar.c
 *
 * @note keep fixtures and related tests as simple as reasonably possible.
 *       The simpler, the better.
 */

#include "logger.h"
#include "numeric_types.h"
#include "scalar.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Prototypes
 */

int test_scalar_add(void);

/**
 * @brief Unit Tests
 */

/**
 * @brief Test the correctness of scalar addition
 */
int test_scalar_add(void) {
    int pass = 0; // assume success

    float          a      = 4.5f;
    float          b      = 2.5f;
    float          result = 0.0f;
    numeric_data_t type   = NUMERIC_FLOAT32;

    result = *(float*) scalar_add(&a, &b, &result, type);

    if (0.0f == result) {
        LOG_ERROR("Failed to resolve result.\n");
        pass = 1;
    } else {
        assert(7.0f == result);
    }

    printf("%s", 0 == pass ? "." : "x");
    return pass;
}

int main(void) {
    int pass = 1; // assume failure
    pass     = test_scalar_add();
    printf("\n");
    return pass;
}
