///                             Yoneda library
/// Copyright (C) 2024 - Present, Luiz Gustavo Mugnaini Anselmo
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
/// Description: Library intrinsics and defaults.
/// File name: yoneda_core.h
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_CORE_H
#define YONEDA_CORE_H

#include <stdbool.h>  // bool
#include <stddef.h>   // offsetof
#include <stdint.h>   // Size-specific integral arithmetic types.

// -----------------------------------------------------------------------------
// - Macros for C++ compliance -
// -----------------------------------------------------------------------------

#if defined(__cplusplus)
#    define YO_LANG_CPP
#else
#    define YO_LANG_C
#endif

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
// Yoneda library compile-time flags.
// -----------------------------------------------------------------------------

#if !defined(YO_DEBUG)
#    define YO_DEBUG 0
#    ifndef YO_ENABLE_ASSERTS
#        define YO_ENABLE_ASSERTS 0
#    endif
#    ifndef YO_ENABLE_ASSERT_NOT_NULL
#        define YO_ENABLE_ASSERT_NOT_NULL 0
#    endif
#    ifndef YO_ENABLE_ABORT_AT_MEMORY_ERROR
#        define YO_ENABLE_ABORT_AT_MEMORY_ERROR 0
#    endif
#    ifndef YO_ENABLE_SAFE_POINTER_ARITHMETIC
#        define YO_ENABLE_SAFE_POINTER_ARITHMETIC 0
#    endif
#    ifndef YO_ENABLE_BOUNDS_CHECK
#        define YO_ENABLE_BOUNDS_CHECK 0
#    endif
#    ifndef YO_ENABLE_MEMCPY_OVERLAP_CHECK
#        define YO_ENABLE_MEMCPY_OVERLAP_CHECK 0
#    endif
#    ifndef YO_ENABLE_LOGGING
#        define YO_ENABLE_LOGGING 0
#    endif
#else  // YO_DEBUG
#    undef YO_DEBUG
#    define YO_DEBUG 1
#    ifndef YO_ENABLE_ASSERTS
#        define YO_ENABLE_ASSERTS 1
#    endif
#    ifndef YO_ENABLE_ASSERT_NOT_NULL
#        define YO_ENABLE_ASSERT_NOT_NULL 1
#    endif
#    ifndef YO_ENABLE_ABORT_AT_MEMORY_ERROR
#        define YO_ENABLE_ABORT_AT_MEMORY_ERROR 1
#    endif
#    ifndef YO_ENABLE_SAFE_POINTER_ARITHMETIC
#        define YO_ENABLE_SAFE_POINTER_ARITHMETIC 1
#    endif
#    ifndef YO_ENABLE_BOUNDS_CHECK
#        define YO_ENABLE_BOUNDS_CHECK 1
#    endif
#    ifndef YO_ENABLE_MEMCPY_OVERLAP_CHECK
#        define YO_ENABLE_MEMCPY_OVERLAP_CHECK 1
#    endif
#    ifndef YO_ENABLE_LOGGING
#        define YO_ENABLE_LOGGING 1
#    endif
#endif

#ifndef YO_ENABLE_ANSI_COLORS
#    define YO_ENABLE_ANSI_COLORS 0
#endif

// -----------------------------------------------------------------------------
// Macros for operating system and compiler detection.
// -----------------------------------------------------------------------------

#if defined(_WIN32)
#    define YO_OS_WINDOWS_32
#elif defined(_WIN64)
#    define YO_OS_WINDOWS_64
#endif
#if defined(_WIN32) || defined(_WIN64)
#    define YO_OS_WINDOWS
#endif
#if defined(__APPLE__) || defined(__MACH__)
#    define YO_OS_APPLE
#endif
#if defined(__linux__)
#    define YO_OS_LINUX
#endif
#if defined(__unix__)
#    define YO_OS_UNIX
#endif
#if defined(_POSIX_VERSION)
#    define YO_OS_POSIX
#endif

/// Windows-specific tweaks.
#if defined(YO_OS_WINDOWS)
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
#endif  // YO_OS_WINDOWS

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

