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
/// Description: Implementation of the string type procedures.
/// Author: Luiz G. Mugnaini A. <luizmuganini@gmail.com>

#include <yoneda/str.h>

#include <string.h>
#include <yoneda/memutils.h>

struct yo_str yo_str_from(strptr cstr) {
    return (struct yo_str){.size = yo_strlen(cstr), .buf = cstr};
}

bool yo_str_eq(struct yo_str lhs, struct yo_str rhs) {
    return (lhs.size == rhs.size) && yo_memeq(lhs.buf, rhs.buf, lhs.size);
}

bool yo_str_eq_cstr(struct yo_str str, strptr cstr) {
    return (str.size == yo_strlen(cstr)) && yo_memeq(str.buf, cstr, str.size);
}
