# Yoneda Library

This is a general C library used across my projects. The code is written with the following
principles: simplicity, performance, and minimal memory usage. It's written in C11 and only
depends on a compatible compiler. Compiler that are ensured to work: Clang, GCC, MSVC.

# Integration

In order to integrate the Yoneda library to your project you may simply add the source file
`src/all.c` and the include directory `include` to your build command. That's it, nothing
fancy, no build system needed.

# Development

The library has a bundled compilation unit `src/all.c` which may be used if you wish to compile as
a unity build. This is as simple as, e.g.:
```sh
# Build static library.
clang -c -std=c11 -Iinclude src/all.c -o libyoneda.o && llvm-ar rc libyoneda.a libyoneda.o
# Build all library tests.
clang -std=c11 -Iinclude tests/test_all.c -o test
```

Another option is to use the `build.lua` script, which will manage to build the project with many
custom options that may be viewed in the file itself. With that said, Lua is, optionally, the only
dependency of the whole project - being only required if you want the convenience of running the build 
script.

# Library compile-time options

The following `#define` macros can be used to tweak the behaviour of the library as you want it:
- `yo_malloc` and `yo_free`: Function-like macros for overriding the default memory allocation/freeing behaviour.
- `YO_LOG_DEFAULT_STREAM`: The default stream used when logging messages to the system.
- `YO_DISABLE_LOGGING`: Silence all logging calls.
- `YO_DISABLE_ASSERTS`: Silence all assert calls.
- `YO_DEBUG`: Enables all debug checks.
