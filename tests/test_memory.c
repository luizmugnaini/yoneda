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
/// Description: Tests regarding memory handling in general.
/// File name: test_memory.c
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include <yoneda_assert.h>
#include <yoneda_core.h>

#include <stdio.h>

#define test_passed() yo_log_info_fmt("Test %s passed.", yo_source_function_name())

yo_internal void core_type_sizes(void) {
    yo_constexpr_assert(yo_size_of(u8) == 1);
    yo_constexpr_assert(yo_size_of(u16) == 2);
    yo_constexpr_assert(yo_size_of(u32) == 4);
    yo_constexpr_assert(yo_size_of(u64) == 8);
    yo_constexpr_assert(yo_size_of(usize) == 8);
    yo_constexpr_assert(yo_size_of(i8) == 1);
    yo_constexpr_assert(yo_size_of(i16) == 2);
    yo_constexpr_assert(yo_size_of(i32) == 4);
    yo_constexpr_assert(yo_size_of(i64) == 8);
    yo_constexpr_assert(yo_size_of(f32) == 4);
    yo_constexpr_assert(yo_size_of(f64) == 8);
    yo_constexpr_assert(yo_size_of(uptr) == 8);
    yo_constexpr_assert(yo_size_of(iptr) == 8);
    test_passed();
}

yo_internal void offset_check(void) {
    struct foo {
        u8    a[64];  // offset 0.
        f32   b;      // offset 64.
        bool  c;      // offset 68.
        i32   d;      // offset 72.
        u32   e;      // offset 76.
        usize f;      // offset 80.
        u8    g;      // offset 88.
    };

    yo_constexpr_assert(yo_offset_of(struct foo, a) == 0);
    yo_constexpr_assert(yo_offset_of(struct foo, b) == 64);
    yo_constexpr_assert(yo_offset_of(struct foo, c) == 68);
    yo_constexpr_assert(yo_offset_of(struct foo, d) == 72);
    yo_constexpr_assert(yo_offset_of(struct foo, e) == 76);
    yo_constexpr_assert(yo_offset_of(struct foo, f) == 80);
    yo_constexpr_assert(yo_offset_of(struct foo, g) == 88);
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
