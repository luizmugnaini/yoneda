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
/// Description: Code assertion utilities.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#ifndef YONEDA_ASSERT_H
#define YONEDA_ASSERT_H

#include <stdbool.h>
#include <stdint.h>
#include "yo_intrinsics.h"
#include "yo_io.h"

YO_START_C_HEADER

// Internal definition of the assert macros
void yo_impl_assert(
    struct yo_src_info info,
    bool               expr_res,
    char const*        expr_str,
    char const*        msg);

/// Assert that an expression evaluates to true.
#define yo_assert(expr)          yo_impl_assert(YO_SRC_INFO, expr, #expr, "")
#define yo_assert_msg(expr, msg) yo_impl_assert(YO_SRC_INFO, expr, #expr, msg)

/// Macro used to mark code-paths as unreachable.
#define YO_UNREACHABLE yo_assert(false, "Code-path should be unreachable!")

/// Macro used to mark code-paths as unimplemented.
#define YO_TODO yo_assert(false, "TODO: code-path unimplemented!")

YO_END_C_HEADER

#endif  // YONEDA_ASSERT_H
