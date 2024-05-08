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
/// Description: IO stream utilities.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_IO_H
#define YONEDA_IO_H

#include <assert.h>
#include <stdint.h>
#include "yo_intrinsics.h"

YO_START_C_HEADER

enum yo_log_level {
    YO_LOG_FATAL   = 0x0000,
    YO_LOG_ERROR   = 0x0001,
    YO_LOG_WARNING = 0x0002,
    YO_LOG_INFO    = 0x0003,
    YO_LOG_DEBUG   = 0x0004,
};

struct yo_src_info {
    char const* file;
    uint32_t    line;
};

#define YO_SRC_INFO                        \
    (struct yo_src_info) {                 \
        .file = __FILE__, .line = __LINE__ \
    }

/// Log a message to the standard error stream.
void yo_impl_log(
    struct yo_src_info info,
    enum yo_log_level  lvl,
    char const*        msg);

/// Log a formatted message to the standard error stream.
void yo_impl_log_fmt(
    struct yo_src_info info,
    enum yo_log_level  lvl,
    char const*        fmt,
    ...);

#define yo_log(lvl_, msg_)                  \
    do {                                    \
        enum yo_log_level lvl = lvl_;       \
        char const*       msg = msg_;       \
        yo_impl_log(YO_SRC_INFO, lvl, msg); \
    } while (0);

#define yo_log_fmt(lvl_, msg_, ...)                          \
    do {                                                     \
        enum yo_log_level lvl = lvl_;                        \
        char const*       msg = msg_;                        \
        yo_impl_log_fmt(YO_SRC_INFO, lvl, msg, __VA_ARGS__); \
    } while (0);


YO_END_C_HEADER

#endif  // YONEDA_IO_H
