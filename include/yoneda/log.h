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
/// Description: Message logging utilities.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_IO_H
#define YONEDA_IO_H

#include <yoneda/core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

/// Default maximum character count for formatted messages.
#ifndef YO_MAX_LEN_FORMATTED_MSG
#    define YO_MAX_LEN_FORMATTED_MSG 8192
#endif

/// All available streams for logging.
enum yo_stream {
    YO_STREAM_ERROR,
    YO_STREAM_OUT,
    YO_STREAM_COUNT,
};

/// The default chosen stream to output logging messages.
#ifndef YO_LOG_DEFAULT_STREAM
#    define YO_LOG_DEFAULT_STREAM YO_STREAM_ERROR
#endif

/// Level of importance of a message.
enum yo_log_level {
    YO_LOG_LEVEL_FATAL,
    YO_LOG_LEVEL_ERROR,
    YO_LOG_LEVEL_WARNING,
    YO_LOG_LEVEL_INFO,
    YO_LOG_LEVEL_DEBUG,
    YO_LOG_LEVEL_COUNT,
};

/// Log a message to the standard error stream.
void yo_log(enum yo_stream stream, strptr file, u32 line_num, enum yo_log_level lvl, strptr msg);

/// Log a formatted message to the standard error stream.
yo_attr_fmt(5) void yo_log_fmt(
    enum yo_stream    stream,
    strptr            file,
    u32               line_num,
    enum yo_log_level lvl,
    strptr            fmt,
    ...);

// clang-format off
#if !defined(YO_DISABLE_LOGGING)
#    define yo_fatal(msg)            yo_log(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_FATAL, msg)
#    define yo_error(msg)            yo_log(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_ERROR, msg)
#    define yo_warning(msg)          yo_log(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_WARNING, msg)
#    define yo_info(msg)             yo_log(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_INFO, msg)
#    define yo_fatal_fmt(fmt, ...)   yo_log_fmt(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_FATAL, fmt, __VA_ARGS__)
#    define yo_error_fmt(fmt, ...)   yo_log_fmt(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#    define yo_warning_fmt(fmt, ...) yo_log_fmt(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_WARNING, fmt, __VA_ARGS__)
#    define yo_info_fmt(fmt, ...)    yo_log_fmt(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#    if defined(YO_DEBUG)
#        define yo_debug(msg)            yo_log(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_DEBUG, msg)
#        define yo_debug_fmt(fmt, ...)   yo_log_fmt(YO_LOG_DEFAULT_STREAM, yo_source_file(), yo_source_line(), YO_LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)
#    else
#        define yo_debug(msg)          0
#        define yo_debug_fmt(fmt, ...) 0
#    endif // YO_DEBUG
#else
#    define yo_fatal(msg)            0
#    define yo_error(msg)            0
#    define yo_warning(msg)          0
#    define yo_info(msg)             0
#    define yo_fatal_fmt(fmt, ...)   0
#    define yo_error_fmt(fmt, ...)   0
#    define yo_warning_fmt(fmt, ...) 0
#    define yo_info_fmt(fmt, ...)    0
#    define yo_debug(msg)            0
#    define yo_debug_fmt(fmt, ...)   0
#endif
// clang-format on

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_IO_H
