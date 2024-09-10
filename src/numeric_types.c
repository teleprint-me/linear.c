/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/numeric_types.c
 *
 * @brief API for handling various numeric types and conversions, currently
 *        focused on 32-bit floating-point (float) and integer (int32_t)
 *        representations. Future extensions may include 16-bit and 8-bit
 *        formats for digital signal processing.
 *
 * Only pure C is used with minimal dependencies on external libraries.
 *
 * - Keep the interface minimal and focused.
 * - Avoid generics; stick to a single base type (float, int32) for now.
 * - Isolate conversion logic into a utility file/module.
 * - Maintain clean and comprehensible separation between different components.
 */

#include "numeric_types.h"

#include <stdint.h>
#include <stdio.h>

int32_t numeric_encode_float32(float value) {
    numeric_union_t data;
    data.value = value;
    return data.bits;
}

float numeric_decode_float32(int32_t bits) {
    numeric_union_t data;
    data.bits = bits;
    return data.value;
}