#if __STDC_VERSION__ >= 202311L
#    define YO_C_VERSION 23
#elif __STDC_VERSION__ >= 201710L
#    define YO_C_VERSION 17
#elif __STDC_VERSION__ >= 201112L
#    define YO_C_VERSION 11
#else
#    error "Yoneda requires you to compile for C11 or later"
#endif

// -----------------------------------------------------------------------------
// DLL support - importing/exporting function declarations
// -----------------------------------------------------------------------------

#if defined(YO_DLL) && defined(YO_BUILD_DLL)
#    error "The user of the DLL version of the library should only define YO_DLL." \
           "The macro YO_BUILD_DLL should only be defined in the internal compilation stage of the library as a DLL."
#endif

#if defined(YO_DLL)
#    if defined(YO_OS_WINDOWS)
#        define yo_api __declspec(dllimport)
#    endif
#elif defined(YO_BUILD_DLL)
#    if defined(YO_OS_WINDOWS)
#        define yo_api __declspec(dllexport)
#    else
#        define yo_api __attribute__((visibility("default"))
#    endif
#else
#    define yo_api
#endif

// -----------------------------------------------------------------------------
// Architecture information.
// -----------------------------------------------------------------------------

/// Processor architecture.
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
#    define YO_ARCH_X64
#elif defined(__arm__) || defined(_ARM_) || defined(_ARM_ARCH)
#    define YO_ARCH_ARM
#endif

/// SIMD availability in x64 processors.
#if defined(YO_ARCH_X64)
#    if defined(YO_COMPILER_MSVC)
#        if defined(_M_AMD64)
#            define YO_ARCH_SIMD_SSE
#            define YO_ARCH_SIMD_SSE2
#        endif
#        if defined(__AVX2__)
#            define YO_ARCH_SIMD_AVX
#            define YO_ARCH_SIMD_AVX2
#        elif defined(__AVX__)
#            define YO_ARCH_SIMD_AVX
#        endif
#    elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#        if defined(__SSE__)
#            define YO_ARCH_SIMD_SSE
#        endif
#        if defined(__SSE2__)
#            define YO_ARCH_SIMD_SSE2
#        endif
#        if defined(__AVX__)
#            define YO_ARCH_SIMD_AVX
#        endif
#        if defined(__AVX2__)
#            define YO_ARCH_SIMD_AVX2
#        endif
#    endif
#endif  // YO_ARCH_X64

/// SIMD availability in ARM processors.
#if defined(YO_ARCH_ARM) && defined(__ARM_NEON)
#    define YO_ARCH_SIMD_NEON
#endif  // YO_ARCH_ARM

// -----------------------------------------------------------------------------
// Compiler hints.
// -----------------------------------------------------------------------------

/// Hint for function inlining.
#if defined(YO_COMPILER_MSVC)
#    define yo_inline __forceinline
#elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_inline inline __attribute__((always_inline))
#else
#    define yo_inline inline
#endif

/// Hints that the current switch branch should fallthrough the next.
#if defined(YO_LANG_CPP) && __cplusplus >= 202002L
#    define YO_FALLTHROUGH [[fallthrough]]
#else
#    define YO_FALLTHROUGH
#endif

/// Macro for aborting a program at runtime.
#if defined(YO_COMPILER_MSVC)
#    define yo_abort_program() __debugbreak()
#elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_abort_program() __builtin_trap()
#else
#    include <assert.h>
#    define yo_abort_program() assert(false)
#endif

/// Code-path should be unreachable.
#if defined(YO_COMPILER_MSVC)
#    define yo_unreachable() __assume(false)
#elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_unreachable() __builtin_unreachable()
#else
#    define yo_unreachable() yo_abort()
#endif

/// Linkage hits.
#define yo_internal static
#define yo_global   static

/// Hints for pointer aliasing rules.
#if defined(YO_COMPILER_MSVC)
#    define yo_no_alias __restrict
#elif defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_no_alias __restrict__
#else
#    define yo_no_alias
#endif

/// Compiler hints for branching patterns.
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_likely(expr)   __builtin_expect(!!(long)(expr), 1)
#    define yo_unlikely(expr) __builtin_expect(!!(long)(expr), 0)
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
#define yo_impl_discard_value(value) ((void)(value))
#define yo_discard_value(expr)       yo_impl_discard_value((expr))

