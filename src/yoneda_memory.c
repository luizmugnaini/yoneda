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
/// Description: Implementation of the utilities for memory-related operations.
/// File name: yoneda_memory.c
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <yoneda_memory.h>

#include <string.h>
#include <yoneda_assert.h>
#include <yoneda_core.h>
#include <yoneda_log.h>
#include <yoneda_math.h>

#if defined(YO_OS_WINDOWS)
#    include <Windows.h>
#elif defined(YO_OS_UNIX)
#    include <sys/mman.h>
#endif

#if YO_ABORT_AT_MEMORY_ERROR
#    include <yoneda_log.h>
#    define yo_impl_return_from_memory_error()                                        \
        do {                                                                          \
            yo_log_fatal("YO_ABORT_AT_MEMORY_ERROR active, aborting the program..."); \
            yo_abort_program();                                                       \
        } while (0)
#else
#    define yo_impl_return_from_memory_error() return NULL
#endif

// -----------------------------------------------------------------------------
// Architecture detection.
// -----------------------------------------------------------------------------

bool yo_arch_is_little_endian(void) {
    i32 integer = 1;
    return yo_cast(bool, *(yo_cast(u8*, &integer)));
}

bool yo_arch_is_big_endian(void) {
    i32 integer = 1;
    return yo_cast(bool, !*(yo_cast(u8*, &integer)));
}

// -----------------------------------------------------------------------------
// Memory allocation/deallocation.
// -----------------------------------------------------------------------------

