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
/// Description: Implementation of the IO stream utilities.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include "yo_io.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define YO_MAX_LEN_MSG 8192

static char const* impl_log_level_str(enum yo_log_level lvl) {
    char const* s;
    switch (lvl) {
        case YO_LOG_FATAL:   s = "\x1b[1;41mFATAL\x1b[0m";
        case YO_LOG_ERROR:   s = "\x1b[1;31mERROR\x1b[0m";
        case YO_LOG_WARNING: s = "\x1b[1;33mWARNING\x1b[0m";
        case YO_LOG_INFO:    s = "\x1b[1;32mINFO\x1b[0m";
        case YO_LOG_DEBUG:   s = "\x1b[1;34mDEBUG\x1b[0m";
    }
    return s;
}

void yo_impl_log(
    struct yo_src_info info,
    enum yo_log_level  lvl,
    char const*        msg) {
    (void)fprintf(
        stderr,
        "[%s][%s:%d] %s\n",
        impl_log_level_str(lvl),
        info.file,
        info.line,
        msg);
}

void yo_impl_log_fmt(
    struct yo_src_info info,
    enum yo_log_level  lvl,
    char const*        fmt,
    ...) {
    char    msg[YO_MAX_LEN_MSG];
    va_list args;
    va_start(args, fmt);
    {
        int char_count = snprintf(msg, YO_MAX_LEN_MSG, fmt, args);
        assert(
            char_count != -1 &&
            "snprintf unable to parse the format string and arguments");
        msg[char_count] = 0;
    }
    va_end(args);

    (void)fprintf(
        stderr,
        "[%s][%s:%d] %s\n",
        impl_log_level_str(lvl),
        info.file,
        info.line,
        msg);
}
