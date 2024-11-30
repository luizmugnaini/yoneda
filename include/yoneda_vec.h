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
/// Description: Vector types and their operations.
/// File name: yoneda_vec.h
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_VEC_H
#define YONEDA_VEC_H

#include <math.h>
#include <yoneda_core.h>
#include <yoneda_math.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

// @TODO: I should write all of the code below with SIMD support...

// -----------------------------------------------------------------------------
// Floating point vectors.
// -----------------------------------------------------------------------------

/// 2-dimensional vector in floating-point space.
struct yo_api yo_Vec2 {
    f32 x;
    f32 y;
};
yo_type_alias(yo_Vec2, struct yo_Vec2);

/// Check if the components of the vector are inside the floating point zero range.
yo_api yo_inline bool yo_vec2_is_zero(yo_Vec2 v) {
    return yo_f32_approx_equal(v.x, 0.0f) && yo_f32_approx_equal(v.y, 0.0f);
}

/// Get the normalized vector.
///
/// Note: The vector is assumed to be non-zero, otherwise this will result in UB.
yo_api yo_inline yo_Vec2 yo_vec2_normalized(yo_Vec2 v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y);

    if (yo_unlikely(len < YO_F32_EPSILON)) {
        return yo_make_default(yo_Vec2);
    }

    return (yo_Vec2){v.x / len, v.y / len};
}

