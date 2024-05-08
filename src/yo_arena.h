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

#include <stdint.h>
#include "yo_intrinsics.h"

YO_START_C_HEADER

/// Arena allocator
///
/// The arena allocator is great for the management of temporary allocation of
/// memory, since an allocation takes nothing more than incrementing an offset.
///
/// Note:
///     * The arena does not own memory, thus it is not responsible for the
///     freeing of it.
struct yo_arena {
    uint8_t* memory;
    size_t   offset;
    size_t   capacity;
};

struct yo_scratch_arena {
    struct yo_arena* arena;
    size_t           saved_offset;
};

struct yo_arena yo_new_arena(uint8_t* memory, size_t capacity);

void yo_arena_clear(struct yo_arena* arena);

uint8_t*
yo_arena_alloc_aligned(struct yo_arena* arena, size_t size, uint32_t align);

/// Allocate a block of memory with a default alignment.
#define yo_arena_alloc(a, s) yo_arena_alloc_aligned(a, s, YO_DEFAULT_ALIGNMENT)

/// Allocate a block of memory capable of a given type.
#define yo_arena_alloc_type(typ, a, s) \
    (typ*)yo_arena_alloc_aligned(a, sizeof(typ) * s, yo_alignof(typ))

uint8_t* yo_arena_realloc_aligned(
    struct yo_arena* arena,
    uint8_t*         block,
    size_t           current_size,
    size_t           new_size,
    uint32_t         align);

/// Reallocate a block of memory with a default alignment.
#define yo_arena_realloc(a, s) \
    yo_arena_realloc_aligned(a, s, YO_DEFAULT_ALIGNMENT)

/// Reallocate a block of memory of a given type.
#define yo_arena_realloc_type(typ, a, block, cs, ns) \
    (typ*)yo_arena_realloc_aligned(                  \
        a,                                           \
        (uint8_t*)block,                             \
        sizeof(typ) * cs,                            \
        sizeof(typ) * ns,                            \
        yo_alignof(typ))

struct yo_scratch_arena yo_new_scratch_arena(struct yo_arena* arena);

void yo_restore_arena(struct yo_arena* arena, struct yo_scratch_arena sarena);

YO_END_C_HEADER

#endif  // YONEDA_ARENA_H
