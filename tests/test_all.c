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
/// Description: Single compilation unit for the whole suite of tests for the Yoneda library.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

// Source for the whole Yoneda library.
#include "../src/all.c"

// Prevent tests from defining a `main` function.
#define YO_TEST_NO_MAIN

// -----------------------------------------------------------------------------
// - Invoke all library tests -
// -----------------------------------------------------------------------------

#include "test_memory.c"

int main(void) {
    test_memory();
    return 0;
}
