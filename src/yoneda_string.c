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
/// Description: Implementation of the string related types and utility functions.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <yoneda_string.h>

#include <string.h>

usize yo_cstring_length(cstring str) {
    usize length = 0;
    if (yo_likely(str != NULL)) {
        length = strlen(str);
    }
    return length;
}

yo_StrCmp yo_cstring_cmp(cstring lhs, cstring rhs) {
    i32 cmp = strcmp(lhs, rhs);

    yo_StrCmp result;
    if (cmp == 0) {
        result = YO_STR_CMP_EQUAL;
    } else if (cmp < 0) {
        result = YO_STR_CMP_LESS_THAN;
    } else {
        result = YO_STR_CMP_GREATER_THAN;
    }

    return result;
}

bool yo_cstring_equal(cstring lhs, cstring rhs) {
    return (strcmp(lhs, rhs) == 0);
}

yo_Status yo_dynstring_resize(yo_DynString* dstring, usize new_capacity) {
    char* new_buf;
    if (dstring->capacity == 0) {
        new_buf = yo_arena_alloc(dstring->arena, char, new_capacity);
    } else {
        new_buf = yo_arena_realloc(dstring->arena, char, dstring->buf, dstring->capacity, new_capacity);
    }

    yo_Status resize_status = YO_STATUS_FAILED;
    if (yo_likely(new_buf != NULL)) {
        dstring->buf      = new_buf;
        dstring->capacity = new_capacity;

        resize_status = YO_STATUS_OK;
    }

    return resize_status;
}

yo_Status yo_join_strings(
    yo_DynString*    target,
    usize            join_strings_count,
    yo_String const* join_strings,
    yo_String        join_element) {
    bool previously_empty = (target->length == 0);

    // Resize the string ahead of time.
    {
        // Get the number of characters that will be added to the string.
        usize additional_length = 1;
        {
            // Add the size accumulated by the join element.
            if (join_element.length != 0) {
                additional_length += previously_empty ? ((join_strings_count - 1) * join_element.length)
                                                      : (join_strings_count * join_element.length);
            }

            // Account for the size of each of the joining strings.
            for (usize idx = 0; idx < join_strings_count; ++idx) {
                additional_length += join_strings[idx].length;
            }
        }

        usize new_capacity = target->length + additional_length;
        if (target->capacity < new_capacity) {
            yo_Status resize_status = yo_dynstring_resize(target, new_capacity);
            if (yo_unlikely(!resize_status)) {
                return YO_STATUS_FAILED;
            }
        }
    }

    u8*   target_buf        = yo_cast(u8*, target->buf);
    usize new_string_length = target->length;
    if (join_element.length != 0) {
        usize first_idx = 0;

        // If the string was empty, join the first string without accounting for the joining element.
        if (previously_empty) {
            yo_String const* first_js        = &join_strings[0];
            usize            first_js_lenght = first_js->length;

            yo_memory_copy(target_buf + new_string_length, yo_cast(u8 const*, first_js->buf), first_js_lenght);
            new_string_length += first_js_lenght;

            ++first_idx;
        }

        // Join remaining strings.
        for (usize idx = first_idx; idx < join_strings_count; ++idx) {
            yo_String const* js        = &join_strings[idx];
            usize            js_length = js->length;

            usize previous_size = new_string_length;
            yo_memory_copy(target_buf + previous_size, yo_cast(u8 const*, join_element.buf), join_element.length);
            yo_memory_copy(target_buf + previous_size + join_element.length, yo_cast(u8 const*, js->buf), js_length);
            new_string_length += join_element.length + js_length;
        }
    } else {
        for (usize idx = 0; idx < join_strings_count; ++idx) {
            yo_String const* js        = &join_strings[idx];
            usize            js_length = js->length;

            yo_memory_copy(target_buf + new_string_length, yo_cast(u8 const*, js->buf), js_length);
            new_string_length += js_length;
        }
    }

    target->buf[new_string_length] = 0;
    target->length                 = new_string_length;

    return YO_STATUS_OK;
}

yo_internal yo_inline bool yo_impl_string_to_number_start_should_skip(char c) {
    return ((c == ' ') || (c == '\n') || (c == '\r'));
}

bool yo_string_to_i32(yo_String string, i32* result) {
    bool valid_conversion = ((string.length != 0) && (result != NULL));

    char const* string_end  = yo_ptr_add(string.buf, string.length);
    char const* number_scan = string.buf;

    while ((number_scan < string_end) && yo_impl_string_to_number_start_should_skip(*number_scan)) {
        ++number_scan;
    }

    valid_conversion &= (number_scan != string_end);

    i32 sign = 1;
    if (yo_likely(valid_conversion)) {
        char maybe_sign_char = *number_scan;
        if (maybe_sign_char == '-') {
            sign = -1;
            ++number_scan;
        } else if (maybe_sign_char == '+') {
            ++number_scan;
        } else if (yo_unlikely(!yo_char_is_digit(maybe_sign_char))) {
            valid_conversion = false;
        }
    }

    usize expected_number_length = yo_cast(usize, yo_ptr_offset_bytes(string_end, number_scan));

    i32 number = 0;
    for (u32 digit_idx = 0u; valid_conversion && (digit_idx < expected_number_length); ++digit_idx) {
        char digit = number_scan[digit_idx];
        valid_conversion &= yo_char_is_digit(digit);

        number = number * 10 + yo_char_to_digit(digit);
    }

    if (yo_likely(valid_conversion)) {
        *result = sign * number;
    }

    return valid_conversion;
}

bool yo_string_to_u32(yo_String string, u32* result) {
    bool valid_conversion = ((string.length != 0) && (result != NULL));

    char const* string_end  = yo_ptr_add(string.buf, string.length);
    char const* number_scan = string.buf;

    while ((number_scan < string_end) && yo_impl_string_to_number_start_should_skip(*number_scan)) {
        ++number_scan;
    }

    valid_conversion &= (number_scan != string_end);

    usize expected_number_length = yo_cast(usize, yo_ptr_offset_bytes(string_end, number_scan));

    u32 number = 0u;
    for (usize digit_idx = 0u; digit_idx < expected_number_length; ++digit_idx) {
        char digit = number_scan[digit_idx];
        valid_conversion &= yo_char_is_digit(digit);

        number = number * 10u + yo_cast(u32, yo_char_to_digit(digit));
    }

    if (yo_likely(valid_conversion)) {
        *result = number;
    }

    return valid_conversion;
}

yo_StrCmp yo_string_cmp(yo_String lhs, yo_String rhs) {
    usize length = yo_min_value(lhs.length, rhs.length);
    i32   cmp    = memcmp(lhs.buf, rhs.buf, length);

    yo_StrCmp result;
    if (cmp == 0) {
        result = YO_STR_CMP_EQUAL;
    } else if (cmp < 0) {
        result = YO_STR_CMP_LESS_THAN;
    } else {
        result = YO_STR_CMP_GREATER_THAN;
    }

    return result;
}

bool yo_string_equal(yo_String lhs, yo_String rhs) {
    usize length = lhs.length;
    return (length == rhs.length) ? (memcmp(lhs.buf, rhs.buf, length) == 0) : false;
}
