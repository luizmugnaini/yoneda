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
/// Description: Utilities for memory-related operations.
/// File name: yoneda_memory.h
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
yo_api bool yo_arch_is_big_endian(void);

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

#define yo_zero_struct_ptr(struct_ptr) yo_memory_set(struct_ptr, yo_size_of(*struct_ptr), 0)

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

// -----------------------------------------------------------------------------
// Memory arena allocator.
// -----------------------------------------------------------------------------

// Forward declaration.
struct yo_Arena;

/// Manually managed checkpoint for arenas.
///
/// You can create a checkpoint with `arena_make_checkpoint` and restore the arena to
/// a given checkpoint via `arena_restore_state`.
struct yo_api yo_ArenaCheckpoint {
    struct yo_Arena* arena;
    usize            saved_offset;
};
yo_type_alias(yo_ArenaCheckpoint, struct yo_ArenaCheckpoint);

/// Arena allocator
///
/// The arena allocator is great for the management of temporary allocation of memory, since an
/// allocation takes nothing more than incrementing an offset.
///
/// The arena does not own memory, thus it is not responsible for the freeing of it.
struct yo_api yo_Arena {
    /// Not-owned block of memory.
    u8*   buf;
    /// Capacity in bytes of the arena block of memory.
    usize capacity;
    /// The current offset to the free-space in the memory block.
    usize offset;
};
yo_type_alias(yo_Arena, struct yo_Arena);

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
        yo_arena_alloc_align(arena, yo_size_of(ValueType) * count, yo_align_of(ValueType)))

#define yo_arena_realloc(arena, ValueType, block, current_count, new_count) \
    yo_cast(                                                                \
        ValueType*,                                                         \
        yo_arena_realloc_align(                                             \
            arena,                                                          \
            yo_cast(u8*, block),                                            \
            yo_size_of(ValueType) * current_count,                          \
            yo_size_of(ValueType) * new_count,                              \
            yo_align_of(ValueType)))

// -----------------------------------------------------------------------------
// Temporary memory management.
// -----------------------------------------------------------------------------

/// Reset the offset of the allocator.
yo_inline void yo_arena_clear(yo_Arena* arena) {
    arena->offset = 0;
}

/// Create a restorable checkpoint for the arena.
yo_inline yo_ArenaCheckpoint yo_make_arena_checkpoint(yo_Arena* arena) {
    return (yo_ArenaCheckpoint){.arena = arena, .saved_offset = arena->offset};
}

/// Restore the arena state to a given checkpoint.
yo_inline void yo_arena_checkpoint_restore(yo_ArenaCheckpoint checkpoint) {
    checkpoint.arena->offset = checkpoint.saved_offset;
    checkpoint.arena         = NULL;  // Invalidate the checkpoint for further uses.
}

/// Make an arena that owns its memory.
///
/// Since the arena is not aware of the ownership, this function call has to be paired
/// with `free_owned_arena`.
yo_inline yo_Arena yo_make_owned_arena(usize capacity) {
    u8* memory = yo_memory_virtual_alloc(capacity);

    if (memory == NULL) {
        capacity = 0;
    }

    return (yo_Arena){.buf = memory, .capacity = capacity};
}

/// Free the memory of an arena that owns its memory.
///
/// This function should only be called for arenas that where created by `make_owned_arena`.
yo_inline void yo_free_owned_arena(yo_Arena* arena) {
    yo_memory_virtual_free(arena->buf, arena->capacity);
    arena->capacity = 0;
}

// -----------------------------------------------------------------------------
// Buffer with runtime-known size.
// -----------------------------------------------------------------------------

struct yo_api yo_BufferHeader {
    usize element_count;
};
yo_type_alias(yo_BufferHeader, struct yo_BufferHeader);
yo_BufferHeader* yo_impl_buffer_header(void* buffer);

/// Generic type alias for arrays.
#define yo_Buffer(T) T*

/// Make a new buffer with a given fixed element count.
#define yo_make_buffer(arena_ptr, T, count) yo_cast(T*, yo_impl_make_buffer(arena_ptr, count, yo_cast(usize, yo_size_of(T)), yo_cast(u32, yo_align_of(T))))

/// Get the element count of the buffer.
#define yo_buffer_count(buffer) (yo_impl_buffer_header(buffer)->element_count)

//
// Implementation details.
//

#if defined(YO_COMPILER_CLANG)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wcast-align"
#endif

yo_api yo_inline yo_BufferHeader* yo_impl_buffer_header(void* buffer) {
    yo_assert_not_null(buffer);
    return yo_cast(yo_BufferHeader*, yo_cast(u8*, buffer) - yo_size_of(yo_BufferHeader));
}

