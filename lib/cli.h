/*
 * cli - library to use 'argp.h'
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
#ifndef DR_LIB_CLI_H_
#define DR_LIB_CLI_H_

#include <argp.h>
#include <config.h>
#include <stdlib.h>

/*
 * Name with the version of the program
 * to display in '--version' message.
 */
const char *cli_argp_program_version = PACKAGE_STRING;

/*
 * Bug report email address to send bugs or asks questions
 * typically shown at the bottom of '--help' message.
 */
const char *cli_argp_program_bug_address = "<" PACKAGE_BUGREPORT ">";

/*
 * Simple documentatin to show at the top of '--help' message.
 */
char cli_argp_doc[] = "dr -- list directory content in a tui.";

/*
 * Add custom usage messages.
 */
char cli_argp_args_doc[] = "[PATH...]";

/*
 * List of options that can be used.
 */
struct argp_option cli_argp_options[] = {
  /* These options set a flag. */
  { "help", 'h', 0, 0, "show this help message", -1 },
  { "version", 'v', 0, 0, "show program version", -1 },
  { "usage", 'u', 0, 0, "show a short usage message", 0 },
  { 0, 0, 0, 0, "program settings:", 0 },
  { "verbose", 'V', 0, 0, "print more information", 0 },
  { "quiet", 'q', 0, 0, "print no information", 0 },
  { 0 },
};

/*
 * Struct that holds flags that get set
 * and values from the command line.
 */
struct cli_arguments
{
  int verbose, quiet; /* '-v', '-q' */
  int no_args;
  char *name;
};

#endif // DR_LIB_CLI_H_
