/*
 * str - library to work with c style strings
 *
 * Copyright (C) 2024  MahmoudESSE

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef DR_LIB_STR_H_
#define DR_LIB_STR_H_

#include "gettext.h"

/*
 * c style string type definition short hand
 */
#define STR_CSTR char *

#define _(str) gettext (str)

/*
 * Lenght limit for arrays.
 */
#define MAX_STR_SIZE 1024

#endif // DR_LIB_STR_H_
