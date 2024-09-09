/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/floating_point.h
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

#ifndef FLOATING_POINT_H
#define FLOATING_POINT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "float_is_close.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Define the linear data type
 *
 * @param LINEAR_FLOAT Enum representing the float data type
 * @param LINEAR_INT Enum representing the int data type
 */
typedef enum LinearData {
    LINEAR_FLOAT, // IEEE-754 32-bit precision
    LINEAR_INT32, // 32-bit integer precision
    LINEAR_TYPES, // Number of data types
} linear_data_t;

/**
 * @brief A union representing a flexible floating-point representation,
 *        allowing access to both raw bit data and its corresponding value.
 *
 * @param bits  Raw 32-bit integer bit representation of the floating-point
 *              number.
 * @param value The actual 32-bit floating-point value represented by this data
 *              structure.
 */
typedef union {
    float   value;
    int32_t bits;
} float_data_t;

/**
 * @brief Encodes a given float value into its corresponding 32-bit
 *        representation.
 *
 * @param[in] value The floating point number to be encoded.
 *
 * @return The resulting encoded 32-bit integer representation of the input
 *         value.
 */
int32_t encode_float32(float value);

/**
 * @brief Decodes a given 32-bit integer representation into its corresponding
 *        float value.
 *
 * @param[in] bits The encoded 32-bit integer bit representation of the
 *                 floating-point number.
 *
 * @return The decoded actual 32-bit floating-point value represented by this
 *         data structure.
 */
float decode_float32(int32_t bits);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FLOATING_POINT_H
