/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/floating_point.c
 *
 * @brief A simple and easy-to-use API in C for handling various floating-point
 *        precisions, including 32-bit (float), 16-bit (half precision float:
 *        IEEE-754 & bfloat16), 16-bit Google Brain format, and extended 8-bit
 *        floats.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 *
 * @ref See teleprint-me/fixed-point for more information
 */

#include "floating_point.h"

#include <stdint.h>
#include <stdio.h>

int32_t encode_float32(float value) {
    linear_mask_t data;
    data.value = value;
    return data.bits;
}

float decode_float32(int32_t bits) {
    linear_mask_t data;
    data.bits = bits;
    return data.value;
}
