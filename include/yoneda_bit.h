///                             Yoneda library
/// Copyright (C) 2024 - Present, Luiz Gustavo Mugnaini Anselmo
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of
/// this software and associated documentation files (the “Software”), to deal in
/// the Software without restriction, including without limitation the rights to
/// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
/// of the Software, and to permit persons to whom the Software is furnished to do
/// so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// Description: Bit twiddling utilities. Implemented as macros to provide speed.
/// File name: yoneda_bit.h
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_BIT_H
#define YONEDA_BIT_H

#include <limits.h>
#include <yoneda_core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Bit Manipulations.
// -----------------------------------------------------------------------------
// Note: The bit indexing count starts at zero for all macros.

/// Get the number of bits that compose a given type.
#define yo_type_bit_count(T) (CHAR_BIT * yo_size_of(T))

/// Get the number of type T whose n-th bit is set to 1 and all other bits are 0.
#define yo_bit(T, n) yo_cast(T, 1 << (n))

/// Get the number of type T whose n-th bit is set to 0 and all other bits are 1.
#define yo_not_bit(T, n) yo_cast(T, ~(yo_cast(T, 1 << (n))) & yo_cast(T, (1ULL << yo_type_bit_count(T)) - 1))

/// Get the number whose first `count` bits are 1's.
#define yo_bit_ones(count) ((1ULL << (count)) - 1)

/// Set the n-th bit of a variable to 1.
#define yo_bit_set(VarType, var, n)  \
    do {                             \
        var |= yo_bit(VarType, (n)); \
    } while (0)

/// Set the n-th bit of a variable to 0.
#define yo_bit_clear(VarType, var, n)    \
    do {                                 \
        var &= yo_not_bit(VarType, (n)); \
    } while (0)

/// Set the n-th bit to 1 if the condition passes, otherwise set the bit to 0.
#define yo_bit_set_or_clear_if(VarType, var, n, cond)                                          \
    do {                                                                                       \
        VarType yo_var_mask_ = yo_bit(VarType, n);                                             \
        var                  = ((var) & ~yo_var_mask_) | (-yo_cast(i32, cond) & yo_var_mask_); \
    } while (0)

/// Get the value of the n-th bit of given value.
#define yo_bit_at(val, n) (((val) >> (n)) & 0b1)

/// Get `count` bits from a number, starting at position `pos`.
#define yo_bits_at(val, pos, count) (((val) >> (pos)) & ((1ULL << (count)) - 1))

/// Test a bit: true if the bit at the given position is set, false otherwise.
#define yo_test_bit_at(val, n) yo_cast(bool, yo_bit_at(val, n))

// -----------------------------------------------------------------------------
// 16-bit manipulations.
// -----------------------------------------------------------------------------

/// Construct a 16 byte value from a pair of bytes.
yo_inline u16 yo_u16_from_bytes(u8 high_byte, u8 low_byte) {
    return ((yo_cast(u16, high_byte) << 8) | yo_cast(u16, low_byte));
}

/// Get the high byte of a 16-byte value.
yo_inline u8 yo_u16_high_byte(u16 value) {
    return yo_cast(u8, value >> 8);
}

/// Get the low byte of a 16-byte value.
yo_inline u8 yo_u16_low_byte(u16 value) {
    return yo_cast(u8, 0x00FF & value);
}

/// Set the value of the high byte of a 16-byte value.
yo_inline u16 yo_u16_set_high_byte(u16 value, u8 high_byte) {
    return (yo_cast(u16, high_byte) << 8) | (yo_cast(u16, value) & 0x00FF);
}

/// Set the value of the low byte of a 16-byte value.
yo_inline u16 yo_u16_set_low_byte(u16 value, u8 low_byte) {
    return (value & 0xFF00) | yo_cast(u16, low_byte);
}

// -----------------------------------------------------------------------------
// Byte manipulations.
// -----------------------------------------------------------------------------

/// Low nibble of a byte.
yo_api yo_inline u8 yo_u8_low_nibble(u8 value) {
    return 0x0F & value;
}

/// High nibble of a byte.
yo_api yo_inline u8 yo_u8_high_nibble(u8 value) {
    return value >> 4;
}

/// Make a byte from a pair of nibbles.
yo_api yo_inline u8 yo_u8_from_nibbles(u8 high_nibble, u8 low_nibble) {
    return (high_nibble << 4) | low_nibble;
}

/// Transform a byte into the high byte of a 16-byte value.
yo_api yo_inline u16 yo_u8_to_u16_high_byte(u8 value) {
    return yo_cast(u16, value) << 8;
}

// -----------------------------------------------------------------------------
// Integer manipulations.
// -----------------------------------------------------------------------------

yo_api yo_inline u32 yo_i32_abs_value(i32 value) {
    i32 mask = value >> (sizeof(i32) * CHAR_BIT - 1);
    return yo_cast(u32, (value + mask) ^ mask);
}

/// Check if a given pair of integers has the same sign.
#define yo_int_opposite_sign(a, b) (((a) ^ (b)) < 0)

/// Swap the value of two variables without branching.
#define yo_int_swap(a, b)                                                           \
    do {                                                                            \
        (void)(((a) == (b)) || ((((a) ^ (b)) && ((b) ^= (a) ^= (b), (a) ^= (b))))); \
    } while (0)

/// Rotate right by `n` digits.
#define yo_int_rotr(ValType, val, n) yo_cast(ValType, (val >> (n)) | (val << (yo_value_bit_count(val) - (n))))

/// Rotate left by `n` digits.
#define yo_int_rotl(ValType, val, n) yo_cast(ValType, (val << (n)) | (val >> (yo_value_bit_count(val) - (n))))

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_BIT_H
