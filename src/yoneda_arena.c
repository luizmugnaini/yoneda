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
/// Description: Implementation of the arena memory allocator.
/// File name: yoneda_arena.c
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <yoneda_arena.h>

#include "yoneda_impl_common.h"

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
            "current_block_size (%zu) surpasses the current offset (%zu) of the arena, which isn't possible",
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
