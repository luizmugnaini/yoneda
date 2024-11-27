///                             Yoneda library
/// Copyright (C) 2024 Luiz Gustavo Mugnaini Anselmo
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
/// Description: Conversion between number representations.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#pragma once

#include <yoneda_arena.hpp>
#include <yoneda_assert.hpp>
#include <yoneda_bit.hpp>
#include <yoneda_string.hpp>

psh_api psh_inline char yo_digit_to_char(u8 digit) {
    yo_assert_msg(digit < 10, "Expected digit to be between 0 and 9.");
    return '0' + yo_cast(char, digit);
}

/// Get the binary representation of a number
template <typename T>
yo_api String binary_repr(Arena* arena, T value) {
    if (value == 0) {
        return make_string(arena, yo_comptime_make_string_view("0b0"));
    }

    constexpr i32 BIT_COUNT = yo_type_bit_count(T);

    yo_DynString repr = yo_make_dyn_string(arena, 2 + BIT_COUNT + 1);

    repr.buf[0] = '0';
    repr.buf[1] = 'b';
    repr.length += 2;

    i32 leading_zeros_count = 0;
    for (i32 idx = BIT_COUNT - 1; idx >= 0; --idx) {
        if (yo_bit_at(value, idx) == 0) {
            ++leading_zeros_count;
        } else {
            break;
        }
    }

    for (i32 idx = BIT_COUNT - 1 - leading_zeros_count; idx >= 0; --idx, ++repr.length) {
        repr[repr.length] = yo_digit_to_char(yo_bit_at(value, idx));
    }

    return repr;
}