// -----------------------------------------------------------------------------
// Fundamental types.
// -----------------------------------------------------------------------------

#define yo_type_alias(alias, type) typedef type alias

/// Unsigned integer type.
yo_type_alias(u8, uint8_t);
yo_type_alias(u16, uint16_t);
yo_type_alias(u32, uint32_t);
yo_type_alias(u64, uint64_t);

/// Signed integer type.
yo_type_alias(i8, int8_t);
yo_type_alias(i16, int16_t);
yo_type_alias(i32, int32_t);
yo_type_alias(i64, int64_t);

/// Memory-address types.
yo_type_alias(uptr, uintptr_t);
yo_type_alias(iptr, ptrdiff_t);
yo_type_alias(usize, size_t);
yo_type_alias(isize, ptrdiff_t);

/// Floating-point types.
yo_type_alias(f32, float);
yo_type_alias(f64, double);

/// Immutable zero-terminated string type
///
/// A pointer to a contiguous array of constant character values.
yo_type_alias(cstring, char const*);

/// Status of an operation.
///
/// This gives a better semantic meaning of the return of a failable function, while still
/// preserving the use of booleans for simple use.
enum yo_Status {
    YO_STATUS_FAILED = false,
    YO_STATUS_OK     = true,
};
yo_type_alias(yo_Status, enum yo_Status);

#if defined(YO_LANG_C)
#    if defined(__typeof__)
#        define yo_type_of(var) __typeof__(var)
#    endif
#else
#    define yo_type_of(var) decltype(var)
#endif

// -----------------------------------------------------------------------------
// Common operations.
// -----------------------------------------------------------------------------

/// Cast a value to another type.
#define yo_cast(T, value) ((T)(value))

/// Create a default value of a given type.
#if defined(YO_LANG_CPP)
#    define yo_make_default(T) (T{})
#else
#    define yo_make_default(T) ((T){0})
#endif

/// The element count of a C array (e.g. `u32 array[5]`).
#define yo_count_of(array_literal) (yo_size_of(array_literal) / yo_size_of(*array_literal))

/// Get the offset in bytes of the member of a given struct type.
#define yo_offset_of(T, member_name) offsetof(T, member_name)

/// Size in bytes of a given type.
#define yo_size_of(T) sizeof(T)

/// The alignment in bytes, required by a given type.
#if defined(YO_LANG_C)
#    define yo_align_of(T) _Alignof(T)
#else
#    define yo_align_of(T) alignof(T)
#endif

/// Alignment hint for forcing a certain alignment in a struct.
#if defined(YO_LANG_C)
#    define yo_align_as(T) _Alignas(T)
#else
#    define yo_align_as(T) alignas(T)
#endif

/// Swap the values of two variables.
#define yo_swap(T, lhs, rhs)       \
    do {                           \
        T swap_temp_ = lhs;        \
        lhs          = rhs;        \
        rhs          = swap_temp_; \
    } while (0)

/// Iterating over a range [start, end).
#define yo_range_for_each(idx, start, end)     for (isize idx = start; idx < end; ++idx)
#define yo_range_for_each_rev(idx, start, end) for (isize idx = end - 1; idx >= start; --idx)

// -----------------------------------------------------------------------------
// Pointer operations.
// -----------------------------------------------------------------------------

/// Add or subtract an offset from a pointer if and only if the pointer is not null.
#if YO_ENABLE_SAFE_POINTER_ARITHMETIC
#    define yo_ptr_add(ptr, offset) (((ptr) == NULL) ? NULL : (ptr + yo_cast(uptr, offset)))
#    define yo_ptr_sub(ptr, offset) (((ptr) == NULL) ? NULL : (ptr - yo_cast(iptr, offset)))
#else
#    define yo_ptr_add(ptr, offset) (ptr + yo_cast(uptr, offset))
#    define yo_ptr_sub(ptr, offset) (ptr - yo_cast(iptr, offset))
#endif

/// Check if two pointers refer to the same address in memory.
#define yo_ptr_same_addr(lhs_ptr, rhs_ptr) (yo_cast(uptr, yo_cast(u8 const*, lhs_ptr)) == yo_cast(uptr, yo_cast(u8 const*, rhs_ptr)))

