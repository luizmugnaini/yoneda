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
/// Description: Implementation of the code assertion utilities.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include "yo_assert.h"

#include "yo_io.h"

void yo_impl_assert(
    struct yo_src_info info,
    bool               expr_res,
    char const*        expr_str,
    char const*        msg) {
    if (!(bool)expr_res) {
        yo_impl_log_fmt(
            info,
            YO_LOG_FATAL,
            "Assertion failed: %s, msg: %s",
            (char const*)expr_str,
            (char const*)msg);
    }
}
