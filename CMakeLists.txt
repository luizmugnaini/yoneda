# ------------------------------------------------------------------------------
# Project description
# 
# Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

project(
    yoneda 
    VERSION 0.0.1
    LANGUAGES C
)

# ------------------------------------------------------------------------------
# Compiler info
# ------------------------------------------------------------------------------

message(STATUS "C Compiler executable: ${CMAKE_C_COMPILER}")
message(STATUS "Linker executable: ${CMAKE_LINKER}")

# ------------------------------------------------------------------------------
# Tooling integration
# ------------------------------------------------------------------------------

# Enable Clangd LSP integration with the project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Resulting compiler and linker flags for the build
# ------------------------------------------------------------------------------

set(YO_C_FLAGS)
set(YO_C_SAN_FLAGS)

if(UNIX)
    set(YO_DEFINE_PREFIX "-D")
elseif(MSVC)
    set(YO_DEFINE_PREFIX "/D")
    set(YO_C_FLAGS ${YO_C_FLAGS} "${YO_DEFINE_PREFIX}_CRT_SECURE_NO_WARNINGS")
endif()

# ------------------------------------------------------------------------------
# Compiler flags
#
# Here we'll have shared flags in the form `__YO_COMMON_CXX_FLAGS_[COMPILERS]`
# and the variables with `YO_[COMPILER]_CXX_FLAGS` will be the final result of
# the list of flags for the corresponding compiler.
# ------------------------------------------------------------------------------

# ---- Unix GCC and Clang ----

set(
    __YO_COMMON_C_FLAGS_GCC_CLANG
    "-std=c11"
    "-Wvla"
    "-Wall"
    "-Wextra"
    "-pedantic"
    "-Wuninitialized"
    "-Wswitch"
    "-Wcovered-switch-default"
    "-Wshadow"
    "-Wcast-align"
    "-Wunused"
    "-Wpedantic"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wnull-dereference"
    "-Wdouble-promotion"
    "-Wmisleading-indentation"
    "-Wunused-parameter"
    "-Wstrict-prototypes"
    "-Wformat=2"
    "-fcolor-diagnostics"
)

# Final compiler flags
set(YO_GCC_C_FLAGS     ${__YO_COMMON_C_FLAGS_GCC_CLANG})
set(YO_CLANG_C_FLAGS   ${__YO_COMMON_C_FLAGS_GCC_CLANG})

# ---- MSVC and Clang-cl ----

set(
    __YO_COMMON_C_FLAGS_MSVC_CLANGCL
    "/std:c11"
    "/nologo"
    "/Oi"
    "/fp:except-"   # Disable exceptions.
    "/GR-"          # Disable emission of RTTI data.
    "/GA"           # Optimize for Windows applications.
)
set(
    YO_COMMON_C_DEBUG_FLAGS_MSVC_CLANGCL
    "/Zi"   # Debug info.
    "/Ob0"  # Disable inlining.
    "/Oy-"  # Disable frame pointer optimization.
)
set(
    YO_COMMON_C_RELEASE_FLAGS_MSVC_CLANGCL
    "/Ob2"  # Inline functions as deemed beneficial by the compiler.
    "/Oy"
)

# Final compiler flags
set(YO_MSVC_C_FLAGS      ${__YO_COMMON_CXX_FLAGS_MSVC_CLANGCL})
set(YO_CLANGCL_C_FLAGS ${__YO_COMMON_CXX_FLAGS_MSVC_CLANGCL} ${__YO_COMMON_C_FLAGS_GCC_CLANG})

# ------------------------------------------------------------------------------
# Sanitizer flags
#
# Here we'll have shared flags in the form `__YO_COMMON_SAN_CXX_FLAGS_[COMPILERS]`
# and the variables with `YO_[COMPILER]_CXX_SAN_FLAGS` will be the final result
# of the variables.
# ------------------------------------------------------------------------------

# ---- GCC and Clang ----

