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
/// Description: Code assertion utilities.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_ASSERT_H
#define YONEDA_ASSERT_H

#include <yoneda_core.h>
#include <yoneda_log.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

#if (YO_C_VERSION >= 23) || defined(YO_LANG_CPP)
#    define yo_constexpr_assert(const_expr) static_assert(const_expr)
#else
#    include <assert.h>
#    if defined(_Static_assert)
#        define yo_constexpr_assert(const_expr) _Static_assert(const_expr)
#    else
#        define yo_constexpr_assert(const_expr) assert(const_expr)
#    endif
#endif

/// Assertion macros.
#if YO_ENABLE_ASSERTS
#    define yo_assert(expr)                                                             \
        do {                                                                            \
            if (!yo_cast(bool, expr)) {                                                 \
                yo_log_fatal_fmt("Assertion failed: %s, msg: %s", #expr, "no message"); \
                yo_abort_program();                                                     \
            }                                                                           \
        } while (0)
#    define yo_assert_msg(expr, msg)                                             \
        do {                                                                     \
            if (!yo_cast(bool, expr)) {                                          \
                yo_log_fatal_fmt("Assertion failed: %s, msg: %s", #expr, (msg)); \
                yo_abort_program();                                              \
            }                                                                    \
        } while (0)
#    define yo_assert_fmt(expr, fmt, ...)                                                \
        do {                                                                             \
            if (!yo_cast(bool, expr)) {                                                  \
                yo_log_fatal_fmt("Assertion failed: %s, msg: " fmt, #expr, __VA_ARGS__); \
                yo_abort_program();                                                      \
            }                                                                            \
        } while (0)
#    if YO_ENABLE_ASSERT_NOT_NULL
#        define yo_assert_not_null(ptr) yo_assert_msg((ptr) != NULL, "Invalid pointer.")
#    endif
#else
#    define yo_assert(expr)               yo_discard_value(expr)
#    define yo_assert_msg(expr, msg)      yo_discard_value(expr)
#    define yo_assert_fmt(expr, fmt, ...) yo_discard_value(expr)
#    define yo_assert_not_null(ptr)       yo_discard_value(ptr)
#endif

/// Abort the program when reaching a non-implemented code-path.
#define yo_todo()                                       \
    do {                                                \
        yo_log_fatal("TODO: code-path unimplemented!"); \
        yo_abort_program();                             \
    } while (0)
#define yo_todo_msg(msg)                                                 \
    do {                                                                 \
        yo_log_fatal_fmt("TODO: code-path unimplemented, msg: %s", msg); \
        yo_abort_program();                                              \
    } while (0)

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_ASSERT_H
