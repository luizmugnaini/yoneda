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
/// Description: OS stream management.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#ifndef YONEDA_STREAMS_H
#define YONEDA_STREAMS_H

#include <yoneda_core.h>
#include <yoneda_string.h>

#if defined(YO_LANG_CPP)
extern "C" {
#endif

#ifndef YO_DEFAUT_STDIN_READ_INITIAL_BUF_SIZE
#    define YO_DEFAUT_STDIN_READ_INITIAL_BUF_SIZE 128
#endif
#ifndef YO_DEFAULT_STDIN_READ_CHUNK_SIZE
#    define YO_DEFAULT_STDIN_READ_CHUNK_SIZE 64
#endif

typedef enum yo_FileFlag {
    /// Open a text file for reading operations.
    ///
    /// The file stream is positioned at the start of the file.
    YO_FILE_FLAG_READ_TEXT = 0,

    /// Open a text file for reading and writing.
    ///
    /// The file stream will be placed at the start of the file and the existing contents of the
    /// file, if any, will remain intact.
    YO_FILE_FLAG_READ_TEXT_EXTENDED,

    /// Open a non-text file for reading operations.
    ///
    /// The file stream is positioned at the start of the file.
    YO_FILE_FLAG_READ_BIN,

    /// Open a non-text file for reading and writing.
    ///
    /// The file stream will be placed at the start of the file and the existing contents of the
    /// file, if any, will remain intact.
    YO_FILE_FLAG_READ_BIN_EXTENDED,

    /// Open a file for writing.
    ///
    /// If the file exists, all file contents are overwritten and the stream is placed at
    /// the start of the file. Otherwise, the file will be created and the file stream will be
    /// placed at the start of the file.
    YO_FILE_FLAG_WRITE,

    /// Open a file for reading and writing.
    ///
    /// If the file exists, all existing file contents are overwritten and the file stream is
    /// positioned at the start of the file. Otherwise, the file will be created and the file
    /// stream will be placed at the start of the file.
    YO_FILE_FLAG_WRITE_EXTENDED,

    /// Open file for writing to the end of the file stream.
    ///
    /// The file stream is placed at the end of the file and all output is written to the
    /// current end of the file. If the file doesn't exist, it is created.
    YO_FILE_FLAG_APPEND,

    // @NOTE: the append extended flag won't be supported due to its non-portable behaviour. The
    //       user may use the WRITE_EXTENDED instead.

    YO_FILE_FLAG_COUNT,
} yo_FileFlag;

typedef enum yo_FileStatus {
    YO_FILE_STATUS_NONE = 0,
    YO_FILE_STATUS_FAILED_TO_OPEN,
    YO_FILE_STATUS_FAILED_TO_CLOSE,
    YO_FILE_STATUS_FAILED_TO_READ,
    YO_FILE_STATUS_OUT_OF_MEMORY,
    YO_FILE_STATUS_SIZE_UNKNOWN,
    YO_FILE_STATUS_COUNT,
} yo_FileStatus;

typedef struct yo_api yo_FileReadResult {
    u8*   buf;
    usize buf_size;
    yo_FileStatus;
} yo_FileReadResult;

/// Read file contents to a string.
///
/// Parameters:
///     * arena: The arena allocator that will carry the contents of the resulting string.
///     * path: A zero-terminated string containing the path to the file to be read.
///     * flag: Can be any flag with read permission.
yo_api yo_FileReadResult yo_read_file(yo_Arena* arena, cstring path, yo_FileFlag flag);

/// Read the standard input stream bytes to a string.
yo_api yo_DynString yo_read_stdin(yo_Arena* arena, u32 initial_buf_size, u32 read_chunk_size);

yo_api yo_DynString yo_absolute_path(yo_Arena* arena, cstring file_path);

#if defined(YO_LANG_CPP)
}
#endif

#endif  // YONEDA_STREAMS_H
