--                          Yoneda Library
--    Copyright (C) 2024 Luiz Gustavo Mugnaini Anselmo
--
--    This program is free software; you can redistribute it and/or modify
--    it under the terms of the GNU General Public License as published by
--    the Free Software Foundation; either version 2 of the License, or
--    (at your option) any later version.
--
--    This program is distributed in the hope that it will be useful,
--    but WITHOUT ANY WARRANTY; without even the implied warranty of
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--    GNU General Public License for more details.
--
--    You should have received a copy of the GNU General Public License along
--    with this program; if not, write to the Free Software Foundation, Inc.,
--    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
--
-- Description: Build system for the Yoneda library.
-- Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
--
-- Running the build system:  lua build.lua [options]
-- Example:                   lua build.lua fmt clang mold test

local start_time = os.time()

-- -----------------------------------------------------------------------------
-- Available command line options
-- -----------------------------------------------------------------------------

-- Available command line options.
local options = {
    -- Format source files.
    fmt = false,
    -- Build type (default: debug build).
    release = false,
    -- Compiler of choice (default: MSVC on Windows and GCC on Linux).
    clang = false,
    gcc = false,
    msvc = false,
    -- Build and run tests.
    test = false,
    -- Whether or not to print the commands ran by the build script and their output.
    quiet = false,
}

for i = 1, #arg do
    options[arg[i]] = true
end

local os_info = {
    windows = (package.config:sub(1, 1) == "\\"),
    linux = false,
    darwin = false,
}

if not os_info.windows then
    local handle = io.popen("uname")
    local result = handle:read("*a")
    handle:close()

    if string.match(result, "Linux") then
        os_info.linux = true
    elseif string.match(result, "Darwin") then
        os_info.darwin = true
    end
end

if os_info.windows then
    os_info.path_sep = "\\"
    os_info.silence_cmd = " > NUL 2>&1"
    os_info.obj_ext = ".obj"
    os_info.lib_ext = ".lib"
    os_info.exe_ext = ".exe"
else
    os_info.path_sep = "/"
    os_info.silence_cmd = " > /dev/null 2>&1"
    os_info.obj_ext = ".o"
    os_info.lib_ext = ".a"
    os_info.exe_ext = ""
end

function exec(cmd_str, quiet)
    if quiet or options.quiet then
        os.execute(cmd_str .. os_info.silence_cmd)
    else
        print("\x1b[1;35mexecuting ::\x1b[0m " .. cmd_str)
        os.execute(cmd_str)
    end
end

function concat(arr, join, is_prefix)
    if #arr < 1 then
        return ""
    end

    local acc = arr[1]
    for i = 2, #arr do
        acc = acc .. join .. arr[i]
    end

    if is_prefix then
        acc = join .. acc
    end

    return acc
end

local function make_path(arr)
    return concat(arr, os_info.path_sep, false)
end

local function get_script_dir()
    local info = debug.getinfo(1, "S").source:sub(2)
    return info:match("(.*)[/\\]")
end

-- -----------------------------------------------------------------------------
-- Project configuration
-- -----------------------------------------------------------------------------

local root_dir = get_script_dir()
if not root_dir then
    root_dir = "."
end

local compilers = {
    clang = {
        cc = "clang",
        opt_include = "-I",
        opt_define = "-D",
        opt_std = "-std=",
        opt_no_link = "-c",
        opt_out_obj = "-o",
        opt_out_exe = "-o",
        flags_common = "-pedantic -Wall -Wextra -Wconversion -Wpedantic -Wuninitialized -Wcast-align -Wbad-function-cast -Wfloat-conversion -Wshadow -Wnull-pointer-arithmetic -Wnull-dereference -Wformat=2 -Wvla "
            .. "-Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion "
            .. "-Wno-unused-variable -Wno-switch-enum -Wno-unsafe-buffer-usage -Wno-declaration-after-statement",
        flags_debug = "-Werror -g -O0 -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fstack-protector-strong -fsanitize=leak",
        flags_release = "-O2",
        ar = "llvm-ar",
        ar_out = "",
        ar_flags = "rcs",
    },
    gcc = {
        cc = "gcc",
        opt_include = "-I",
        opt_define = "-D",
        opt_std = "-std=",
        opt_no_link = "-c",
        opt_out_obj = "-o",
        opt_out_exe = "-o",
        flags_common = "-pedantic -Wall -Wextra -Wconversion -Wpedantic -Wuninitialized -Wcast-align -Wfloat-conversion -Wshadow -Wnull-pointer-arithmetic -Wnull-dereference -Wformat=2  -Wvla"
            .. "-Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion ",
        flags_debug = "-Werror -g -O0 -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fstack-protector-strong -fsanitize=leak",
        flags_release = "-O2",
        ar = "ar",
        ar_out = "",
        ar_flags = "rcs",
    },
    msvc = {
        cc = "cl",
        opt_include = "-I",
        opt_define = "-D",
        opt_std = "/std:",
        opt_no_link = "-c",
        opt_out_obj = "/Fo:",
        opt_out_exe = "/Fe:",
        flags_common = "-nologo -Oi -TC -MP -FC -GF -GA /fp:except- -GR- -EHsc- /INCREMENTAL:NO /W3",
        flags_debug = "/Ob0 /Od /Oy- /Z7 /RTC1 /MTd",
        flags_release = "/O2 /MT",
        ar = "lib",
        ar_out = "/out:",
        ar_flags = "/nologo",
    },
    clang_cl = {
        cc = "clang-cl",
        opt_include = "-I",
        opt_define = "-D",
        opt_std = "/std:",
        opt_no_link = "-c",
        opt_out_obj = "-o",
        opt_out_exe = "-o",
        flags_common = "/TC -Wall -Wextra -Wconversion -Wpedantic -Wuninitialized -Wcast-align -Wbad-function-cast -Wfloat-conversion -Wshadow -Wnull-pointer-arithmetic -Wnull-dereference -Wformat=2 "
            .. "-Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion "
            .. "-Wno-unused-variable -Wno-switch-enum -Wno-unsafe-buffer-usage -Wno-declaration-after-statement",
        flags_debug = "-Ob0 /Od /Oy- /Z7 /RTC1 -g /MTd",
        flags_release = "-O2 /MT",
        ar = "llvm-lib",
        ar_out = "/out:",
        ar_flags = "/nologo",
    },
}

