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
/// Description: Arena memory allocator.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#ifndef YONEDA_ARENA_H
#define YONEDA_ARENA_H

#include <yoneda/core.h>
#include <yoneda/memutils.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

// -----------------------------------------------------------------------------
// - Arena allocator interface -
// -----------------------------------------------------------------------------

/// Arena memory allocator.
///
/// Note: The arena does not own memory, thus it is not responsible for the freeing of it.
struct yo_arena {
    usize size;
    usize offset;
    u8*   buf;
};

struct yo_arena yo_new_arena(struct yo_memory memory);

void yo_arena_reset(struct yo_arena* arena);

u8* yo_arena_alloc_aligned(struct yo_arena* arena, usize size, u32 align);

/// Allocate a block of memory capable of a given type.
#define yo_arena_alloc(type, arena, size) \
    (type*)yo_arena_alloc_aligned(arena, sizeof(type) * size, yo_alignof(type))

u8* yo_arena_realloc_aligned(
    struct yo_arena* arena,
    usize            current_size,
    u8*              block,
    usize            new_size,
    u32              align);

/// Reallocate a block of memory of a given type.
#define yo_arena_realloc(type, arena, current_size, block, new_size) \
    (type*)yo_arena_realloc_aligned(                                 \
        arena,                                                       \
        sizeof(type) * current_size,                                 \
        (u8*)block,                                                  \
        sizeof(type) * new_size,                                     \
        yo_alignof(type))

// -----------------------------------------------------------------------------
// - Scratch arena interface -
//
// TODO: alloc and realloc for the scratch arena.
// -----------------------------------------------------------------------------

struct yo_scratch_arena {
    usize            saved_offset;
    struct yo_arena* arena;
};

struct yo_scratch_arena yo_new_scratch_arena(struct yo_arena arena);

struct yo_scratch_arena yo_scratch_arena_decouple(struct yo_scratch_arena sarena);

void yo_scratch_arena_end(struct yo_scratch_arena sarena);

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_ARENA_H