yo_api yo_inline
yo_Buffer(u8) yo_impl_make_buffer(yo_Arena* arena, usize element_count, usize element_size, u32 element_alignment) {
    yo_assert_not_null(arena);

    usize effective_buffer_size = (element_size * element_count) + yo_size_of(yo_BufferHeader);

    u8* memory = yo_arena_alloc_align(arena, effective_buffer_size, element_alignment);

    if (yo_likely(memory != NULL)) {
        yo_BufferHeader* header = yo_cast(yo_BufferHeader*, memory);
        header->element_count   = element_count;

        memory += yo_size_of(yo_BufferHeader);
    }

    return memory;
}

#if defined(YO_COMPILER_CLANG)
#    pragma clang diagnostic pop
#endif

// -----------------------------------------------------------------------------
// Arrays with runtime-known fixed size capacity and growing count.
// -----------------------------------------------------------------------------

struct yo_api yo_ArrayHeader {
    usize element_capacity;
    usize element_count;
};
yo_type_alias(yo_ArrayHeader, struct yo_ArrayHeader);
yo_ArrayHeader* yo_impl_array_header(void* array);

/// Generic type alias for arrays.
#define yo_Array(T) T*

/// Create a new array with a given fixed element capacity.
#define yo_make_array(arena_ptr, T, capacity) yo_cast(T*, yo_impl_make_array(arena_ptr, capacity, yo_cast(usize, yo_size_of(T)), yo_cast(u32, yo_align_of(T))))

/// Get the maximum (fixed) capacity of the array.
#define yo_array_capacity(array) ((array != NULL) ? yo_impl_array_header(array)->element_capacity : 0)

/// Get the current element count of the array.
#define yo_array_count(array) ((array != NULL) ? yo_impl_array_header(array)->element_count : 0)

/// Push a new element by value to the end of the array.
#define yo_array_push(array, element)                                  \
    do {                                                               \
        yo_impl_array_assert_can_push(array);                          \
        array[yo_impl_array_header(array)->element_count++] = element; \
    } while (0)

/// Copy an existing element instance to the end of the array.
#define yo_array_push_copy(array, element_ptr)                                                                   \
    do {                                                                                                         \
        yo_impl_array_assert_can_push(array);                                                                    \
        yo_assert_msg(yo_size_of(*array) == yo_size_of(*element_ptr), "Array and element sizes don't match.");   \
        usize yo_var_count = yo_array_count(array);                                                              \
        yo_memory_copy(yo_cast(u8*, array + yo_var_count), yo_cast(u8 const*, element_ptr), yo_size_of(*array)); \
        yo_array_count(array) = yo_var_count + 1;                                                                \
    } while (0)

/// Clear the array storage.
yo_api yo_inline void yo_array_clear(yo_Array(void) array) {
    yo_assert_not_null(array);
    yo_impl_array_header(array)->element_count = 0;
}

//
// Implementation details.
//

#if YO_ENABLE_BOUNDS_CHECK
#    define yo_impl_array_assert_can_push(array) 0
#else
#    define yo_impl_array_assert_can_push(array)                               \
        do {                                                                   \
            usize yo_var_capacity = yo_array_capacity(array);                  \
            yo_assert_fmt(                                                     \
                yo_array_count(array) < yo_var_capacity,                       \
                "Cannot push a new element for full array with capacity %zu.", \
                yo_var_capacity);                                              \
        } while (0)
#endif

#if defined(YO_COMPILER_CLANG)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wcast-align"
#endif

yo_api yo_inline yo_ArrayHeader* yo_impl_array_header(void* array) {
    yo_assert_not_null(array);
    return yo_cast(yo_ArrayHeader*, yo_cast(u8*, array) - yo_size_of(yo_ArrayHeader));
}

yo_api yo_inline
yo_Array(u8) yo_impl_make_array(yo_Arena* arena, usize element_capacity, usize element_size, u32 element_alignment) {
    yo_assert_not_null(arena);

    usize effective_array_size = (element_size * element_capacity) + yo_size_of(yo_ArrayHeader);

    u8* memory = yo_arena_alloc_align(arena, effective_array_size, element_alignment);

    if (yo_likely(memory != NULL)) {
        yo_ArrayHeader* header   = yo_cast(yo_ArrayHeader*, memory);
        header->element_capacity = element_capacity;
        header->element_count    = 0;

        memory += yo_size_of(yo_ArrayHeader);
    }

    return memory;
}

#if defined(YO_COMPILER_CLANG)
#    pragma clang diagnostic pop
#endif

// -----------------------------------------------------------------------------
// @TODO: Ring buffer, an array of runtime-known fixed-size infinitely pushable.
// -----------------------------------------------------------------------------

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_MEMORY_H
