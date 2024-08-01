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

#ifndef YO_CORE_H
#define YO_CORE_H

#include <stdbool.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
// - Macros for C++ compliance -
// -----------------------------------------------------------------------------

/// Macro for C++ detection.
#if defined(__cplusplus)
#    define YO_LANG_CPP
#else
#    define YO_LANG_C
#endif

/// Defining what NULL is, depending on the language being compiled.
#if !defined(NULL)
#    if defined(YO_LANG_CPP)
#        define NULL 0
#    else
#        define NULL ((void*)0)
#    endif
#endif

#if defined(YO_LANG_CPP)
extern "C" {
#endif

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

/// Windows-specific tweaks.
#if defined(YO_OS_WINDOWS_32) || defined(YO_OS_WINDOWS_64)
// Stop MSVC from complaining about fopen and such.
#    ifndef _CRT_SECURE_NO_WARNINGS
#        define _CRT_SECURE_NO_WARNINGS
#    endif
// Macros disabling many of the annoying Windows headers macros.
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#    ifndef NOATOM
#        define NOATOM
#    endif
#    ifndef NOGDI
#        define NOGDI
#    endif
#    ifndef NOKERNEL
#        define NOKERNEL
#    endif
#    ifndef NOUSER
#        define NOUSER
#    endif
#    ifndef NONLS
#        define NONLS
#    endif
#    ifndef NOMB
#        define NOMB
#    endif
#    ifndef NOMEMMGR
#        define NOMEMMGR
#    endif
#    ifndef NOMETAFILE
#        define NOMETAFILE
#    endif
#    ifndef NOOPENFILE
#        define NOOPENFILE
#    endif
#    ifndef NOSERVICE
#        define NOSERVICE
#    endif
#    ifndef NOSOUND
#        define NOSOUND
#    endif
#    ifndef NOWH
#        define NOWH
#    endif
#    ifndef NOCOMM
#        define NOCOMM
#    endif
#    ifndef NODEFERWINDOWPOS
#        define NODEFERWINDOWPOS
#    endif
#    ifndef NOMCX
#        define NOMCX
#    endif
#    ifndef NOIME
#        define NOIME
#    endif
#endif  // YO_OS_WINDOWS_32 || YO_OS_WINDOWS_64

/// Macros for compiler detection.
#if defined(_MSC_VER)
#    define YO_COMPILER_MSVC
#    if _MSC_VER >= 1920
#        define YO_COMPILER_MSVC_YEAR 2019
#    elif _MSC_VER >= 1910
#        define YO_COMPILER_MSVC_YEAR 2017
#    elif _MSC_VER >= 1900
#        define YO_COMPILER_MSVC_YEAR 2015
#    elif _MSC_VER >= 1800
#        define YO_COMPILER_MSVC_YEAR 2013
#    elif _MSC_VER >= 1700
#        define YO_COMPILER_MSVC_YEAR 2012
#    elif _MSC_VER >= 1600
#        define YO_COMPILER_MSVC_YEAR 2010
#    elif _MSC_VER >= 1500
#        define YO_COMPILER_MSVC_YEAR 2008
#    elif _MSC_VER >= 1400
#        define YO_COMPILER_MSVC_YEAR 2005
#    else
#        define YO_COMPILER_MSVC_YEAR 0
#    endif
#    if defined(__clang_major__)
#        define YO_COMPILER_CLANG_CL
#        define YO_COMPILER_CLANG
#    endif
#elif defined(__clang__)
#    define YO_COMPILER_CLANG
#elif defined(__GNUC__)
#    define YO_COMPILER_GCC
#endif

// -----------------------------------------------------------------------------
// - Fundamental types -
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
// - Macros for compiler hints -
// -----------------------------------------------------------------------------

/// Function inlining.
#define yo_inline inline

/// Signals internal linkage.
#define yo_internal static

/// Signals that a variable is available in the global scope.
#define yo_global static

/// Compiler hints for branching patterns.
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_likely(expr)   __builtin_expect(!!((long)((bool)(expr))), 1)
#    define yo_unlikely(expr) __builtin_expect(!!((long)((bool)(expr))), 0)
#else
#    define yo_likely(expr)   (expr)
#    define yo_unlikely(expr) (expr)
#endif

/// printf-like function attribute.
///
/// Parameters:
///     * fmt_pos: The position of the argument containing the formatting string (the first argument
///                of a function has position 1).
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_attr_fmt(fmt_pos) __attribute__((__format__(__printf__, fmt_pos, fmt_pos + 1)))
#else
#    define yo_attr_fmt(fmt_pos)
#endif

/// Discard the value of a given expression.
#define yo_discard(x) (void)(x)

/// Macro for aborting a program at runtime.
#if defined(YO_COMPILER_MSVC)
#    define yo_abort() __debugbreak()
#elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_abort() __builtin_trap()
#else
#    include <signal.h>
#    define yo_abort() raise(SIGTRAP)
#endif

/// Macro for hinting that a codepath shouldn't be reachable.
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_unreachable() (__builtin_unreachable())
#elif defined(YO_COMPILER_MSVC)
#    define yo_unreachable() (__assume(false))
#else
#    define yo_unreachable()
#endif

// -----------------------------------------------------------------------------
// - Overriding allocation/deallocation -
// -----------------------------------------------------------------------------

#if defined(yo_malloc) && defined(yo_free)
// Fine, all defined.
#elif !defined(yo_malloc) && !defined(yo_free)
// Fine, none defined.
#else
#    error "You must define all or none of the macros yo_malloc and yo_free."
#endif

// Define the default allocation procedures.
#if !defined(yo_malloc)
#    define yo_malloc malloc
#endif
#if !defined(yo_free)
#    define yo_free free
#endif

// -----------------------------------------------------------------------------
// - Pointer operations -
// -----------------------------------------------------------------------------

/// Add or subtract an offset from a pointer if and only if the pointer is not
/// null.
#define yo_ptr_add(ptr, offset) ((ptr) == NULL ? NULL : (ptr) + (offset))
#define yo_ptr_sub(ptr, offset) ((ptr) == NULL ? NULL : (ptr) - (offset))

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
// - Source file information -
// -----------------------------------------------------------------------------

/// Path of the current source file.
#define yo_source_file() ((char const*)__FILE__)

/// Line number of the current source file.
#define yo_source_line() ((u32)__LINE__)

/// Name of the current function being executed.
#if defined(YO_COMPILER_MSVC) && YO_COMPILER_MSVC_YEAR < 2015
#    define yo_source_func() "<unknown function>"
#else
#    define yo_source_func() ((char const*)__func__)
#endif

// -----------------------------------------------------------------------------
// - Miscelaneous utility macros -
// -----------------------------------------------------------------------------

/// Number of elements contained in a given array.
#define yo_array_size(array_ptr) (sizeof(array_ptr) / sizeof(*array_ptr))

/// Swap the value of two variables.
#define yo_swap(type, lhs, rhs)             \
    do {                                    \
        type yo_swap_tmp__ = lhs;           \
        lhs                = rhs;           \
        rhs                = yo_swap_tmp__; \
    } while (0)

/// Get the offset of a struct member.
#define yo_offsetof(type, member_name) ((usize)(&((type*)0)->member_name))

/// Common memory sizes in bytes.
#define yo_kibibytes(n) ((n) * (1 << 10))
#define yo_mebibytes(n) ((n) * (1 << 20))
#define yo_gibibytes(n) ((n) * (1 << 30))

/// Generate a string containing the given expression.
#define yo_stringify(x) #x

// -----------------------------------------------------------------------------

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YO_CORE_H
