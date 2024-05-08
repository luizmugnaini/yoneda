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
/// Description: Implementation of the arena memory allocator.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include "yo_arena.h"

#include <string.h>
#include "yo_assert.h"
#include "yo_io.h"

yo_internal uintptr_t impl_align_forward(uintptr_t addr, uint32_t align) {
    yo_assert_msg(
        yo_is_pow_of_two(align),
        "Expected the alignment to be a power of two");

    uintptr_t mod_align = addr & ((uintptr_t)align - 1);
    if (mod_align != 0) {
        addr += align - mod_align;
    }
    return addr;
}

struct yo_arena yo_new_arena(uint8_t* memory, size_t capacity) {
    yo_assert_msg(
        (capacity == 0) || (capacity != 0 && memory),
        "Non-zero capacity but null memory");
    return (struct yo_arena){.memory = memory, .capacity = capacity};
}

void yo_arena_clear(struct yo_arena* arena) {
    if (yo_likely(arena)) {
        arena->offset = 0;
    }
}

uint8_t*
yo_arena_alloc_aligned(struct yo_arena* arena, size_t size, uint32_t align) {
    if (yo_unlikely(!arena) || yo_unlikely(size == 0 || arena->capacity == 0)) {
        return NULL;
    }

    // Compute the address of the block that will be created.
    uintptr_t mem_end   = (uintptr_t)arena->memory + arena->capacity;
    uintptr_t new_block = impl_align_forward(mem_end, align);

    // Check if there is enough memory.
    if (yo_unlikely(new_block + size > mem_end)) {
        yo_log_fmt(
            YO_LOG_ERROR,
            "Arena unable to allocate %zu bytes of memory. Only %zu bytes "
            "remaining",
            size + new_block - ((uintptr_t)arena->memory + arena->offset),
            arena->capacity - arena->offset);
        return NULL;
    }

    // Commit the allocation by bumping the offset.
    arena->offset = size + new_block - mem_end;

    memset((void*)new_block, 0, size);

    return (uint8_t*)new_block;
}

uint8_t* yo_arena_realloc_aligned(
    struct yo_arena* arena,
    uint8_t*         block,
    size_t           current_size,
    size_t           new_size,
    uint32_t         align) {
    // Check if there is any memory at all.
    if (yo_unlikely(!arena) ||
        yo_unlikely((new_size == 0) || (arena->capacity == 0))) {
        return NULL;
    }

    // Check if the user wants to allocate a completely new block.
    if (yo_unlikely(!block || (current_size == 0))) {
        return yo_arena_alloc_aligned(arena, new_size, align);
    }

    uint8_t* mem_start = arena->memory;
    uint8_t* mem_end   = arena->memory + arena->capacity;
    uint8_t* free_mem  = arena->memory + arena->offset;

    if (yo_unlikely((block < mem_start) || (block >= mem_end))) {
        yo_log(
            YO_LOG_ERROR,
            "Tried to reallocate a block outside of the arena's domain.");
        return NULL;
    }

    // Check if the block is already free.
    if (yo_unlikely(block >= free_mem)) {
        yo_log(
            YO_LOG_ERROR,
            "Tried to reallocate a free address of the arena's domain.");
        return NULL;
    }

    yo_assert_msg(
        current_size <= arena->offset,
        "Arena tried to reallocate a block whose size surpasses the current "
        "offset of the arena, which shouldn't be possible");

    // Check if the block corresponds to the last allocation.
    if (block == free_mem - current_size) {
        // Check if there is enough space.
        if (yo_unlikely(block + new_size > mem_end)) {
            yo_log_fmt(
                YO_LOG_ERROR,
                "ArenaAlloc::realloc unable to reallocate block from %zu bytes "
                "to %zu "
                "bytes.",
                current_size,
                new_size);
            return NULL;
        }

        // Commit the difference between the given sizes.
        arena->offset += new_size - current_size;

        return block;
    }

    // There is no help but to create a totally new block.
    uint8_t* new_mem = yo_arena_alloc_aligned(arena, new_size, align);

    // Copy the existing data to the new block.
    size_t copy_size = yo_min(current_size, new_size);
    memcpy(new_mem, block, copy_size);

    return new_mem;
}
