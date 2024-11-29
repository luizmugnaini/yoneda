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
/// Description: Utilities for memory-related operations.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_MEMORY_H
#define YONEDA_MEMORY_H

#include <yoneda_core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

// -----------------------------------------------------------------------------
// CPU architecture query utilities.
// -----------------------------------------------------------------------------

/// Check whether the current architecture is little-endian or big-endian.
///
/// Note: Unfortunately, these checks can only be reliably made at runtime.
yo_api bool yo_arch_is_little_endian(void);
yo_api bool yo_arch_is_bit_endian(void);

// -----------------------------------------------------------------------------
// Memory manipulation utilities.
// -----------------------------------------------------------------------------

/// Reserve and commit a virtual block of memory.
///
/// The memory allocated is always initialized to zero.
///
/// Directly calls the respective system call for allocating memory. Should be
/// used for large allocations, if you want small allocations, malloc is the way to go.
yo_api u8* yo_memory_virtual_alloc(usize size_bytes);

/// Release and decommit all memory.
yo_api void yo_memory_virtual_free(u8* memory, usize size_bytes);

// TODO: add procedures for reserving/commiting memory separately.

/// Simple wrapper around `memset` that automatically deals with null values.
///
/// Does nothing if `ptr` is a null pointer.
yo_api void yo_memory_set(u8* memory, usize size_bytes, i32 fill);

#define yo_zero_struct_ptr(struct_ptr) yo_memory_set(struct_ptr, yo_sizeof(*struct_ptr), 0)

/// Simple wrapper around `memcpy`.
///
/// This function will assert that the blocks of memory don't overlap, avoiding undefined
/// behaviour introduced by `memcpy` in this case.
yo_api void yo_memory_copy(u8* yo_no_alias dst, u8 const* yo_no_alias src, usize size_bytes);

/// Simple wrapper around `memmove`.
///
/// Does nothing if either `dst` or `src` are null pointers.
yo_api void yo_memory_move(u8* yo_no_alias dst, u8 const* yo_no_alias src, usize size_bytes);

// -----------------------------------------------------------------------------
// Alignment utilities.
// -----------------------------------------------------------------------------

/// Compute the padding needed for the alignment of the memory and header.
///
/// The padding should contain the header, thus it is ensured that `padding >= header_size`.
/// Both the alignment needed for the new memory block as the alignment required by the header
/// will be accounted when calculating the padding.
///
/// Parameters:
///     * ptr: The current memory address.
///     * alignment: The alignment requirement for the new memory block.
///     * header_size: The total size of the header associated to the new memory block.
///     * header_alignment: The alignment required by the header.
///
/// Return: The resulting padding with respect to `ptr` that should satisfy the alignment
///         requirements, as well as accommodating the associated header.
yo_api usize yo_padding_with_header(
    uptr  ptr,
    usize alignment,
    usize header_size,
    usize header_alignment);

/// Compute the next address that satisfies a given alignment.
///
/// The alignment should always be a power of two.
///
/// Parameters:
///     * ptr: The starting address.
///     * alignment: The alignment requirement.
///
/// Return: The next address, relative to `ptr` that satisfies the alignment requirement imposed
///         by `alignment`.
yo_api usize yo_align_forward(uptr ptr, usize alignment);

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_MEMORY_H
