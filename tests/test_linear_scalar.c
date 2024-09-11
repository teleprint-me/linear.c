/**
 * Copyright © 2024 Austin Berrio
 *
 * @file tests/test_linear_scalar.c
 *
 * @note Keep fixtures and tests as simple as reasonably possible.
 *       The simpler, the better.
 *
 * Manual Build:
 *     gcc -o test_linear_scalar \
 *         mods/logger/src/logger.c \
 *         src/numeric_types.c src/scalar.c \
 *         tests/test_linear_scalar.c \
 *         -I./include -I./mods/logger/include
 */

#include "logger.h"
#include "numeric_types.h"
#include "scalar.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Test scalar operations
 *
 * @return 1 on failure, 0 on success
 */

/**
 * @brief Test the correctness of scalar addition
 */
int test_scalar_add(void);

/**
 * @brief Test the correctness of scalar subtraction
 */
int test_scalar_subtract(void);

/**
 * @brief Test the correctness of scalar multiplication
 */
int test_scalar_multiply(void);

/**
 * @brief Test the correctness of scalar division
 */
int test_scalar_divide(void);

/**
 * @brief Unit Tests
 */

int test_scalar_add(void) {
    int pass = 0; // assume success

    float          a      = 4.5f;
    float          b      = 2.5f;
    float          result = 0.0f;
    numeric_data_t type   = NUMERIC_FLOAT32;

    result = *(float*) scalar_add(&a, &b, &result, type);

    if (0.0f == result) {
        LOG_ERROR("Failed to add result.\n");
        pass = 1;
    } else if (7.0f != result) {
        LOG_ERROR("Failed to get expected sum.\n");
    }

    printf("%s", 0 == pass ? "." : "x");
    return pass;
}

int test_scalar_subtract(void) {
    int pass = 0; // assume success

    int32_t        a      = 7;
    int32_t        b      = 2;
    int32_t        result = 0;
    numeric_data_t type   = NUMERIC_INT32;

    result = *(int32_t*) scalar_subtract(&a, &b, &result, type);

    if (0 == result) {
        LOG_ERROR("Failed to subtract result.\n");
        pass = 1;
    } else if (5 != result) {
        LOG_ERROR("Failed to get expected difference.\n");
    }

    printf("%s", 0 == pass ? "." : "x");
    return pass;
}

int test_scalar_multiply(void) {
    int pass = 0; // assume success

    int32_t        a      = 4;
    int32_t        b      = 2;
    int32_t        result = 0;
    numeric_data_t type   = NUMERIC_INT32;

    result = *(int32_t*) scalar_multiply(&a, &b, &result, type);

    if (0 == result) {
        LOG_ERROR("Failed to multiply result.\n");
        pass = 1;
    } else if (8 != result) {
        LOG_ERROR("Failed to get expected product.\n");
        pass = 1;
    }

    printf("%s", 0 == pass ? "." : "x");
    return pass;
}

/**
 * @brief Test the correctness of scalar addition
 */
int test_scalar_divide(void) {
    int pass = 0; // assume success

    float          a      = 10.0f;
    float          b      = 5.0f;
    float          result = 0.0f;
    numeric_data_t type   = NUMERIC_FLOAT32;

    result = *(float*) scalar_divide(&a, &b, &result, type);

    if (0.0f == result) {
        LOG_ERROR("Failed to divide result.\n");
        pass = 1;
    } else if (2.0f != result) {
        LOG_ERROR("Failed to get expected ratio.\n");
        pass = 1;
    }

    printf("%s", 0 == pass ? "." : "x");
    return pass;
}

int main(void) {
    int pass = 1; // assume failure

    pass &= test_scalar_add();
    pass &= test_scalar_subtract();
    pass &= test_scalar_multiply();
    pass &= test_scalar_divide();
    printf("\n"); // pad output

    return pass;
}
