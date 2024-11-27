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
/// Description: Implementation of the IO stream utilities.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <yoneda_log.h>

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <yoneda_core.h>

// @TODO: enable the user to set a different output stream for the logs.

#define YO_IMPL_LOG_FMT = "%s [%s:%u:%s] %s\n"

yo_internal cstring const YO_IMPL_LOG_LEVEL_STR[LOG_LEVEL_COUNT] = {
#if !defined(YO_DISABLE_ANSI_COLORS)
    "\x1b[1;41m[FATAL]\x1b[0m",
    "\x1b[1;31m[ERROR]\x1b[0m",
    "\x1b[1;33m[WARNING]\x1b[0m",
    "\x1b[1;32m[INFO]\x1b[0m",
    "\x1b[1;34m[DEBUG]\x1b[0m",
#else
    "[FATAL]",
    "[ERROR]",
    "[WARNING]",
    "[INFO]",
    "[DEBUG]",
#endif
};

void yo_impl_log_msg(LogInfo info, cstring msg) {
    yo_discard_value(fprintf(
        stderr,
        YO_IMPL_LOG_FMT,
        YO_IMPL_LOG_LEVEL_STR[info.level],
        info.file_name,
        info.line,
        info.function_name,
        msg));
}

void yo_impl_log_fmt(LogInfo info, cstring fmt, ...) {
    char msg[YO_MAX_LOG_MESSAGE_LENGTH];

    va_list args;
    va_start(args, fmt);
    {
        // Format the message with the given arguments.
        i32 res_len = vsnprintf(msg, YO_MAX_LOG_MESSAGE_LENGTH, fmt, args);
        assert((res_len != -1) && "snptrintf unable to parse the format string and arguments");

        // Stamp the message with a null-terminator.
        usize ures_len = yo_cast(usize, res_len);
        usize msg_len  = yo_min_value(ures_len, MAX_MSG_LEN);
        msg[msg_len]   = 0;
    }
    va_end(args);

    yo_discard_value(fprintf(
        stderr,
        YO_IMPL_LOG_FMT,
        YO_IMPL_LOG_LEVEL_STR[info.level],
        info.file_name,
        info.line,
        info.function_name,
        msg));
}
