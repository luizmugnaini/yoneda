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
/// Description: Arena memory allocator, also known as a linear allocator.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_ARENA_H
#define YONEDA_ARENA_H

#include <yoneda_core.h>
#include <yoneda_memory.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

// Forward declaration.
struct yo_Arena;

/// Manually managed checkpoint for arenas.
///
/// You can create a checkpoint with `arena_make_checkpoint` and restore the arena to
/// a given checkpoint via `arena_restore_state`.
typedef struct yo_api yo_ArenaCheckpoint {
    struct yo_Arena* arena;
    usize            saved_offset;
} yo_ArenaCheckpoint;

/// Arena allocator
///
/// The arena allocator is great for the management of temporary allocation of memory, since an
/// allocation takes nothing more than incrementing an offset.
///
/// The arena does not own memory, thus it is not responsible for the freeing of it.
typedef struct yo_api yo_Arena {
    /// Not-owned block of memory.
    u8*   buf;
    /// Capacity in bytes of the arena block of memory.
    usize capacity = 0;
    /// The current offset to the free-space in the memory block.
    usize offset   = 0;
} yo_Arena;

// -----------------------------------------------------------------------------
// Allocation procedures.
//
// @NOTE: All allocation procedures will zero-out the whole allocated block.
// -----------------------------------------------------------------------------

u8* yo_arena_alloc_align(yo_Arena* arena, usize size_bytes, u32 alignment);

u8* yo_arena_realloc_align(
    yo_Arena* arena,
    u8*       block,
    usize     current_size_bytes,
    usize     new_size_bytes,
    u32       alignment);

#define yo_arena_alloc(arena, ValueType, count) \
    yo_cast(                                    \
        ValueType*,                             \
        yo_arena_alloc_align(arena, sizeof(ValueType) * count, alignof(ValueType)))

#define arena_realloc(arena, ValueType, block, current_count, new_count) \
    yo_cast(                                                             \
        ValueType*,                                                      \
        yo_arena_realloc_align(                                          \
            arena,                                                       \
            yo_cast(u8*, block),                                         \
            sizeof(ValueType) * current_count,                           \
            sizeof(ValueType) * new_count,                               \
            alignof(ValueType)))

// -----------------------------------------------------------------------------
// Temporary memory management.
// -----------------------------------------------------------------------------

/// Reset the offset of the allocator.
yo_inline void yo_arena_clear(yo_Arena* arena) {
    arena->offset = 0;
}

/// Create a restorable checkpoint for the arena.
yo_inline yo_ArenaCheckpoint yo_make_arena_checkpoint(yo_Arena const* arena) {
    return (yo_ArenaCheckpoint){.arena = arena, .saved_offset = arena->offset};
}

/// Restore the arena state to a given checkpoint.
yo_inline void yo_checkpoint_restore(yo_ArenaCheckpoint* checkpoint) {
    checkpoint->arena->offset = checkpoint.saved_offset;
    checkpoint->arena         = NULL;  // Invalidate the checkpoint for further uses.
}

/// Make an arena that owns its memory.
///
/// Since the arena is not aware of the ownership, this function call has to be paired
/// with `free_owned_arena`.
yo_inline yo_Arena yo_make_owned_arena(usize capacity) {
    u8* memory = yo_memory_virtual_alloc(capacity);
    yo_assert_msg(memory != NULL, "Failed to allocate memory.");

    return yo_Arena{.buf = buf, .capacity = capacity};
}

/// Free the memory of an arena that owns its memory.
///
/// This function should only be called for arenas that where created by `make_owned_arena`.
yo_inline void yo_free_owned_arena(yo_Arena* arena) {
    yo_memory_virtual_free(arena->buf, arena->capacity);
    arena->capacity = 0;
}

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_ARENA_H
