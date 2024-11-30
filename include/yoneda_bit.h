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

/// Construct a word with the pair (high byte, low byte).
#define yo_u16_from_bytes(hi_u8, lo_u8) ((yo_cast(u16, hi_u8) << 8) | yo_cast(u16, lo_u8))

/// Get the value of the high byte of a word.
#define yo_u16_hi(val_u16) ((val_u16) >> 8)

/// Get the value of the low byte of a word.
#define yo_u16_lo(val_u16) (0x00FF & (val_u16))

/// Set the value of the high byte of a word.
#define yo_u16_set_hi(var_u16, hi_u8)                             \
    do {                                                          \
        u16 yo_var_lo_ = (var_u16) & 0x00FF;                      \
        var_u16        = (yo_cast(u16, hi_u8) << 8) | yo_var_lo_; \
    } while (0)

/// Set the value of the low byte of a word.
#define yo_u16_set_lo(var_u16, lo_u8)                      \
    do {                                                   \
        u16 yo_var_hi_ = (var_u16) & 0xFF00;               \
        var_u16        = yo_var_hi_ | yo_cast(u16, lo_u8); \
    } while (0)

// -----------------------------------------------------------------------------
// Byte manipulations.
// -----------------------------------------------------------------------------

/// Low nibble of a byte.
#define yo_u8_lo(val_u8) (0x0F & (val_u8))

/// High nibble of a byte.
#define yo_u8_hi(val_u8) ((val_u8) >> 4)

/// Make a byte from a pair of nibbles.
#define yo_u8_from_nibbles(hi_nib, lo_nib) yo_cast(u8, ((hi_nib) << 4) | lo_nib)

/// Transform a byte into the high byte of a word.
#define yo_u8_to_u16_hi(val_u8) yo_cast(u16, (val_u8) << 8)

// -----------------------------------------------------------------------------
// Integer manipulations.
// -----------------------------------------------------------------------------

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
