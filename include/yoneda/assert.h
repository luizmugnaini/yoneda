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

#include <yoneda/core.h>
#include <yoneda/log.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

#define YO_ASSERT_FMT "[Assertion failed] expr: %s, msg: %s"

/// Code assertion macros.
///
/// Note: An assertion will always evaluate the internal expression, but will only check for its
///       truthness in case either YO_DEBUG or YO_ENABLE_ASSERTS are defined.
#if defined(YO_DEBUG) || defined(YO_DISABLE_ASSERTS)
#    define yo_assert(expr)                                                    \
        do {                                                                   \
            if (!(bool)(expr)) {                                               \
                yo_fatal_fmt(YO_ASSERT_FMT, yo_stringify(expr), "no message"); \
                yo_abort();                                                    \
            }                                                                  \
        } while (0)
#    define yo_assert_msg(expr, msg)                                    \
        do {                                                            \
            if (!(bool)(expr)) {                                        \
                yo_fatal_fmt(YO_ASSERT_FMT, yo_stringify(expr), (msg)); \
                yo_abort();                                             \
            }                                                           \
        } while (0)
#else
#    define yo_assert(expr)          yo_discard(expr)
#    define yo_assert_msg(expr, msg) yo_discard(expr)
#endif  // YO_DEBUG || YO_DISABLE_ASSERTS

#define yo_todo()                                   \
    do {                                            \
        yo_fatal("TODO: code-path unimplemented!"); \
        yo_abort();                                 \
    } while (0)

#define yo_todo_msg(msg)                                             \
    do {                                                             \
        yo_fatal_fmt("TODO: code-path unimplemented, msg: %s", msg); \
        yo_abort();                                                  \
    } while (0)

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_ASSERT_H
