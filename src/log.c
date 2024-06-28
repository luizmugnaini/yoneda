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
/// Description: Implementation of the message logging utilities.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include <yoneda/log.h>

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <yoneda/intrinsics.h>

/// Formatting string common to all logging procedures.
#define YO_LOG_FMT "%s [%s:%d] %s\n"

/// Log level indicator string.
yo_internal strptr const YO_LOG_LEVEL_STR[YO_LOG_LEVEL_COUNT + 1] = {
#if !defined(YO_DISABLE_ANSI_COLORS)  // clang-format off
    "\x1b[1;41m[FATAL]\x1b[0m",
    "\x1b[1;31m[ERROR]\x1b[0m",
    "\x1b[1;33m[WARNING]\x1b[0m",
    "\x1b[1;32m[INFO]\x1b[0m",
    "\x1b[1;34m[DEBUG]\x1b[0m",
#   else
    "[FATAL]",
    "[ERROR]",
    "[WARNING]",
    "[INFO]",
    "[DEBUG]",
#endif  // clang-format on
};

yo_inline yo_internal FILE* yo_impl_get_stream(enum yo_stream stream) {
    FILE* handle;
    switch (stream) {
        case YO_STREAM_ERROR: handle = stderr; break;
        case YO_STREAM_OUT:   handle = stdout; break;
        default:              assert(false && "Invalid stream.");
    }
    return handle;
}

void yo_log(enum yo_stream stream, strptr file, u32 line_num, enum yo_log_level lvl, strptr msg) {
    yo_discard(fprintf(
        yo_impl_get_stream(stream),
        YO_LOG_FMT,
        YO_LOG_LEVEL_STR[lvl],
        file,
        line_num,
        msg));
}

void yo_log_fmt(
    enum yo_stream    stream,
    strptr            file,
    u32               line_num,
    enum yo_log_level lvl,
    strptr            fmt,
    ...) {
    char    msg_buf[YO_MAX_LEN_FORMATTED_MSG];
    va_list args;
    va_start(args, fmt);
    {
        // Format the message with the given args.
        i32 char_count = vsnprintf(msg_buf, YO_MAX_LEN_FORMATTED_MSG, fmt, args);
        assert(char_count != -1 && "snprintf unable to parse the format string and arguments");

        // Stamp the message with a null-terminator character.
        usize msg_len    = yo_min((usize)char_count, YO_MAX_LEN_FORMATTED_MSG);
        msg_buf[msg_len] = 0;
    }
    va_end(args);

    yo_discard(fprintf(
        yo_impl_get_stream(stream),
        YO_LOG_FMT,
        YO_LOG_LEVEL_STR[lvl],
        file,
        line_num,
        msg_buf));
}
