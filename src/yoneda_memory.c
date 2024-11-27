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
/// Description: Implementation of the utilities for memory-related operations.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <yoneda_memory.h>

#include <string.h>
#include <yoneda_assert.h>
#include <yoneda_core.h>
#include <yoneda_math.h>

#if defined(YO_OS_WINDOWS)
#    include <Windows.h>
#elif defined(YO_OS_UNIX)
#    include <sys/mman.h>
#endif

bool yo_arch_is_little_endian() {
    i32 integer = 1;
    return yo_cast(bool, *(yo_cast(u8*, &integer)));
}

bool yo_arch_is_bit_endian() {
    i32 integer = 1;
    return yo_cast(bool, !*(yo_cast(u8*, &integer)));
}

u8* yo_memory_virtual_alloc(usize size_bytes) {
    u8* memory = NULL;

#if defined(YO_OS_WINDOWS)
    memory.buf = yo_cast(u8*, VirtualAlloc(nullptr, size_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
#    if defined(YO_ABORT_AT_MEMORY_ERROR)
    if (yo_unlikely(memory == NULL)) {
        yo_log_fatal_fmt("OS failed to allocate memory with error code: %lu", GetLastError());
        yo_abort_program();
    }
#    endif
#elif defined(YO_OS_UNIX)
    memory.buf = yo_cast(u8*, mmap(nullptr, size_bytes, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    if (yo_likely(yo_cast(void*, memory.buf) != MAP_FAILED)) {
        memory = NULL;
#    if defined(YO_ABORT_AT_MEMORY_ERROR)
        yo_log_error_fmt("OS failed to allocate memory due to: %s", strerror(errno));
        yo_abort_program();
#    endif
    }
#endif

    return memory;
}

void yo_memory_virtual_free(u8* memory, usize, size_bytes) {
#if defined(YO_OS_WINDOWS)
    BOOL result = VirtualFree(memory, 0, MEM_RELEASE);
    if (yo_unlikely(result == FALSE)) {
        yo_log_error_fmt("Failed free memory with error code: %lu", GetLastError());
    }
#elif defined(YO_OS_UNIX)
    i32 result = munmap(memory, size_bytes);
    if (yo_unlikely(result == -1)) {
        yo_log_error_fmt("Failed to free memory due to: %s", strerror(errno));
    }
#endif
}

void yo_memory_set(u8* memory, usize size_bytes, i32 fill) {
    if (yo_unlikely(size_bytes == 0)) {
        return;
    }
    yo_assert(memory != nullptr);

    yo_discard_value(memset(memory, fill, size_bytes));
}

void yo_memory_copy(u8* yo_no_alias dst, u8 const* yo_no_alias src, usize size_bytes) {
    if (yo_unlikely(size_bytes == 0)) {
        return;
    }
    yo_assert(dst != nullptr);
    yo_assert(src != nullptr);

#if defined(YO_CHECK_MEMCPY_OVERLAP)
    yo_assert_msg(
        (dst + size_bytes > src) || (dst < src + size_bytes),
        "Source and destination overlap in copy region (UB).");
#endif

    yo_discard_value(memcpy(dst, src, size_bytes));
}

void yo_memory_move(u8* yo_no_alias dst, u8 const* yo_no_alias src, usize size_bytes) {
    if (yo_unlikely(size_bytes == 0)) {
        return;
    }
    yo_assert(dst != nullptr);
    yo_assert(src != nullptr);

    yo_discard_value(memmove(dst, src, size_bytes));
}

usize yo_padding_with_header(
    uptr  ptr_addr,
    usize alignment,
    usize header_size,
    usize header_alignment) {
    yo_assert_fmt(
        yo_is_pow_of_two(alignment) && yo_is_pow_of_two(header_alignment),
        "Expected the alignments to be powers of two (alignment: %zu; header_alignment: %zu).",
        alignment,
        header_alignment);

    // Calculate the padding necessary for the alignment of the new block of memory.
    usize padding   = 0;
    usize mod_align = ptr_addr & (alignment - 1);  // Same as `ptr_addr % alignment`.
    if (mod_align != 0) {
        padding += alignment - mod_align;
    }
    ptr_addr += padding;

    // Padding necessary for the header alignment.
    usize mod_header = ptr_addr & (header_alignment - 1);  // Same as `ptr_addr % header_alignment`.
    if (mod_header != 0) {
        padding += header_alignment - mod_header;
    }

    // The padding should at least contain the header.
    padding += header_size;

    return padding;
}

usize yo_align_forward(uptr ptr_addr, usize alignment) {
    yo_assert_fmt(yo_is_pow_of_two(alignment), "Expected alignment (%zu) to be a power of two.", alignment);

    usize mod_align = ptr_addr & (alignment - 1);
    if (mod_align != 0) {
        ptr_addr += alignment - mod_align;
    }

    return ptr_addr;
}
