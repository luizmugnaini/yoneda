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
/// Description: Tests regarding memory handling in general.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include <yoneda/assert.h>
#include <yoneda/core.h>

#include <stdio.h>

#define test_passed() yo_info_fmt("Test %s passed.", yo_source_func())

struct foo {
    u8    a[64];  // offset 0.
    f32   b;      // offset 64.
    bool  c;      // offset 68.
    i32   d;      // offset 72.
    u32   e;      // offset 76.
    usize f;      // offset 80.
    u8    g;      // offset 88.
};

yo_internal void core_type_sizes(void) {
    yo_assert(yo_sizeof(u8) == 1);
    yo_assert(yo_sizeof(u16) == 2);
    yo_assert(yo_sizeof(u32) == 4);
    yo_assert(yo_sizeof(u64) == 8);
    yo_assert(yo_sizeof(usize) == 8);
    yo_assert(yo_sizeof(i8) == 1);
    yo_assert(yo_sizeof(i16) == 2);
    yo_assert(yo_sizeof(i32) == 4);
    yo_assert(yo_sizeof(i64) == 8);
    yo_assert(yo_sizeof(f32) == 4);
    yo_assert(yo_sizeof(f64) == 8);
    yo_assert(yo_sizeof(uptr) == 8);
    yo_assert(yo_sizeof(iptr) == 8);
    test_passed();
}

yo_internal void offset_check(void) {
    yo_assert(yo_offsetof(struct foo, a) == 0);
    yo_assert(yo_offsetof(struct foo, b) == 64);
    yo_assert(yo_offsetof(struct foo, c) == 68);
    yo_assert(yo_offsetof(struct foo, d) == 72);
    yo_assert(yo_offsetof(struct foo, e) == 76);
    yo_assert(yo_offsetof(struct foo, f) == 80);
    yo_assert(yo_offsetof(struct foo, g) == 88);
    test_passed();
}

yo_internal void test_memory(void) {
    core_type_sizes();
    offset_check();
}

#if !defined(YO_TEST_NO_MAIN)
int main(void) {
    test_memory();
    return 0;
}
#endif