yo_api yo_inline f32 yo_vec2_dot(yo_Vec2 lhs, yo_Vec2 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

yo_api yo_inline bool yo_vec2_is_to_the_left_of(yo_Vec2 lhs, yo_Vec2 rhs) {
    return ((rhs.x * lhs.y - rhs.y * lhs.x) >= 0.0f);
}

yo_api yo_inline void yo_vec2_eq_add(yo_Vec2* lhs, yo_Vec2 rhs) {
    lhs->x += rhs.x;
    lhs->y += rhs.y;
}
yo_api yo_inline void yo_vec2_eq_sub(yo_Vec2* lhs, yo_Vec2 rhs) {
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
}
yo_api yo_inline void yo_vec2_eq_mul_vec2(yo_Vec2* lhs, yo_Vec2 rhs) {
    lhs->x *= rhs.x;
    lhs->y *= rhs.y;
}
yo_api yo_inline void yo_vec2_eq_mul(yo_Vec2* lhs, f32 scalar) {
    lhs->x *= scalar;
    lhs->y *= scalar;
}

yo_api yo_inline yo_Vec2 yo_vec2_add(yo_Vec2 lhs, yo_Vec2 rhs) {
    return (yo_Vec2){lhs.x + rhs.x, lhs.y + rhs.y};
}
yo_api yo_inline yo_Vec2 yo_vec2_sub(yo_Vec2 lhs, yo_Vec2 rhs) {
    return (yo_Vec2){lhs.x - rhs.x, lhs.y - rhs.y};
}
yo_api yo_inline yo_Vec2 yo_vec2_mul_vec2(yo_Vec2 lhs, yo_Vec2 rhs) {
    return (yo_Vec2){lhs.x * rhs.x, lhs.y * rhs.y};
}
yo_api yo_inline yo_Vec2 yo_vec2_mul(yo_Vec2 v, f32 scalar) {
    return (yo_Vec2){v.x * scalar, v.y * scalar};
}
yo_api yo_inline yo_Vec2 yo_vec2_neg(yo_Vec2 v) {
    return (yo_Vec2){-v.x, -v.y};
}

/// 3-dimensional vector in floating-point space.
struct yo_api yo_Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
yo_type_alias(yo_Vec3, struct yo_Vec3);

/// Check if the components of the vector are inside the floating point zero range.
yo_api yo_inline bool yo_vec3_is_zero(yo_Vec3 v) {
    return yo_f32_approx_equal(v.x, 0.0f) &&
           yo_f32_approx_equal(v.y, 0.0f) &&
           yo_f32_approx_equal(v.z, 0.0f);
}

/// Get the normalized vector.
yo_api yo_inline yo_Vec3 yo_vec3_normalized(yo_Vec3 v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

    if (yo_unlikely(yo_f32_approx_equal(len, 0.0f))) {
        return yo_make_default(yo_Vec3);
    }

    return (yo_Vec3){v.x / len, v.y / len, v.z / len};
}

yo_api yo_inline f32 yo_vec3_dot(yo_Vec3 lhs, yo_Vec3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

yo_api yo_inline void yo_vec3_eq_add(yo_Vec3* lhs, yo_Vec3 rhs) {
    lhs->x += rhs.x;
    lhs->y += rhs.y;
    lhs->z += rhs.z;
}
yo_api yo_inline void yo_vec3_eq_sub(yo_Vec3* lhs, yo_Vec3 rhs) {
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
    lhs->z -= rhs.z;
}
yo_api yo_inline void yo_vec3_eq_mul_vec3(yo_Vec3* lhs, yo_Vec3 rhs) {
    lhs->x *= rhs.x;
    lhs->y *= rhs.y;
    lhs->z *= rhs.z;
}
yo_api yo_inline void yo_vec3_eq_mul(yo_Vec3* lhs, f32 scalar) {
    lhs->x *= scalar;
    lhs->y *= scalar;
    lhs->z *= scalar;
}

yo_api yo_inline yo_Vec3 yo_vec3_add(yo_Vec3 lhs, yo_Vec3 rhs) {
    return (yo_Vec3){lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
yo_api yo_inline yo_Vec3 yo_vec3_sub(yo_Vec3 lhs, yo_Vec3 rhs) {
    return (yo_Vec3){lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
yo_api yo_inline yo_Vec3 yo_vec3_mul_vec3(yo_Vec3 lhs, yo_Vec3 rhs) {
    return (yo_Vec3){lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}
yo_api yo_inline yo_Vec3 yo_vec3_cross(yo_Vec3 lhs, yo_Vec3 rhs) {
    return (yo_Vec3){
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
}
yo_api yo_inline yo_Vec3 yo_vec3_mul(yo_Vec3 v, f32 scalar) {
    return (yo_Vec3){v.x * scalar, v.y * scalar, v.z * scalar};
}
yo_api yo_inline yo_Vec3 yo_vec3_neg(yo_Vec3 v) {
    return (yo_Vec3){-v.x, -v.y, -v.z};
}

/// 4-dimensional vector in floating-point space.
// @TODO Write associated procedures.
struct yo_api yo_Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
yo_type_alias(yo_Vec4, struct yo_Vec4);

// -----------------------------------------------------------------------------
// Integer vectors.
// -----------------------------------------------------------------------------

/// 2-dimensional vector in integer space.
struct yo_api yo_IVec2 {
    i32 x;
    i32 y;
};
yo_type_alias(yo_IVec2, struct yo_IVec2);

/// Check if all components of the vector are zero.
yo_api yo_inline bool yo_ivec2_is_zero(yo_IVec2 v) {
    return (v.x == 0) && (v.y == 0);
}

/// Get the normalized vector in floating point coordinates.
yo_api yo_inline yo_Vec2 yo_ivec2_normalized(yo_IVec2 v) {
    f32 len = sqrtf(yo_cast(f32, v.x * v.x + v.y * v.y));

    if (yo_unlikely(len < YO_F32_EPSILON)) {
        return yo_make_default(yo_Vec2);
    }

    return (yo_Vec2){yo_cast(f32, v.x) / len, yo_cast(f32, v.y) / len};
}

yo_api yo_inline i32 yo_ivec2_dot(yo_IVec2 lhs, yo_IVec2 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

yo_api yo_inline void yo_ivec2_eq_add(yo_IVec2* lhs, yo_IVec2 rhs) {
    lhs->x += rhs.x;
    lhs->y += rhs.y;
}
yo_api yo_inline void yo_ivec2_eq_sub(yo_IVec2* lhs, yo_IVec2 rhs) {
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
}
yo_api yo_inline void yo_ivec2_eq_mul_ivec2(yo_IVec2* lhs, yo_IVec2 rhs) {
    lhs->x *= rhs.x;
    lhs->y *= rhs.y;
}
yo_api yo_inline void yo_ivec2_eq_mul(yo_IVec2* lhs, i32 scalar) {
    lhs->x *= scalar;
    lhs->y *= scalar;
}
yo_api yo_inline yo_IVec2 yo_ivec2_add(yo_IVec2 lhs, yo_IVec2 rhs) {
    return (yo_IVec2){lhs.x + rhs.x, lhs.y + rhs.y};
}
yo_api yo_inline yo_IVec2 yo_ivec2_sub(yo_IVec2 lhs, yo_IVec2 rhs) {
    return (yo_IVec2){lhs.x - rhs.x, lhs.y - rhs.y};
}
yo_api yo_inline yo_IVec2 yo_ivec2_mul_ivec2(yo_IVec2 lhs, yo_IVec2 rhs) {
    return (yo_IVec2){lhs.x * rhs.x, lhs.y * rhs.y};
}
yo_api yo_inline yo_IVec2 yo_ivec2_mul(yo_IVec2 lhs, i32 scalar) {
    return (yo_IVec2){lhs.x * scalar, lhs.y * scalar};
}
yo_api yo_inline yo_IVec2 yo_ivec2_neg(yo_IVec2 v) {
    return (yo_IVec2){-v.x, -v.y};
}
yo_api yo_inline bool yo_ivec2_equals(yo_IVec2 lhs, yo_IVec2 rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

/// 3-dimensional vector in integer space.
struct yo_api yo_IVec3 {
    i32 x;
    i32 y;
    i32 z;
};
yo_type_alias(yo_IVec3, struct yo_IVec3);

/// Check if all components of the vector are zero.
yo_api yo_inline bool yo_ivec3_is_zero(yo_IVec3 v) {
    return (v.x == 0) && (v.y == 0) & (v.z == 0);
}

/// Get the normalized vector in floating point coordinates.
yo_api yo_inline yo_Vec3 yo_ivec3_normalized(yo_IVec3 v) {
    f32 len = sqrtf(yo_cast(f32, v.x * v.x + v.y * v.y + v.z * v.z));

    if (yo_unlikely(len < YO_F32_EPSILON)) {
        return yo_make_default(yo_Vec3);
    }

    return (yo_Vec3){yo_cast(f32, v.x) / len, yo_cast(f32, v.y) / len, yo_cast(f32, v.z) / len};
}

yo_api yo_inline i32 yo_ivec3_dot(yo_IVec3 lhs, yo_IVec3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

yo_api yo_inline void yo_ivec3_eq_add(yo_IVec3* lhs, yo_IVec3 rhs) {
    lhs->x += rhs.x;
    lhs->y += rhs.y;
    lhs->z += rhs.y;
}
yo_api yo_inline void yo_ivec3_eq_sub(yo_IVec3* lhs, yo_IVec3 rhs) {
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
    lhs->z -= rhs.z;
}
yo_api yo_inline void yo_ivec3_eq_mul_ivec3(yo_IVec3* lhs, yo_IVec3 rhs) {
    lhs->x *= rhs.x;
    lhs->y *= rhs.y;
    lhs->z *= rhs.z;
}
yo_api yo_inline void yo_ivec3_eq_mul(yo_IVec3* lhs, i32 scalar) {
    lhs->x *= scalar;
    lhs->y *= scalar;
    lhs->z *= scalar;
}

yo_api yo_inline yo_IVec3 yo_ivec3_add(yo_IVec3 lhs, yo_IVec3 rhs) {
    return (yo_IVec3){lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
yo_api yo_inline yo_IVec3 yo_ivec3_sub(yo_IVec3 lhs, yo_IVec3 rhs) {
    return (yo_IVec3){lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
yo_api yo_inline yo_IVec3 yo_ivec3_mul_ivec2(yo_IVec3 lhs, yo_IVec3 rhs) {
    return (yo_IVec3){lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}
yo_api yo_inline yo_IVec3 yo_ivec3_cross(yo_IVec3 lhs, yo_IVec3 rhs) {
    return (yo_IVec3){
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
}
yo_api yo_inline yo_IVec3 yo_ivec3_mul(yo_IVec3 lhs, i32 scalar) {
    return (yo_IVec3){lhs.x * scalar, lhs.y * scalar, lhs.z * scalar};
}
yo_api yo_inline yo_IVec3 yo_ivec3_neg(yo_IVec3 v) {
    return (yo_IVec3){-v.x, -v.y, -v.z};
}
yo_api yo_inline bool yo_ivec3_equals(yo_IVec3 lhs, yo_IVec3 rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

// -----------------------------------------------------------------------------
// Floating point matrices.
// -----------------------------------------------------------------------------

// @TODO: implement Mat2 methods.
struct yo_api yo_Mat2 {
    f32 buf[4];
};
yo_type_alias(yo_Mat2, struct yo_Mat2);

/// Row-major 3-dimensional square matrix in floating-point space.
struct yo_api yo_Mat3 {
    f32 buf[9];
};
yo_type_alias(yo_Mat3, struct yo_Mat3);

/// Get a reference to the matrix component whose row is `r` and column is `c`.
yo_api yo_inline f32* yo_mat3_at(yo_Mat3* m, u32 r, u32 c) {
    yo_assert_msg(r <= 3, "Row outside range.");
    yo_assert_msg(c <= 3, "Column outside range.");

    return &m->buf[r + c * 3];
}

/// Create an identity matrix.
yo_api yo_inline yo_Mat3 yo_mat3_id(void) {
    // clang-format off
    return (yo_Mat3){
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
    // clang-format on
}

/// Create the change of basis transformation for a given triple of basis vectors.
yo_api yo_inline yo_Mat3 yo_mat3_change_of_basis(yo_Vec3 v1, yo_Vec3 v2, yo_Vec3 v3) {
    // @TODO: should we check if v1, v2, v3 form an orthogonal triple?
    // clang-format off
    return (yo_Mat3){
        v1.x, v2.x, v3.x,
        v1.y, v2.y, v3.y,
        v1.z, v2.z, v3.z,
    };
    // clang-format on
}

/// Create the general Tait-Bryan intrinsic rotation matrix.
///
/// Parameters: All angle parameters are assumed to be in radians.
///     * rot_x: The angle to rotate about the x axis (aka roll angle).
///     * rot_y: The angle to rotate about the y axis (aka pitch angle).
///     * rot_z: The angle to rotate about the z axis (aka yaw angle).
yo_api yo_inline yo_Mat3 yo_mat3_rotation_tb(f32 rot_x, f32 rot_y, f32 rot_z) {
    f32 xsin = sinf(rot_x);
    f32 ysin = sinf(rot_y);
    f32 zsin = sinf(rot_z);
    f32 xcos = cosf(rot_x);
    f32 ycos = cosf(rot_y);
    f32 zcos = cosf(rot_z);

    // clang-format off
    return (yo_Mat3){
        (zcos * ycos), (zcos * ysin * xsin - zsin * xcos), (xcos * ysin * xcos + zsin * xsin),
        (zsin * ycos), (zsin * ysin * xsin + zcos * xcos), (zsin * ysin * xcos - zcos * xsin),
           (-ysin),               (ycos * xsin),                      (ycos * xcos),
    };
    // clang-format on
}

// @TODO: rotation with Euler angles.

/// Column-major 3-dimensional square matrix in floating-point space.
struct yo_api yo_ColMat3 {
    f32 buf[9];
};
yo_type_alias(yo_ColMat3, struct yo_ColMat3);

/// Get a reference to the matrix component whose row is `r` and column is `c`.
yo_api yo_inline f32* yo_colmat3_at(yo_ColMat3* m, u32 r, u32 c) {
    yo_assert_msg(r <= 3, "Row outside range.");
    yo_assert_msg(c <= 3, "Column outside range.");

    return &m->buf[r * 3 + c];
}

/// Create an identity matrix.
yo_api yo_inline yo_ColMat3 yo_colmat3_id(void) {
    // clang-format off
    return (yo_ColMat3){
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };
    // clang-format on
}
/// Column-major 4-dimensional square matrix in floating-point space.
struct yo_api yo_ColMat4 {
    f32 buf[16];
};
yo_type_alias(yo_ColMat4, struct yo_ColMat4);

/// Get the matrix component whose row is `r` and column is `c`.
yo_api yo_inline f32* yo_colmat4_at(yo_ColMat4* m, u32 r, u32 c) {
    yo_assert_msg(r <= 4, "Row outside range.");
    yo_assert_msg(c <= 4, "Column outside range.");

    return &m->buf[r * 4 + c];
}

/// Create an identity matrix.
yo_api yo_inline yo_ColMat4 yo_colmat4_id(void) {
    // clang-format off
    return (yo_ColMat4){
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    // clang-format on
}

/// Create a scaling matrix for 3D space.
yo_api yo_inline yo_ColMat4 yo_colmat4_scale(yo_Vec3 scaling) {
    // clang-format off
    return (yo_ColMat4){
        scaling.x,    0.0f,      0.0f,   0.0f,
           0.0f,   scaling.y,    0.0f,   0.0f,
           0.0f,      0.0f,   scaling.z, 0.0f,
           0.0f,      0.0f,      0.0f,   1.0f,
    };
    // clang-format on
}

/// Create the translation matrix for a given displacement in 3D space.
yo_api yo_inline yo_ColMat4 yo_colmat4_translation(yo_Vec3 dx_dy_dz) {
    // clang-format off
    return (yo_ColMat4){
           1.0f,       0.0f,       0.0f,    0.0f,
           0.0f,       1.0f,       0.0f,    0.0f,
           0.0f,       0.0f,       1.0f,    0.0f,
        dx_dy_dz.x, dx_dy_dz.y, dx_dy_dz.z, 1.0f,
    };
    // clang-format on
}

/// Create the right-handed world-to-view coordinate transformation with respect to a
/// viewing directional vector.
///
/// Parameters:
///     * eye: The position of the camera's projection center.
///     * target: The direction to look at.
///     * view_up: Orientational vector with the up direction of the camera coordinate
///                system.
yo_api yo_inline yo_ColMat4 yo_colmat4_view_direction_rh(yo_Vec3 eye, yo_Vec3 view_direction, yo_Vec3 view_up) {
    yo_Vec3 forward = yo_vec3_normalized(view_direction);
    yo_Vec3 right   = yo_vec3_normalized(yo_vec3_cross(forward, view_up));
    yo_Vec3 up      = yo_vec3_cross(right, forward);

    // clang-format off
    return (yo_ColMat4){
             right.x,                     up.x,                  -forward.x,         0.0f,
             right.y,                     up.y,                  -forward.y,         0.0f,
             right.z,                     up.z,                  -forward.z,         0.0f,
         -yo_vec3_dot(right, eye), -yo_vec3_dot(up, eye), yo_vec3_dot(forward, eye), 1.0f,
    };
    // clang-format on
}

/// Create the right-handed world-to-view coordinate transformation with respect to a
/// target.
///
/// Parameters:
///     * eye: The position of the camera's projection center.
///     * target: The target that the camera should look at.
///     * view_up: Orientational vector with the up direction of the camera coordinate
///                system.
yo_api yo_inline yo_ColMat4 yo_colmat4_look_at_rh(yo_Vec3 eye, yo_Vec3 target, yo_Vec3 view_up) {
    return yo_colmat4_view_direction_rh(eye, yo_vec3_sub(target, eye), view_up);
}

/// Create the right-handed perspective projection transformation.
///
/// This transformation assumes the following constraints:
///     * The x coordinates go from -1 to 1 (left to right direction).
///     * The y coordinates go from -1 to 1 (up to down direction, distinct from OpenGL convention).
///     * The z coordinates go from 0 to 1 (forward to back direction) distinct from OpenGL convention.
/// This transformation is suited for programs working with Vulkan, which assumes the exact
/// constraints seen above.
yo_api yo_inline yo_ColMat4
yo_colmat4_perspective_projection_rhzo(f32 fovy, f32 aspect, f32 near_plane, f32 far_plane) {
    f32 tan_hfovy = tanf(fovy * 0.5f);

    // clang-format off
    return (yo_ColMat4){
        1 / (aspect * tan_hfovy),      0.0f,                               0.0f,                             0.0f,
                 0.0f,            -(1 / tan_hfovy),                        0.0f,                             0.0f,
                 0.0f,                 0.0f,            -(far_plane / (far_plane - near_plane)),            -1.0f,
                 0.0f,                 0.0f,        -((far_plane * near_plane) / (far_plane - near_plane)),  0.0f,
    };
    // clang-format on
}

/// Create the right-handed orthogonal projection transformation.
///
/// This transformation assumes the following constraints:
///     * The x coordinates go from -1 to 1 (left to right direction).
///     * The y coordinates go from -1 to 1 (up to down direction, distinct from OpenGL convention).
///     * The z coordinates go from 0 to 1 (forward to back direction) distinct from OpenGL convention.
/// This transformation is suited for programs working with Vulkan, which assumes the exact
/// constraints seen above.
yo_api yo_inline yo_ColMat4 yo_colmat4_orthographic_projection_rhzo(
    f32 left,
    f32 right,
    f32 bottom,
    f32 top,
    f32 near_plane,
    f32 far_plane) {
    // clang-format off
    return (yo_ColMat4){
             2.0f / (right - left),                  0.0f,                            0.0f,                      0.0f,
                      0.0f,                  2.0f / (top - bottom),                   0.0f,                      0.0f,
                      0.0f,                          0.0f,                  1.0f / (near_plane - far_plane),     0.0f,
        (left + right) / (left - right), (bottom + top) / (bottom - top), near_plane / (near_plane - far_plane), 1.0f,
    };
    // clang-format on
}

// @TODO:
// - Perspective_projection_rhno following OpenGL conventions.
// - Orthographic_projection_rhno following OpenGL conventions.
// - Should we really copy yo_ColMat??

/// Left-multiply a 2D vector by a 2D square matrix.
yo_api yo_inline yo_Vec2 yo_mat2_mul_vec2(yo_Mat2 m, yo_Vec2 v) {
    return (yo_Vec2){
        (m.buf[0] * v.x) + (m.buf[1] * v.y),
        (m.buf[2] * v.x) + (m.buf[3] * v.y),
    };
}

/// Left-multiply a 3D vector by a 3D square matrix.
yo_api yo_inline yo_Vec3 yo_mat3_mul_vec3(yo_Mat3 m, yo_Vec3 v) {
    return (yo_Vec3){
        (m.buf[0] * v.x) + (m.buf[1] * v.y) + (m.buf[2] * v.z),
        (m.buf[3] * v.x) + (m.buf[4] * v.y) + (m.buf[5] * v.z),
        (m.buf[6] * v.x) + (m.buf[7] * v.y) + (m.buf[8] * v.z),
    };
}

/// Multiply a pair of 3D square matrices.
yo_api yo_inline yo_Mat3 yo_mat3_mul_mat3(yo_Mat3 lhs, yo_Mat3 rhs) {
    return (yo_Mat3){
        // Row 1.
        (lhs.buf[0] * rhs.buf[0]) + (lhs.buf[1] * rhs.buf[3]) + (lhs.buf[2] * rhs.buf[6]),
        (lhs.buf[0] * rhs.buf[1]) + (lhs.buf[1] * rhs.buf[4]) + (lhs.buf[2] * rhs.buf[7]),
        (lhs.buf[0] * rhs.buf[2]) + (lhs.buf[1] * rhs.buf[5]) + (lhs.buf[2] * rhs.buf[8]),
        // Row 2.
        (lhs.buf[3] * rhs.buf[0]) + (lhs.buf[4] * rhs.buf[3]) + (lhs.buf[5] * rhs.buf[6]),
        (lhs.buf[3] * rhs.buf[1]) + (lhs.buf[4] * rhs.buf[4]) + (lhs.buf[5] * rhs.buf[7]),
        (lhs.buf[3] * rhs.buf[2]) + (lhs.buf[4] * rhs.buf[5]) + (lhs.buf[5] * rhs.buf[8]),
        // Row 3.
        (lhs.buf[6] * rhs.buf[0]) + (lhs.buf[7] * rhs.buf[3]) + (lhs.buf[8] * rhs.buf[6]),
        (lhs.buf[6] * rhs.buf[1]) + (lhs.buf[7] * rhs.buf[4]) + (lhs.buf[8] * rhs.buf[7]),
        (lhs.buf[6] * rhs.buf[2]) + (lhs.buf[7] * rhs.buf[5]) + (lhs.buf[8] * rhs.buf[8]),
    };
}

/// Left-multiply a 4D vector by a 4D square column-major matrix.
yo_api yo_inline yo_Vec4 yo_colmat4_mul_vec4(yo_ColMat4 m, yo_Vec4 v) {
    return (yo_Vec4){
        (m.buf[0] * v.x) + (m.buf[4] * v.y) + (m.buf[8] * v.z) + (m.buf[12] * v.w),
        (m.buf[1] * v.x) + (m.buf[5] * v.y) + (m.buf[9] * v.z) + (m.buf[13] * v.w),
        (m.buf[2] * v.x) + (m.buf[6] * v.y) + (m.buf[10] * v.z) + (m.buf[14] * v.w),
        (m.buf[3] * v.x) + (m.buf[7] * v.y) + (m.buf[11] * v.z) + (m.buf[15] * v.w),
    };
}

/// Multiply a pair of 4D square column-major matrices.
yo_api yo_inline yo_ColMat4 yo_colmat4_mul_colmat4(yo_ColMat4 lhs, yo_ColMat4 rhs) {
    // clang-format off
    return (yo_ColMat4){
        // Row 1.
        (lhs.buf[0] * rhs.buf[0]) + (lhs.buf[1] * rhs.buf[4]) + (lhs.buf[2] * rhs.buf[8])  + (lhs.buf[3] * rhs.buf[12]),
        (lhs.buf[0] * rhs.buf[1]) + (lhs.buf[1] * rhs.buf[5]) + (lhs.buf[2] * rhs.buf[9])  + (lhs.buf[3] * rhs.buf[13]),
        (lhs.buf[0] * rhs.buf[2]) + (lhs.buf[1] * rhs.buf[6]) + (lhs.buf[2] * rhs.buf[10]) + (lhs.buf[3] * rhs.buf[14]),
        (lhs.buf[0] * rhs.buf[3]) + (lhs.buf[1] * rhs.buf[7]) + (lhs.buf[2] * rhs.buf[11]) + (lhs.buf[3] * rhs.buf[15]),
        // Row 2.
        (lhs.buf[4] * rhs.buf[0]) + (lhs.buf[5] * rhs.buf[4]) + (lhs.buf[6] * rhs.buf[8])  + (lhs.buf[7] * rhs.buf[12]),
        (lhs.buf[4] * rhs.buf[1]) + (lhs.buf[5] * rhs.buf[5]) + (lhs.buf[6] * rhs.buf[9])  + (lhs.buf[7] * rhs.buf[13]),
        (lhs.buf[4] * rhs.buf[2]) + (lhs.buf[5] * rhs.buf[6]) + (lhs.buf[6] * rhs.buf[10]) + (lhs.buf[7] * rhs.buf[14]),
        (lhs.buf[4] * rhs.buf[3]) + (lhs.buf[5] * rhs.buf[7]) + (lhs.buf[6] * rhs.buf[11]) + (lhs.buf[7] * rhs.buf[15]),
        // Row 3.
        (lhs.buf[8] * rhs.buf[0]) + (lhs.buf[9] * rhs.buf[4]) + (lhs.buf[10] * rhs.buf[8])  + (lhs.buf[11] * rhs.buf[12]),
        (lhs.buf[8] * rhs.buf[1]) + (lhs.buf[9] * rhs.buf[5]) + (lhs.buf[10] * rhs.buf[9])  + (lhs.buf[11] * rhs.buf[13]),
        (lhs.buf[8] * rhs.buf[2]) + (lhs.buf[9] * rhs.buf[6]) + (lhs.buf[10] * rhs.buf[10]) + (lhs.buf[11] * rhs.buf[14]),
        (lhs.buf[8] * rhs.buf[3]) + (lhs.buf[9] * rhs.buf[7]) + (lhs.buf[10] * rhs.buf[11]) + (lhs.buf[11] * rhs.buf[15]),
        // Row 4.
        (lhs.buf[12] * rhs.buf[0]) + (lhs.buf[13] * rhs.buf[4]) + (lhs.buf[14] * rhs.buf[8])  + (lhs.buf[15] * rhs.buf[12]),
        (lhs.buf[12] * rhs.buf[1]) + (lhs.buf[13] * rhs.buf[5]) + (lhs.buf[14] * rhs.buf[9])  + (lhs.buf[15] * rhs.buf[13]),
        (lhs.buf[12] * rhs.buf[2]) + (lhs.buf[13] * rhs.buf[6]) + (lhs.buf[14] * rhs.buf[10]) + (lhs.buf[15] * rhs.buf[14]),
        (lhs.buf[12] * rhs.buf[3]) + (lhs.buf[13] * rhs.buf[7]) + (lhs.buf[14] * rhs.buf[11]) + (lhs.buf[15] * rhs.buf[15]),
    };
    // clang-format on
}

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_VEC_H
