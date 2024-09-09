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
 * @note Official document
 * @ref https://ieeexplore.ieee.org/document/8766229
 *
 * @note 3rd party work
 * @ref https://geeksforgeeks.org/ieee-standard-754-floating-point-numbers/
 *
 * @note Source code
 * @ref /usr/include/c10/util/half.h
 * @ref https://github.com/Maratyszcza/FP16
 * @ref https://github.com/pytorch/pytorch/blob/main/c10/util/Half.h
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
