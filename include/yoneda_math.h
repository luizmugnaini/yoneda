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
/// Description: Mathematical utilities.
/// File name: yoneda_mat.h
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_MATH_H
#define YONEDA_MATH_H

#include <limits.h>
#include <yoneda_assert.h>
#include <yoneda_core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

#ifndef YO_F32_PI
#    define YO_F32_PI 3.14159265359f
#endif
#ifndef YO_F32_EPSILON
#    define YO_F32_EPSILON 1e-6f
#endif

yo_api yo_inline bool yo_f32_approx_equal(f32 a, f32 b) {
    f32 sub = a - b;
    return (-YO_F32_EPSILON < sub) && (sub < YO_F32_EPSILON);
}

yo_api yo_inline f32 yo_f32_as_radians(f32 deg) {
    return deg * YO_F32_PI / 180.0f;
}

yo_api yo_inline u32 yo_u32_no_wrap_sub(u32 a, u32 b) {
    u32 c = a - b;
    return (c <= a) ? c : 0;
}

yo_api yo_inline u64 yo_u64_no_wrap_sub(u64 a, u64 b) {
    u64 c = a - b;
    return (c <= a) ? c : 0;
}

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_MATH_H
