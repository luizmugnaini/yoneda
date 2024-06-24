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

#include <yoneda/arena.h>

#include <string.h>
#include <yoneda/assert.h>
#include <yoneda/log.h>

// -----------------------------------------------------------------------------
// - Arena implementation internal details -
// -----------------------------------------------------------------------------

yo_internal uptr yo_impl_align_forward(uptr addr, u32 align) {
    yo_assert_msg(yo_is_pow_of_two(align), "Expected the alignment to be a power of two");

    uptr mod_align = addr & ((uptr)align - 1);
    if (mod_align != 0) {
        addr += align - mod_align;
    }
    return addr;
}

// -----------------------------------------------------------------------------
// - Arena implementation details -
// -----------------------------------------------------------------------------

struct yo_arena yo_new_arena(struct yo_memory memory) {
    yo_assert_msg((memory.size == 0) || (memory.buf != NULL), "Non-zero capacity but null memory");
    return (struct yo_arena){.size = memory.size, .offset = 0, .buf = memory.buf};
}

void yo_arena_reset(struct yo_arena* arena) {
    if (yo_likely(arena)) {
        arena->offset = 0;
    }
}

u8* yo_arena_alloc_aligned(struct yo_arena* arena, usize size, u32 align) {
    if (yo_unlikely(size == 0 || arena->size == 0)) {
        return NULL;
    }

    // Compute the address of the block that will be created.
    uptr mem_end        = (uptr)arena->buf + arena->size;
    uptr new_block_addr = yo_impl_align_forward(mem_end, align);

    // Check if there is enough memory.
    if (yo_unlikely(new_block_addr + size > mem_end)) {
        yo_error_fmt(
            "Arena unable to allocate %zu bytes of memory. Only %zu bytes remaining",
            size + new_block_addr - (uptr)arena->buf + arena->offset,
            arena->size - arena->offset);
        return NULL;
    }

    // Commit the allocation by bumping the offset.
    arena->offset = size + new_block_addr - mem_end;

    u8* new_block = (u8*)new_block_addr;
    yo_memset(new_block, 0, size);

    return new_block;
}

u8* yo_arena_realloc_aligned(
    struct yo_arena* arena,
    usize            current_size,
    u8*              block,
    usize            new_size,
    u32              align) {
    // Check if there is any memory at all.
    if (yo_unlikely((new_size == 0) || (arena->size == 0))) {
        return NULL;
    }

    // Check if the user wants to allocate a completely new block.
    if (yo_unlikely(!block || (current_size == 0))) {
        return yo_arena_alloc_aligned(arena, new_size, align);
    }

    uptr block_addr = (uptr)block;
    uptr mem_start  = (uptr)arena->buf;
    uptr mem_end    = mem_start + arena->size;
    uptr free_mem   = mem_start + arena->offset;

    if (yo_unlikely((block_addr < mem_start) || (block_addr >= mem_end))) {
        yo_error("Tried to reallocate a block outside of the arena's domain.");
        return NULL;
    }

    // Check if the block is already free.
    if (yo_unlikely(block_addr >= free_mem)) {
        yo_error("Tried to reallocate a free address of the arena's domain.");
        return NULL;
    }

    yo_assert_msg(
        current_size <= arena->offset,
        "Arena tried to reallocate a block whose size surpasses the current "
        "offset of the arena, which shouldn't be possible");

    // Check if the block corresponds to the last allocation.
    if (block_addr == free_mem - current_size) {
        // Check if there is enough space.
        if (yo_unlikely(block_addr + new_size > mem_end)) {
            yo_error_fmt(
                "Unable to reallocate block from %zu bytes to %zu bytes.",
                current_size,
                new_size);
            return NULL;
        }

        // Commit the difference between the given sizes.
        arena->offset += new_size - current_size;

        return block;
    }

    // There is no help but to create a totally new block.
    u8* new_mem = yo_arena_alloc_aligned(arena, new_size, align);

    // Copy the existing data to the new block.
    usize copy_size = yo_min(current_size, new_size);
    yo_memcpy(new_mem, block, copy_size);

    return new_mem;
}

struct yo_scratch_arena yo_new_scratch_arena(struct yo_arena arena) {
    return (struct yo_scratch_arena){
        .saved_offset = arena.offset,
        .arena        = &arena,
    };
}

struct yo_scratch_arena yo_scratch_arena_decouple(struct yo_scratch_arena sarena) {
    return (struct yo_scratch_arena){
        .saved_offset = sarena.arena->offset,
        .arena        = sarena.arena,
    };
}

void yo_scratch_arena_end(struct yo_scratch_arena sarena) {
    sarena.arena->offset = sarena.saved_offset;
}
