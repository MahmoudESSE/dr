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
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sysexits.h>

#include "gettext.h"
#include "str.h"

#define _(str) gettext (str)

#define MAX_STR_SIZE 1024

const char *argp_program_version = PACKAGE_STRING;

const char *argp_program_bug_address = "<" PACKAGE_BUGREPORT ">";

static char argp_doc[] = "dr -- list directory content in a tui.";

static char argp_args_doc[] = "[PATH...]";

/* List of Options */
static struct argp_option argp_options[] = {
  /* These options set a flag. */
  { "help", 'h', 0, 0, "show this help message", -1 },
  { "version", 'v', 0, 0, "show program version", -1 },
  { "usage", 'u', 0, 0, "show a short usage message", 0 },
  { 0, 0, 0, 0, "program settings:", 0 },
  { "verbose", 'V', 0, 0, "print more information", 0 },
  { "quiet", 'q', 0, 0, "print no information", 0 },
  { 0 },
};

struct arguments
{
  int verbose, quiet; /* '-v', '-q' */
  int no_args;
  char *name;
};

static error_t
argp_parser (int key, char *arg, struct argp_state *state)
{

  /* Get the 'input' argument from 'argp_parse', which we
   * know is a pointer to our arguments structure */
  struct arguments *arguments = state->input;

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
      fprintf (state->out_stream, "%s\n", argp_program_version);
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

static struct argp argp = {
  argp_options, argp_parser, argp_args_doc, argp_doc, 0, 0, 0,
};

static int
select_entries (const struct dirent *ep)
{
  int check_dot_current_directory = (0 != strcmp (ep->d_name, "."));
  int check_dot_dot_parent_directory = (0 != strcmp (ep->d_name, ".."));

  return check_dot_current_directory && check_dot_dot_parent_directory;
}

int
get_directory_entries (const char *const list_dir_name, int *num_entries)
{
  struct dirent **eps;
  *num_entries = scandir (list_dir_name, &eps, select_entries, alphasort);

  if (*num_entries < 0)
    {
      perror ("Couldn't open the directory");
      return 1;
    }

  int cen;
  for (cen = 0; cen < *num_entries; ++cen)
    {
      puts (eps[cen]->d_name);
    }

  return 0;
}

int
main (int argc, char **argv)
{
  struct arguments arguments;
  arguments.quiet = 0;
  arguments.verbose = 0;
  arguments.no_args = 0;

  STR_CSTR exec_path = argv[0];
  STR_CSTR exec_name = basename (exec_path);

  argp_parse (&argp, argc, argv, ARGP_NO_HELP, 0, &arguments);

  setlocale (LC_ALL, "");

  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  if (arguments.verbose)
    {
      printf ("verbose: true\n");
    }

  if (arguments.quiet)
    {
      printf ("verbose: true\n");
    }

  char *list_dir_name = NULL;
  int lenght_list_dir_name = MAX_STR_SIZE * sizeof (char);
  list_dir_name = malloc (lenght_list_dir_name + +1);
  if (list_dir_name == 0)
    {
      goto error;
    }

  memset (list_dir_name, 0, sizeof (char));
  if (arguments.no_args)
    {
      list_dir_name = "./";
    }
  else
    {
      list_dir_name = arguments.name;
    }

  int num_entries = 0;
  get_directory_entries (list_dir_name, &num_entries);

  if (num_entries < 0)
    {
      goto error;
    }

  printf ("listed: %d entries\n", num_entries);

error:
  if (errno != 0)
    {
      printf (_ ("%s: error %d: %s\n"), exec_name, errno, strerror (errno));
      exit (EXIT_FAILURE);
    }

  exit (EXIT_SUCCESS);
}
