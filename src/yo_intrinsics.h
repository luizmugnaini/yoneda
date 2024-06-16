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
/// Description: Library intrinsics and defaults.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include <stddef.h>
#include <stdint.h>

#define yo_internal static

#if defined(__cplusplus)
#    define YO_START_C_HEADER extern "C" {
#    define YO_END_C_HEADER   }
#else
#    define YO_START_C_HEADER
#    define YO_END_C_HEADER
#endif  // __cplusplus

YO_START_C_HEADER

#ifndef NULL
#    if defined(__cplusplus)
#        define NULL 0
#    else
#        define NULL ((void*)0)
#    endif
#endif

#define yo_likely(expr)   __builtin_expect(!!((long)((bool)(expr))), 1)
#define yo_unlikely(expr) __builtin_expect(!!((long)((bool)(expr))), 0)

#define YO_DEFAULT_ALIGNMENT sizeof(void*)

#define yo_alignof(T) _Alignof(T)

#define yo_stringify(x) #x

#define yo_min(lhs, rhs) ((lhs < rhs) ? (lhs) : (rhs))
#define yo_max(lhs, rhs) ((lhs > rhs) ? (lhs) : (rhs))

#define yo_is_pow_of_two(n) (((n) > 0) && !((n) & ((n) - 1)))

#define yo_discard(x) (void)(x)

YO_END_C_HEADER
