/**
 * @file include/numeric_types.h
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

#ifndef LINEAR_NUMERIC_TYPES_H
#define LINEAR_NUMERIC_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

/**
 * @brief Define the linear data type
 *
 * @param NUMERIC_FLOAT Enum representing the float data type
 * @param NUMERIC_INT32 Enum representing the int data type
 * @param NUMERIC_TYPES Enum representing the number of supported data types
 */
typedef enum NumericData {
    NUMERIC_FLOAT32, // IEEE-754 32-bit precision
    NUMERIC_INT32,   // 32-bit integer precision
    NUMERIC_TYPES,   // Number of data types
} numeric_data_t;

/**
 * @brief A union representing a flexible floating-point representation,
 *        allowing access to both raw bit data and its corresponding value.
 *
 * @param bits  Raw 32-bit integer bit representation of the floating-point
 *              number.
 * @param value The actual 32-bit floating-point value represented by this data
 *              structure.
 */
typedef union NumericUnion {
    float   value;
    int32_t bits;
} numeric_union_t;

/**
 * @brief Encodes a given float value into its corresponding 32-bit
 *        representation.
 *
 * @param[in] value The floating point number to be encoded.
 *
 * @return The resulting encoded 32-bit integer representation of the input
 *         value.
 */
int32_t numeric_encode_float32(float value);

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
float numeric_decode_float32(int32_t bits);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LINEAR_NUMERIC_TYPES_H