/// Compute the offset, in bytes, between two pointers.
#define yo_ptr_offset_bytes(end_ptr, start_ptr) \
    (yo_ptr_same_addr(end_ptr, start_ptr)       \
         ? 0                                    \
         : yo_cast(iptr, yo_ptr_sub(yo_cast(u8 const*, end_ptr), yo_cast(iptr, start_ptr))))

// -----------------------------------------------------------------------------
// Mathematical operations.
// -----------------------------------------------------------------------------

/// Checks if a value is in the closed interval [min, max].
#define yo_value_in_range(value, min, max) (((min) <= (value)) && ((value) <= (max)))

/// Checks if a value is in the open intervalue (min, max).
#define yo_value_within_range(value, min, max) (((min) < (value)) && ((value) < (max)))

/// Minimum/maximum functions.
#define yo_min_value(lhs, rhs) (((lhs) < (rhs)) ? (lhs) : (rhs))
#define yo_max_value(lhs, rhs) (((lhs) > (rhs)) ? (lhs) : (rhs))

/// Clamp a value to an interval.
#define yo_clamp_value(x, min, max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))

// Get the sign of a number.
#define yo_sign_value(x) ((yo_cast(f64, x) > 0.0) ? 1 : ((yo_cast(f64, x) != 0.0) ? -1 : 0))

// Get the absolute value of a number.
#define yo_abs_value(x) ((yo_cast(f64, x) > 0.0) ? (x) : -(x))

/// Add values and clamp to a lower bound.
#define yo_lower_bound_add(lhs, rhs, lb) (((lhs) + (rhs)) < (lb) ? (lb) : ((lhs) + (rhs)))

/// Add values and clamp to an upper bound.
#define yo_upper_bound_add(lhs, rhs, ub) (((lhs) + (rhs)) > (ub) ? (ub) : ((lhs) + (rhs)))

/// Decrement an unsigned value without wrapping - the lower bound will always be zero.
#define yo_nowrap_unsigned_dec(x) (((x) > 0) ? (((x)) - 1) : 0)

/// Check if a value is a power of two.
#define yo_is_pow_of_two(n) (((n) > 0) && !((n) & (((n)) - 1)))

// -----------------------------------------------------------------------------
// Stringification and tokenization utilities.
// -----------------------------------------------------------------------------

/// Generate a string containing the given expression.
#define yo_stringify(x) #x

/// Concatenate two tokens as a string.
#define yo_token_concat(prefix, suffix) yo_impl_token_concat(prefix, suffix)

//
// Implementation details
//

#define yo_impl_token_concat(x, y) x##y

// -----------------------------------------------------------------------------
// Common memory sizes.
// -----------------------------------------------------------------------------

#define yo_kibibytes(n) ((n) * (1 << 10))
#define yo_mebibytes(n) ((n) * (1 << 20))
#define yo_gibibytes(n) ((n) * (1 << 30))

// -----------------------------------------------------------------------------
// Source introspection information.
// -----------------------------------------------------------------------------

/// Query the string representing the signature of the current function.
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_source_function_signature() __PRETTY_FUNCTION__
#elif defined(YO_COMPILER_MSVC)
#    define yo_source_function_signature() __FUNCSIG__
#else
#    define yo_source_function_signature() "<unknown signature>"
#endif

/// Query the string representing the unadorned name of the current function.
#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_source_function_name() __func__
#elif defined(YO_COMPILER_MSVC)
#    define yo_source_function_name() __FUNCTION__
#else
#    define yo_source_function_name() "<unknown name>"
#endif

#if defined(YO_COMPILER_CLANG) || defined(YO_COMPILER_GCC)
#    define yo_source_file_name()   __builtin_FILE()
#    define yo_source_line_number() __builtin_LINE()
#elif defined(YO_COMPILER_MSVC)
#    define yo_source_file_name()   __FILE__
#    define yo_source_line_number() __LINE__
#else
#    define yo_source_file_name()   "<unknown file>"
#    define yo_source_line_number() (0)
#endif

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_CORE_H
