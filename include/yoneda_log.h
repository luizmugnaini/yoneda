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
/// Description: IO stream utilities.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_LOG_H
#define YONEDA_LOG_H

#include <yoneda_core.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

#ifndef YO_MAX_LOG_MESSAGE_LENGTH
#    define YO_MAX_LOG_MESSAGE_LENGTH 8192
#endif

#if defined(YO_ENABLE_LOGGING)
#    define yo_impl_make_log_info(log_level)            \
        yo_impl_LogInfo {                               \
            .file_name     = yo_source_file_name(),     \
            .function_name = yo_source_function_name(), \
            .line          = yo_source_line_number(),   \
            .level         = log_level,                 \
        }
#    define yo_log_fatal(msg)            yo_impl_log_msg(yo_impl_make_log_info(YO_LOG_LEVEL_FATAL), msg)
#    define yo_log_error(msg)            yo_impl_log_msg(yo_impl_make_log_info(YO_LOG_LEVEL_ERROR), msg)
#    define yo_log_warning(msg)          yo_impl_log_msg(yo_impl_make_log_info(YO_LOG_LEVEL_WARNING), msg)
#    define yo_log_info(msg)             yo_impl_log_msg(yo_impl_make_log_info(YO_LOG_LEVEL_INFO), msg)
#    define yo_log_fatal_fmt(fmt, ...)   yo_impl_log_fmt(yo_impl_make_log_info(YO_LOG_LEVEL_FATAL), fmt, __VA_ARGS__)
#    define yo_log_error_fmt(fmt, ...)   yo_impl_log_fmt(yo_impl_make_log_info(YO_LOG_LEVEL_ERROR), fmt, __VA_ARGS__)
#    define yo_log_warning_fmt(fmt, ...) yo_impl_log_fmt(yo_impl_make_log_info(YO_LOG_LEVEL_WARNING), fmt, __VA_ARGS__)
#    define yo_log_info_fmt(fmt, ...)    yo_impl_log_fmt(yo_impl_make_log_info(YO_LOG_LEVEL_INFO), fmt, __VA_ARGS__)
#    if defined(YO_DEBUG)
#        define yo_log_debug(msg)          yo_impl_log_msg(yo_impl_make_log_info(yo_LOG_LEVEL_DEBUG), msg)
#        define yo_log_debug_fmt(fmt, ...) yo_impl_log_fmt(yo_impl_make_log_info(yo_LOG_LEVEL_DEBUG), fmt, __VA_ARGS__)
#    else
#        define yo_log_debug(msg)          0
#        define yo_log_debug_fmt(fmt, ...) 0
#    endif  // YO_DEBUG
#else
#    define yo_log_fatal(msg)            0
#    define yo_log_error(msg)            0
#    define yo_log_warning(msg)          0
#    define yo_log_info(msg)             0
#    define yo_log_debug(msg)            0
#    define yo_log_fatal_fmt(fmt, ...)   0
#    define yo_log_error_fmt(fmt, ...)   0
#    define yo_log_warning_fmt(fmt, ...) 0
#    define yo_log_info_fmt(fmt, ...)    0
#    define yo_log_debug_fmt(fmt, ...)   0
#endif

// -----------------------------------------------------------------------------
// Implementation details.
// -----------------------------------------------------------------------------

enum yo_LogLevel {
    YO_LOG_LEVEL_FATAL = 0,
    YO_LOG_LEVEL_ERROR,
    YO_LOG_LEVEL_WARNING,
    YO_LOG_LEVEL_INFO,
    YO_LOG_LEVEL_DEBUG,
    YO_LOG_LEVEL_COUNT,
};

typedef struct yo_api yo_LogInfo {
    cstring           file_name;
    cstring           function_name;
    u32               line;
    enum psh_LogLevel level;
} psh_LogInfo;

/// Log a message to the standard error stream.
yo_api void yo_impl_log_msg(yo_LogInfo info, cstring msg);

/// Log a formatted message to the standard error stream.
yo_api yo_attr_fmt(2) void yo_impl_log_fmt(yo_LogInfo info, cstring fmt, ...);

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_LOG_H
