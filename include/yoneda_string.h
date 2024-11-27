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
/// Description: String related types and utility functions.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_STRING_H
#define YONEDA_STRING_H

#include <yoneda_arena.h>
#include <yoneda_core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

// -----------------------------------------------------------------------------
// String utilities.
// -----------------------------------------------------------------------------

typedef enum yo_StrCmp {
    YO_STR_CMP_LESS_THAN,
    YO_STR_CMP_EQUAL,
    YO_STR_CMP_GREATER_THAN,
} yo_StrCmp;

yo_api usize yo_cstring_length(cstring str);

// @TODO: Implement for yo_String and yo_DynString.
yo_api yo_StrCmp yo_cstring_cmp(cstring lhs, cstring rhs);
yo_api bool      yo_cstring_equal(cstring lhs, cstring rhs);

yo_api yo_inline bool yo_is_utf8(char c) {
    return (0x1F < c && c < 0x7F);
}

yo_api yo_inline char yo_digit_to_char(u8 digit) {
    yo_assert_msg(digit < 10, "Expected digit to be between 0 and 9.");
    return '0' + yo_cast(char, digit);
}

// -----------------------------------------------------------------------------
// String types.
// -----------------------------------------------------------------------------

/// Immutable string with known length.
typedef struct yo_api yo_String {
    cstring buf;
    usize   length;
} yo_String;

/// Create a string at compile time from a given string literal.
///
/// Note: You should ONLY use these macros with string literals, like "one ring rule them all".
#define yo_comptime_make_string(string_literal) \
    (yo_String) {                               \
        .buf    = string_literal,               \
        .length = sizeof(cstr_literal - 1),     \
    }

yo_api yo_inline yo_String yo_make_string(cstring str) {
    return (yo_String){str, yo_cstr_length(str)};
}

/// Dynamically sized string.
typedef struct yo_api yo_DynString {
    char*  buf;
    usize  length;
    usize  capacity;
    Arena* arena;
} yo_DynString;

yo_api yo_inline yo_DynString yo_make_dynstring(yo_Arena* arena, usize capacity) {
    char* memory = yo_arena_alloc(arena, char, capacity);
    yo_assert_msg(memory != NULL, "Failed to allocate memory.");

    return (yo_DynString){
        .buf      = memory,
        .length   = 0,
        .capacity = capacity,
        .arena    = arena,
    };
}

yo_api yo_inline void yo_init_dynstring(yo_String* string, yo_Arena* arena, usize capacity) {
    char* memory = yo_arena_alloc(arena, char, capacity);
    yo_assert_msg(memory != NULL, "Failed to allocate memory.");

    string->buf      = memory;
    string->length   = 0;
    string->capacity = capacity;
    string->arena    = arena;
}

yo_api yo_inline yo_DynString yo_make_dynstring_from_string(yo_Arena* arena, yo_String str) {
    yo_DynString dstring = yo_make_string(arena, string.length + 1);

    yo_memory_copy(yo_cast(u8*, dstring.buf), yo_cast(u8 const*, string.buf), sizeof(char) * string.length);
    dstring.length = string.length;

    return dstring;
}

yo_api yo_inline yo_String yo_make_string_from_dynstring(yo_DynString const* dstring) {
    return (yo_String){
        .buf    = dstring.buf,
        .length = dstring.length,
    };
}

yo_api yo_Status yo_dynstring_resize(yo_DynString* dstring, usize new_capacity);

/// Join an array of string views to a target string data. You can also provide a join element to be
/// attached to the end of each join.
yo_api yo_Status yo_join_strings(
    yo_DynString*    target,
    usize            join_strings_count,
    yo_String const* join_strings,
    yo_String        join_element);

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_STRING_H
