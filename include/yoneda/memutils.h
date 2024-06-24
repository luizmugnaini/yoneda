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
/// Description: Memory handling utility functions.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#ifndef YO_MEMUTILS_H
#define YO_MEMUTILS_H

#include <yoneda/intrinsics.h>
#include <yoneda/types.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

struct yo_memory {
    usize size;
    u8*   buf;
};

struct yo_memory yo_malloc(usize size);
void             yo_free(struct yo_memory mem);

#define yo_memcpy(dst, src, size)  (u8*)memcpy(dst, src, size)
#define yo_memset(dst, val, count) (u8*)memset(dst, val, count)

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YO_MEMUTILS_H
