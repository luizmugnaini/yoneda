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
/// Description: Implementation of runtime known fixed size arrays.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_ARRAY_H
#define YONEDA_ARRAY_H

#include <yoneda_core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

struct yo_api yo_ArrayHeader {
    usize count;
};
yo_type_alias(yo_ArrayHeader, struct yo_ArrayHeader);

struct yo_api yo_Array {
    yo_ArrayHeader header;
    u8*            buf;
}

yo_api yo_inline u8*
yo_make_array_bytes(yo_Arena* arena, usize size_bytes) {
    u8* memory = yo_arena_alloc(arena, size_bytes + yo_sizeof(yo_ArrayHeader));
    yo_assert_msg(memory != NULL, "Failed to allocate memory.");

    yo_ArrayHeader* header = yo_cast(yo_ArrayHeader*, memory);
    header->count          = size_bytes;

    return memory + yo_sizeof(yo_ArrayHeader);
}

#define yo_make_array(arena_ptr, T, count) yo_cast(T*, yo_make_array_bytes(arena_ptr, sizeof(T) * count))
#define yo_array_count(array_ptr)          ((yo_cast(u8*, array_ptr) - yo_sizeof(yo_ArrayHeader))->count)

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_ARRAY_H