set(
    __YO_COMMON_C_SAN_FLAGS_GCC_CLANG
    "-fsanitize=address"
    "-fsanitize=pointer-compare"
    "-fsanitize=pointer-subtract"
    "-fsanitize=shadow-call-stack"
    "-fsanitize=undefined"
    "-fstack-protector-strong"
    "-fsanitize=leak"
)
set(YO_GCC_C_SAN_FLAGS   ${__YO_COMMON_C_SAN_FLAGS_GCC_CLANG})
set(YO_CLANG_C_SAN_FLAGS ${__YO_COMMON_C_SAN_FLAGS_GCC_CLANG})

# ---- MSVC and Clang-cl ----

set(
    __YO_COMMON_C_SAN_FLAGS_MSVC_CLANGCL 
    # TODO: add "/fsanitize=address" flag
)
set(YO_MSVC_C_SAN_FLAGS    ${__YO_COMMON_C_SAN_FLAGS_MSVC_CLANGCL})
set(YO_CLANGCL_C_SAN_FLAGS ${__YO_COMMON_C_SAN_FLAGS_MSVC_CLANGCL})

# ------------------------------------------------------------------------------
# Compiler flags setup per compiler
# ------------------------------------------------------------------------------

# ---- C ----

if(CMAKE_C_COMPILER_ID STREQUAL "GCC")
    set(YO_C_FLAGS ${YO_C_FLAGS} ${YO_GCC_C_FLAGS})
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # Clang
    if(UNIX)
        set(YO_C_FLAGS ${YO_C_FLAGS} ${YO_CLANG_C_FLAGS})
    endif(UNIX)

    # Clang-cl
    if(MSVC)
        set(YO_C_FLAGS ${YO_C_FLAGS} ${YO_CLANGCL_C_FLAGS})
    endif(MSVC)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(YO_C_FLAGS ${YO_C_FLAGS} ${YO_MSVC_C_FLAGS})
endif()

# Set debug/release flags for both MSVC and Clang-cl
if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(YO_C_FLAGS     ${YO_C_FLAGS}     ${YO_COMMON_C_DEBUG_FLAGS_MSVC_CLANGCL})
        set(YO_C_SAN_FLAGS ${YO_C_SAN_FLAGS} ${YO_CLANGCL_C_SAN_FLAGS})
    else()
        set(YO_C_FLAGS ${YO_C_FLAGS} ${YO_COMMON_C_RELEASE_FLAGS_MSVC_CLANG_CL})
    endif()
endif()

# ------------------------------------------------------------------------------
# Inform the resulting compiler and sanitizer flags
# ------------------------------------------------------------------------------

message(STATUS "Yoneda C compiler flags:    ${YO_C_FLAGS}")
message(STATUS "Yoneda C sanitizer flags:   ${YO_C_SAN_FLAGS}")

# ------------------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------------------

set(
    YOC_SOURCES
    "${PROJECT_SOURCE_DIR}/src/yo_arena.c"
    "${PROJECT_SOURCE_DIR}/src/yo_io.c"
    "${PROJECT_SOURCE_DIR}/src/yo_assert.c"
)

# ------------------------------------------------------------------------------
# Where the products are going
# ------------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# ------------------------------------------------------------------------------
# Library
# ------------------------------------------------------------------------------

add_library(yoneda ${YOC_SOURCES})
target_compile_options(yoneda PRIVATE ${YO_C_FLAGS} ${YO_C_SAN_FLAGS})
target_link_libraries(yoneda PUBLIC ${YO_C_SAN_FLAGS})
target_include_directories(yoneda PUBLIC "${PROJECT_SOURCE_DIR}/src")

# ------------------------------------------------------------------------------
# Library tests
# ------------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enabling tests")
    enable_testing()
endif()

list(APPEND TESTS)

foreach(t IN LISTS TESTS)
    add_executable(${t} "${PROJECT_SOURCE_DIR}/tests/${t}.cc")
    target_compile_options(${t} PRIVATE ${YO_CXX_FLAGS})
    target_link_libraries(${t} PUBLIC yoneda)
    add_test(NAME ${t} COMMAND ${t})
endforeach()
