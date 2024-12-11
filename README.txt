Yoneda Library
==============

This is a general C library used across my projects. The code is written with the following
principles in mind: simplicity, performance, and minimal memory usage. It's written in C11 and only
depends on a compatible compiler. Compilers that are ensured to work: Clang, GCC, MSVC.

Is this library for you?
------------------------

I really don't know. I actively write this library for the benefit of my own projects, not really
thinking of any user other than myself. I would actually recommend writing your own base library just
like I'm doing with Yoneda. This library is only open for the purpose of using in other public
projects and for maybe serving as an inspiration for a follow appreciator of the programming craft.

Integration
-----------

In order to integrate the Yoneda library to your project you may simply add the source file
src/yoneda_all.c and the include directory to your build command. That's it, nothing fancy, no build
system needed.

Development
-----------

Aside from the possibility of compiling on the command line using the bundled compilation unit
src/yoneda_all.c, I would recommend using the Lua script build.lua for development. It comes with
many options to tweak. For more info, run the script with the -help flag.

Library compile-time options
----------------------------

The following #define macros can be used to tweak the behaviour of the library as you want it:
- YO_ENABLE_LOGGING: Enable logging to the console.
- YO_ENABLE_ASSERTS: Issue assert calls for ensuring correctness of the program.
- YO_ENABLE_ASSERT_NOT_NULL: When necessary, always assert that a pointer is non-null before proceeding.
- YO_ENABLE_BOUNDS_CHECK: Check the index bounds for operations that may overflow a given buffer.
- YO_ENABLE_MEMCPY_OVERLAP_CHECK: Before calling memcpy, check if the memory ranges overlap.
- YO_ENABLE_ABORT_AT_MEMORY_ERROR: Abort the program whenever a memory allocation fails.
- YO_DEBUG: Enable all of the above debug checks.
- YO_ENABLE_ANSI_COLORS: Enable the use of ANSI Colors for printing to the console.
