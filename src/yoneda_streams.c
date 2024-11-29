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
/// Description: Implementation of the file system management utilities.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <yoneda_streams.h>

#include <stdio.h>
#include <yoneda_core.h>

#if defined(YO_OS_WINDOWS)
#    include <Windows.h>
#    define YO_IMPL_PATH_MAX_CHAR_COUNT                     MAX_PATH
#    define yo_impl_file_open(file_handle, file_name, mode) fopen_s(&file_handle, file_name, mode)
#else
#    include <limits.h>
#    include <unistd.h>
#    define YO_IMPL_PATH_MAX_CHAR_COUNT PATH_MAX
#    define yo_impl_file_open(file_handle, file_name, mode) \
        do {                                                \
            file_handle = fopen(file_name, mode);           \
        } while (0)
#endif

// @TODO(luiz): Substitute the `perror` calls with a thread safe alternative to `strerror`.

yo_internal cstring const YO_IMPL_FILE_FLAG_TO_CSTR[YO_FILE_FLAG_COUNT] = {
    "r",
    "r+",
    "rb",
    "rb+",
    "w",
    "w+",
    "a",
};

yo_inline bool yo_has_read_permission(yo_FileFlag flag) {
    return (flag == YO_FILE_FLAG_READ_TEXT) ||
           (flag == YO_FILE_FLAG_READ_TEXT_EXTENDED) ||
           (flag == YO_FILE_FLAG_READ_BIN) ||
           (flag == YO_FILE_FLAG_READ_BIN_EXTENDED) ||
           (flag == YO_FILE_FLAG_WRITE_EXTENDED);
}

yo_FileReadResult yo_read_file(yo_Arena* arena, cstring file_name, yo_FileFlag flag) {
    yo_FileStatus status = YO_FILE_STATUS_NONE;

    // Open file.
    FILE* file_handle;
    {
        yo_impl_file_open(file_handle, file_name, YO_IMPL_FILE_FLAG_TO_CSTR[flag]);

        if (yo_unlikely(file_handle == NULL)) {
            status = YO_FILE_STATUS_FAILED_TO_OPEN;
        }
    }

    // Get the file size in bytes.
    usize buf_size = 0;
    if (yo_likely(status == YO_FILE_STATUS_NONE)) {
        if (yo_unlikely(fseek(file_handle, 0, SEEK_END) == -1)) {
            perror("Couldn't seek end of file.\n");
            status |= YO_FILE_STATUS_FAILED_TO_READ;
        }

        isize file_size = ftell(file_handle);
        if (yo_unlikely(file_size == -1)) {
            perror("Couldn't tell the size of the file.\n");
            status |= YO_FILE_STATUS_SIZE_UNKNOWN;
        }
        buf_size = yo_cast(usize, file_size);

        if (yo_unlikely(fseek(file_handle, 0, SEEK_SET) == -1)) {
            perror("Couldn't seek start of file.\n");
            status |= YO_FILE_STATUS_FAILED_TO_READ;
        }
    }

    yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(arena);

    // Allocate target buffer.
    u8* buf = NULL;
    if (yo_likely(status == YO_FILE_STATUS_NONE)) {
        yo_assert_msg(arena != NULL, "Invalid arena.");

        buf = yo_arena_alloc(arena, u8, buf_size);
        if (yo_unlikely(buf == NULL)) {
            status |= YO_FILE_STATUS_OUT_OF_MEMORY;
        }
    }

    // Read file content to the target buffer.
    if (yo_likely(status == YO_FILE_STATUS_NONE)) {
        usize read_count = fread(buf, yo_sizeof(u8), buf_size, file_handle);

        if (yo_unlikely(read_count != buf_size)) {
            yo_arena_checkpoint_restore(arena_checkpoint);

            buf      = NULL;
            buf_size = 0;
            status |= YO_FILE_STATUS_FAILED_TO_READ;
        }
    }

    if (yo_likely(!(status & YO_FILE_STATUS_FAILED_TO_OPEN))) {
        if (yo_unlikely(fclose(file_handle) == EOF)) {
            status |= YO_FILE_STATUS_FAILED_TO_CLOSE;
        }
    }

    return (yo_FileReadResult){
        .buf      = buf,
        .buf_size = buf_size,
        .status   = status,
    };
}

yo_DynString yo_read_stdin(yo_Arena* arena, u32 initial_buf_size, u32 read_chunk_size) {
    yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(arena);

    yo_DynString content = yo_make_dynstring(arena, initial_buf_size);

#if defined(YO_OS_WINDOWS)
    HANDLE handle_stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (handle_stdin == INVALID_HANDLE_VALUE) {
        yo_log_error("Unable to acquire the handle to the stdin stream.");

        yo_arena_checkpoint_restore(arena_checkpoint);
        return yo_make_default(yo_DynString);
    }

    for (;;) {
        if (content.length + read_chunk_size > content.capacity) {
            yo_dynstring_resize(&content, content.length + read_chunk_size);
        }

        DWORD bytes_read;
        BOOL  success = ReadFile(handle_stdin, &content.buf[content.length], read_chunk_size, &bytes_read, NULL);
        content.length += bytes_read;
        if (yo_unlikely(!success)) {
            yo_log_error("Unable to read from the stdin stream.");

            yo_arena_checkpoint_restore(arena_checkpoint);
            return yo_make_default(yo_DynString);
        }

        if (bytes_read < read_chunk_size) {
            break;
        }
    }
#else
    for (;;) {
        if (content.length + read_chunk_size > content.capacity) {
            yo_dynstring_resize(&content, content.length + read_chunk_size);
        }

        isize bytes_read = read(STDIN_FILENO, &content.buf[content.length], read_chunk_size);

        if (yo_unlikely(bytes_read == -1)) {
            yo_log_error("Unable to read from the stdin stream.");

            yo_arena_checkpoint_restore(arena_checkpoint);
            return yo_make_default(yo_DynString);
        }

        content.length += yo_cast(usize, bytes_read);
        if (bytes_read < read_chunk_size) {
            break;
        }
    }
#endif

    // Add null terminator to the end of the string.
    if (content.length == content.capacity) {
        yo_dynstring_resize(&content, content.length + 1);
    }
    content.buf[content.length] = 0;

    return content;
}

yo_DynString yo_absolute_path(yo_Arena* arena, cstring file_path) {
    yo_assert(arena != NULL);
    yo_ArenaCheckpoint arena_checkpoint = yo_make_arena_checkpoint(arena);

    yo_DynString abs_path = yo_make_dynstring(arena, YO_IMPL_PATH_MAX_CHAR_COUNT);

#if defined(YO_OS_WINDOWS)
    DWORD result = GetFullPathName(file_path, YO_IMPL_PATH_MAX_CHAR_COUNT, abs_path.buf, NULL);
    if (yo_unlikely(result == 0)) {
        yo_log_error_fmt(
            "Unable to obtain the full path of %s due to the error: %lu",
            file_path,
            GetLastError());

        yo_arena_checkpoint_restore(arena_checkpoint);
        return yo_make_default(yo_DynString);
    }
#else
    char* result = realpath(file_path, abs_path.buf);
    if (yo_unlikely(result == NULL)) {
        yo_log_error_fmt("Unable to obtain the full path of %s due to the error:", file_path);
        perror(NULL);

        yo_arena_checkpoint_restore(arena_checkpoint);
        return yo_make_default(yo_DynString);
    }
#endif

    return abs_path;
}