local yoneda = {
    src = make_path({ root_dir, "src", "all.c" }),
    test_src = make_path({ root_dir, "tests", "test_all.c" }),
    include_dir = make_path({ root_dir, "include" }),
    defines = {},
    debug_defines = { "YO_DEBUG" },
    lib = os_info.windows and "yoneda" or "libyoneda",
    test_exe = "test_all",
    std = "c11",
}

-- -----------------------------------------------------------------------------
-- Toolchain
-- -----------------------------------------------------------------------------

local tc = os_info.windows and compilers.msvc or compilers.gcc
if options.clang then
    tc = os_info.windows and compilers.clang_cl or compilers.clang
elseif options.gcc then
    assert(not os_info.windows, "GCC build not supported on Windows")
    tc = compilers.gcc
elseif options.msvc then
    tc = compilers.msvc
end

-- -----------------------------------------------------------------------------
-- Execute build instructions
-- -----------------------------------------------------------------------------

if options.fmt then
    exec(
        string.format(
            "clang-format -i %s %s",
            make_path({ root_dir, yoneda.include_dir, "*.h" }),
            make_path({ root_dir, "src", "*.c" })
        )
    )
end

local out_dir = make_path({ root_dir, "build" })
local obj_out = make_path({ out_dir, yoneda.lib .. os_info.obj_ext })
local lib_out = make_path({ out_dir, yoneda.lib .. os_info.lib_ext })
exec("mkdir " .. out_dir, true)

local function build_yoneda_lib()
    -- Compile without linking.
    exec(
        string.format(
            string.rep("%s ", 9),
            tc.cc,
            tc.opt_no_link,
            tc.opt_std .. yoneda.std,
            tc.flags_common,
            options.release and tc.flags_release or tc.flags_debug,
            concat(yoneda.defines, " " .. tc.opt_define, true),
            tc.opt_include .. yoneda.include_dir,
            tc.opt_out_obj .. obj_out,
            yoneda.src
        )
    )
    -- Archive objs into a library.
    exec(string.format("%s %s %s %s", tc.ar, tc.ar_flags, tc.ar_out .. lib_out, obj_out))
end

local function build_yoneda_tests()
    local test_exe_out = make_path({ out_dir, yoneda.test_exe .. os_info.exe_ext })
    local test_obj_out = make_path({ out_dir, yoneda.test_exe .. os_info.obj_ext })
    exec(
        string.format(
            string.rep("%s ", 10),
            tc.cc,
            tc.opt_std .. yoneda.std,
            tc.flags_common,
            tc.flags_debug,
            concat(yoneda.defines, " " .. tc.opt_define, true),
            concat(yoneda.debug_defines, " " .. tc.opt_define, true),
            tc.opt_include .. yoneda.include_dir,
            tc.opt_out_obj .. test_obj_out,
            tc.opt_out_exe .. test_exe_out,
            yoneda.test_src
        )
    )
    return test_exe_out
end

build_yoneda_lib()

if options.test then
    local test_exe = build_yoneda_tests()
    exec(test_exe)
end

local end_time = os.time()
print(string.format("\x1b[1;35mtime elapsed ::\x1b[0m %.5f seconds", os.difftime(end_time, start_time)))
