///                          Yoneda, C Library
///    Copyright (C) 2024 Luiz Gustavo Mugnaini Anselmo
///
///    This program is free software; you can redistribute it and/or modify
///    it under the terms of the GNU General Public License as published by
///    the Free Software Foundation; either version 2 of the License, or
///    (at your option) any later version.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    You should have received a copy of the GNU General Public License along
///    with this program; if not, write to the Free Software Foundation, Inc.,
///    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
///
/// Description: String types and their associated procedures.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#ifndef YO_STR_H
#define YO_STR_H

#include <yoneda/intrinsics.h>
#include <yoneda/types.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

/// String type with known size.
struct yo_str {
    usize  size;
    strptr buf;
};

/// Create a string structure with size known at compile time from a given c-string literal.
///
/// Note: Use this macro with care, you should only use it with literal strings. Otherwise the
///       length of the string won't be computed corrected by the compiler and you may obtain the
///       size of a pointer instead the length of the string.
#define yo_new_str(cstr_literal)                             \
    (struct yo_str) {                                        \
        .size = sizeof(cstr_literal), .buf = (cstr_literal), \
    }

struct yo_str yo_str_from(strptr cstr);
bool          yo_str_eq(struct yo_str lhs, struct yo_str rhs);
bool          yo_str_eq_cstr(struct yo_str str, strptr cstr);

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YO_STR_H