u8* yo_memory_virtual_alloc(usize size_bytes) {
    u8* memory = NULL;

#if defined(YO_OS_WINDOWS)
    memory = yo_cast(u8*, VirtualAlloc(NULL, size_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
#    if YO_ABORT_AT_MEMORY_ERROR
    if (yo_unlikely(memory == NULL)) {
        yo_log_fatal_fmt("OS failed to allocate memory with error code: %lu", GetLastError());
        yo_abort_program();
    }
#    endif
#elif defined(YO_OS_UNIX)
    memory = yo_cast(u8*, mmap(NULL, size_bytes, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    if (yo_likely(yo_cast(void*, memory) != MAP_FAILED)) {
        memory = NULL;
#    if YO_ABORT_AT_MEMORY_ERROR
        yo_log_error_fmt("OS failed to allocate memory due to: %s", strerror(errno));
        yo_abort_program();
#    endif
    }
#endif

    return memory;
}

void yo_memory_virtual_free(u8* memory, usize size_bytes) {
#if defined(YO_OS_WINDOWS)
    yo_discard_value(size_bytes);

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

// -----------------------------------------------------------------------------
// Memory manipulation.
// -----------------------------------------------------------------------------

void yo_memory_set(u8* memory, usize size_bytes, i32 fill) {
    if (yo_unlikely(size_bytes == 0)) {
        return;
    }
    yo_assert(memory != NULL);

    yo_discard_value(memset(memory, fill, size_bytes));
}

void yo_memory_copy(u8* yo_no_alias dst, u8 const* yo_no_alias src, usize size_bytes) {
    if (yo_unlikely(size_bytes == 0)) {
        return;
    }
    yo_assert(dst != NULL);
    yo_assert(src != NULL);

#if YO_ENABLE_MEMCPY_OVERLAP_CHECK
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
    yo_assert(dst != NULL);
    yo_assert(src != NULL);

    yo_discard_value(memmove(dst, src, size_bytes));
}

// -----------------------------------------------------------------------------
// Memory alignment.
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
// Memory Arena.
// -----------------------------------------------------------------------------

#define yo_impl_arena_report_out_of_memory(arena, requested_size, requested_alignment)   \
    do {                                                                                 \
        yo_log_error_fmt(                                                                \
            "Arena unable to allocate %zu bytes (with %u bytes of alignment) of memory." \
            " The allocator has only %zu bytes remaining.",                              \
            requested_size,                                                              \
            requested_alignment,                                                         \
            arena->capacity - arena->offset);                                            \
    } while (0)

#define yo_impl_arena_is_empty(arena) (((arena == NULL) || (arena)->capacity == 0) || ((arena)->buf == NULL))

u8* yo_arena_alloc_align(yo_Arena* arena, usize size_bytes, u32 alignment) {
    if (yo_unlikely(size_bytes == 0)) {
        return NULL;
    }
    if (yo_unlikely(arena == NULL)) {
        yo_impl_arena_report_out_of_memory(arena, size_bytes, alignment);
        yo_impl_return_from_memory_error();
    }

    // Check if there is enough memory.
    uptr memory_addr    = yo_cast(uptr, arena->buf);
    uptr new_block_addr = yo_align_forward(memory_addr + arena->offset, alignment);
    if (yo_unlikely(new_block_addr + size_bytes > arena->capacity + memory_addr)) {
        yo_impl_arena_report_out_of_memory(arena, size_bytes, alignment);
        yo_impl_return_from_memory_error();
    }

    // Commit the new block of memory.
    arena->offset = yo_cast(usize, size_bytes + new_block_addr - memory_addr);

    u8* new_block = yo_cast(u8*, new_block_addr);
    yo_memory_set(new_block, size_bytes, 0);
    return new_block;
}

u8* yo_arena_realloc_align(
    yo_Arena* arena,
    u8*       block,
    usize     current_size_bytes,
    usize     new_size_bytes,
    u32       alignment) {
    yo_assert_msg(new_size_bytes != 0, "Don't use realloc to free blocks of memory.");
    yo_assert_msg((block != NULL) && (current_size_bytes != 0), "Don't use realloc to allocate new memory.");

    if (yo_unlikely(arena == NULL)) {
        yo_impl_arena_report_out_of_memory(arena, new_size_bytes, alignment);
        yo_impl_return_from_memory_error();
    }

    // Avoid re-reading arena-> multiple times.
    uptr  memory_addr      = yo_cast(uptr, arena->buf);
    uptr  memory_end       = memory_addr + arena->capacity;
    usize memory_offset    = arena->offset;
    uptr  free_memory_addr = memory_addr + memory_offset;

    uptr block_addr = yo_cast(uptr, block);

    // Check if the block lies within the allocator's memory.
    if (yo_unlikely((block_addr < memory_addr) || (block_addr >= memory_end))) {
        yo_log_error("Pointer outside of arena domain.");
        yo_impl_return_from_memory_error();
    }

    // Check if the block is already free.
    if (yo_unlikely(block_addr >= free_memory_addr)) {
        yo_log_error("Pointer to a free address of the arena domain.");
        yo_impl_return_from_memory_error();
    }

    if (yo_unlikely(current_size_bytes > memory_offset)) {
        yo_log_error_fmt(
            "current_block_size (%zu) surpassthe current offset (%zu) of the arena, which isn't possible",
            current_size_bytes,
            memory_offset);
        yo_impl_return_from_memory_error();
    }

    // If the block is the last allocated, just bump the offset.
    if (block_addr == free_memory_addr - current_size_bytes) {
        // Check if there is enough space.
        if (yo_unlikely(block_addr + new_size_bytes > memory_end)) {
            yo_log_error_fmt(
                "Unable to reallocate block from %zu bytes to %zu bytes.",
                current_size_bytes,
                new_size_bytes);
            yo_impl_return_from_memory_error();
        }

        arena->offset = yo_cast(usize, yo_cast(isize, memory_offset) + yo_cast(isize, new_size_bytes - current_size_bytes));
        return block;
    }

    // Allocate a new block and copy old memory.
    u8* new_block = yo_arena_alloc_align(arena, new_size_bytes, alignment);
    yo_memory_move(new_block, block, yo_min_value(current_size_bytes, new_size_bytes));

    return new_block;
}
