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

#ifndef YO_INTRINSICS_H
#define YO_INTRINSICS_H

// -----------------------------------------------------------------------------
// - Macros for operating system and compiler detection -
// -----------------------------------------------------------------------------

/// Macros for OS detection.
#if defined(_WIN32)
#    define YO_OS_WINDOWS_32
#elif defined(_WIN64)
#    define YO_OS_WINDOWS_64
#elif defined(__APPLE__) || defined(__MACH__)
#    define YO_OS_APPLE
#elif defined(__linux__)
#    define YO_OS_LINUX
#elif defined(__unix__)
#    define YO_OS_UNIX
#elif defined(_POSIX_VERSION)
#    define YO_OS_POSIX
#endif

/// Macros for compiler detection.
#if defined(_MSC_VER)
#    define YO_COMPILER_MSVC
#elif defined(__clang__)
#    define YO_COMPILER_CLANG
#elif defined(__GNUC__)
#    define YO_COMPILER_GCC
#endif

// -----------------------------------------------------------------------------
// - Macros for C++ compliance -
// -----------------------------------------------------------------------------

/// Macro for C++ detection.
#if defined(__cplusplus)
#    define YO_LANG_CPP
#endif

/// Defining what NULL is, depending on the language being compiled.
#if !defined(NULL)
#    if defined(YO_LANG_CPP)
#        define NULL 0
#    else
#        define NULL ((void*)0)
#    endif
#endif

/// Macro for aborting a program at runtime.
#if defined(YO_COMPILER_MSVC)
#    define yo_abort() __debugbreak()
#elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_abort() __builtin_trap()
#else
#    include <signal.h>
#    define yo_abort() raise(SIGTRAP)
#endif

// -----------------------------------------------------------------------------
// - Macros for compiler hints -
// -----------------------------------------------------------------------------

/// Function inlining.
#define yo_inline inline

/// Signals internal linkage.
#define yo_internal static
/// Signals that a variable is available in the global scope.
#define yo_global   static

/// Compiler hints for branching patterns.
#define yo_likely(expr)   __builtin_expect(!!((long)((bool)(expr))), 1)
#define yo_unlikely(expr) __builtin_expect(!!((long)((bool)(expr))), 0)

/// printf-like function attribute.
///
/// Note: The first argument of a function has position 1.
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_attr_fmt(fmt_arg_pos) \
        __attribute__((__format__(__printf__, fmt_arg_pos, fmt_arg_pos + 1)))
#else
#    define yo_attr_fmt(fmt_arg_pos)
#endif

// -----------------------------------------------------------------------------
// - Macros for mathematical functions -
// -----------------------------------------------------------------------------

/// Minimum/maximum mathematical functions.
#define yo_min(lhs, rhs) (((lhs) < (rhs)) ? (lhs) : (rhs))
#define yo_max(lhs, rhs) (((lhs) > (rhs)) ? (lhs) : (rhs))

/// Check if a value is a power of two.
#define yo_is_pow_of_two(n) (((n) > 0) && !((n) & ((n)-1)))

/// Clamp a value to an interval.
#define yo_clamp(x, min, max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))

/// Add values and clamp to a lower bound.
#define yo_lb_add(lhs, rhs, lb) (((lhs) + (rhs)) < (lb) ? (lb) : ((lhs) + (rhs)))

/// Add values and clamp to an upper bound.
#define yo_ub_add(lhs, rhs, ub) (((lhs) + (rhs)) > (ub) ? (ub) : ((lhs) + (rhs)))

// -----------------------------------------------------------------------------
// - Miscelaneous utility macros -
// -----------------------------------------------------------------------------

/// Add or subtract an offset from a pointer if and only if the pointer is not null.
#define yo_ptr_add(ptr, offset) ((ptr) == NULL ? NULL : (ptr) + (offset))
#define yo_ptr_sub(ptr, offset) ((ptr) == NULL ? NULL : (ptr) - (offset))

/// Discard the value of a given expression.
#define yo_discard(x) (void)(x)

/// Common memory sizes in bytes.
#define yo_kibibytes(n) ((n) * (1 << 10))
#define yo_mebibytes(n) ((n) * (1 << 20))
#define yo_gibibytes(n) ((n) * (1 << 30))

/// Generate a string containing the given expression.
#define yo_stringify(x) #x

#define yo_source_file() ((char const*)__builtin_FILE())
#define yo_source_line() ((unsigned int)__builtin_LINE())

#endif  // YO_INTRINSICS_H
