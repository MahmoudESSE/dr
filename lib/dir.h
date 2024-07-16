/*
 * dir - library to use manage directory entries
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

#ifndef DR_LIB_DIR_H_
#define DR_LIB_DIR_H_

#include <dirent.h>
#include <libgen.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sysexits.h>

/*
 * We want the files and directories that the user can interact with,
 * yes '.' and '..' are normally used on the command line but in a
 * user interface such as the tui they would be only usefull for us
 * but not the user so we don't show them.
 * TODO: remove the filter and filter at the business logic level instead
 * of in the directory reader.
 */
int dir_select_entries (const struct dirent *ep);

/*
 * Make the output follow a strict sorting of:
 * hidden directory > regular directory > hidden files > regular files.
 */
int dir_typesort (const struct dirent **a, const struct dirent **b);

/*
 * Get the directory entries for the LIST_DIR_NAME.
 * pass NUM_ENTRIES to handle errors such as if the directory is empty.
 * TODO: pass an array to save into it the entries to handle those errors,
 * and use them later in the program.
 */
int dir_get_directory_entries (const char *const list_dir_name,
                               struct dirent ***dir_list, int *num_entries);

#endif // DR_LIB_DIR_H_
