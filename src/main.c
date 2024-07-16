/*
 * dr - list directory content using a tui
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

#include <argp.h>
#include <config.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "dir.h"
#include "str.h"

#define MAX_SIZE 1024

/*
 * Argp parser to go through the options,
 * it sets the flags in ARGUMENTS and if there is
 * arguments it save those values to their
 * appropriete variable.
 */
static error_t
argp_parser (int key, char *arg, struct argp_state *state)
{

  /* Get the 'input' argument from 'argp_parse', which we
   * know is a pointer to our arguments structure.
   */
  struct cli_arguments *arguments = state->input;

  switch (key)
    {
    case 'V':
      arguments->verbose = 1;
      break;
    case 'q':
      arguments->quiet = 1;
      break;
    case 'h':
      argp_state_help (state, state->out_stream, ARGP_HELP_STD_HELP);
      break;
    case 'u':
      argp_state_help (state, state->out_stream,
                       ARGP_HELP_USAGE | ARGP_HELP_EXIT_OK);
      break;
    case 'v':
      fprintf (state->out_stream, "%s\n", cli_argp_program_version);
      exit (EXIT_SUCCESS);
      break;
    case ARGP_KEY_NO_ARGS:
      arguments->no_args = 1;
      break;
    case ARGP_KEY_ARG:
      arguments->name = arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return EXIT_SUCCESS;
}

/*
 * Construct the argp data structure wich we pass to the argp parse function.
 */
struct argp argp = {
  cli_argp_options, argp_parser, cli_argp_args_doc, cli_argp_doc, 0, 0, 0,
};

int
main (int argc, char **argv)
{
  /*
   * Initialize the arguments so we don't get
   * any weird values if their not set.
   */
  struct cli_arguments arguments;
  arguments.quiet = 0;
  arguments.verbose = 0;
  arguments.no_args = 0;

  /*
   * Get the path where the program was executed.
   */
  STR_CSTR exec_path = argv[0];
  /*
   * And extract the name of the executable.
   */
  STR_CSTR exec_name = basename (exec_path);

  argp_parse (&argp, argc, argv, ARGP_NO_HELP, 0, &arguments);

  /*
   * Set the local based on the system where the program is run.
   */
  setlocale (LC_ALL, "");

  /*
   * Set the directory where the translation text lives.
   */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /*
   * TODO: implement a logging architecture
   * to save logs either in the syslog or in file.log.
   */
  if (arguments.verbose)
    {
      printf ("verbose: true\n");
    }

  /*
   * Just don't.
   */
  if (arguments.quiet)
    {
      printf ("verbose: true\n");
    }

  /*
   * TODO: add ncurses here.
   */

  /*
   * Get the entries and save them here for later use.
   */
  char *name_dir_list = NULL;
  int lenght_list_dir_name = MAX_STR_SIZE * sizeof (char);
  /*
   * Allocate enough size for directory path name.
   */
  name_dir_list = malloc (lenght_list_dir_name + 1);
  if (name_dir_list == NULL)
    {
      goto error;
    }

  memset (name_dir_list, 0, sizeof (char));

  /*
   * If no arguments were passed we set list the current directory.
   */
  if (arguments.no_args)
    {
      name_dir_list = "./";
    }
  else
    {
      name_dir_list = arguments.name;
    }

  int num_entries = 0;

  struct dirent **dir_list = NULL;
  int lenght_dir_list = MAX_SIZE * sizeof (struct dirent);
  dir_list = malloc (lenght_dir_list + 1);
  if (dir_list == NULL)
    {
      goto error;
    }

  memset (dir_list, 0, sizeof (struct dirent));

  if (dir_get_directory_entries (name_dir_list, &dir_list, &num_entries) != 0)
    {
      goto error;
    }

  int cen;
  for (cen = 0; cen < num_entries; ++cen)
    {
      puts (dir_list[cen]->d_name);
    }

  /*
   * TODO: use ncurses to display this list.
   */
  printf ("listed: %d entries\n", num_entries);

  /*
   * Handle most error case here in defer way
   * to not repeat though the function.
   * if there is a better way maybe we can change this.
   */
error:
  if (errno != 0)
    {
      printf (_ ("%s: error %d: %s\n"), exec_name, errno, strerror (errno));
      exit (EXIT_FAILURE);
    }

  exit (EXIT_SUCCESS);
}
