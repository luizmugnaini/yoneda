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
/// Description: Types used thoughout the library.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#ifndef YONEDA_TYPES_H
#define YONEDA_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <yoneda/intrinsics.h>

/// Unsigned integer type.
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u64      usize;

/// Signed integer type.
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef i64     isize;

/// Memory-address types.
typedef u64 uptr;
typedef i64 iptr;

/// Floating-point types.
typedef float  f32;
typedef double f64;

/// Immutable zero-terminated string type
///
/// A pointer to a contiguous array of constant character values.
typedef char const* strptr;

#endif  // YONEDA_TYPES_H
