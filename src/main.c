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

/*
 * Lenght limit for arrays.
 */
#define MAX_STR_SIZE 1024

/*
 * Name with the version of the program
 * to display in '--version' message.
 */
const char *argp_program_version = PACKAGE_STRING;

/*
 * Bug report email address to send bugs or asks questions
 * typically shown at the bottom of '--help' message.
 */
const char *argp_program_bug_address = "<" PACKAGE_BUGREPORT ">";

/*
 * Simple documentatin to show at the top of '--help' message.
 */
static char argp_doc[] = "dr -- list directory content in a tui.";

/*
 * Add custom usage messages.
 */
static char argp_args_doc[] = "[PATH...]";

/*
 * List of options that can be used.
 */
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

/*
 * Struct that holds flags that get set
 * and values from the command line.
 */
struct arguments
{
  int verbose, quiet; /* '-v', '-q' */
  int no_args;
  char *name;
};

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

/*
 * Construct the argp data structure wich we pass to the argp parse function.
 */
static struct argp argp = {
  argp_options, argp_parser, argp_args_doc, argp_doc, 0, 0, 0,
};

/*
 * We want the files and directories that the user can interact with,
 * yes '.' and '..' are normally used on the command line but in a
 * user interface such as the tui they would be only usefull for us
 * but not the user so we don't show them.
 * TODO: remove the filter and filter at the business logic level instead
 * of in the directory reader.
 */
static int
select_entries (const struct dirent *ep)
{
  int check_dot_current_directory = (0 != strcmp (ep->d_name, "."));
  int check_dot_dot_parent_directory = (0 != strcmp (ep->d_name, ".."));

  return check_dot_current_directory && check_dot_dot_parent_directory;
}

/*
 * Make the output follow a strict sorting of:
 * hidden directory > regular directory > hidden files > regular files.
 */
int
typesort (const struct dirent **a, const struct dirent **b)
{
  /*
   * Both are directories so we check wich one start
   * with a '.'.
   */
  if ((*a)->d_type == DT_DIR && (*b)->d_type == DT_DIR)
    {
      if ((*a)->d_name[0] == '.' && (*b)->d_name[0] != '.')
        {
          return -1;
        }

      if ((*a)->d_name[0] != '.' && (*b)->d_name[0] == '.')
        {
          return 1;
        }

      return strcoll ((*a)->d_name, (*b)->d_name);
    }

  /*
   * Both are files so we check wich one start
   * with a '.'.
   */
  if ((*a)->d_type != DT_DIR && (*b)->d_type != DT_DIR)
    {
      if ((*a)->d_name[0] == '.' && (*b)->d_name[0] != '.')
        {
          return -1;
        }

      if ((*a)->d_name[0] != '.' && (*b)->d_name[0] == '.')
        {
          return 1;
        }

      return strcoll ((*a)->d_name, (*b)->d_name);
    }

  /*
   * One of them is directory
   */
  if ((*a)->d_type == DT_DIR && (*b)->d_type != DT_DIR)
    {
      return -1;
    }

  return 1;
}

/*
 * Get the directory entries for the LIST_DIR_NAME.
 * pass NUM_ENTRIES to handle errors such as if the directory is empty.
 * TODO: pass an array to save into it the entries to handle those errors,
 * and use them later in the program.
 */
int
get_directory_entries (const char *const list_dir_name, int *num_entries)
{
  struct dirent **eps;
  *num_entries = scandir (list_dir_name, &eps, select_entries, typesort);

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
  /*
   * Initialize the arguments so we don't get
   * any weird values if their not set.
   */
  struct arguments arguments;
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
  name_dir_list = malloc (lenght_list_dir_name + +1);
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
  get_directory_entries (name_dir_list, &num_entries);

  if (num_entries < 0)
    {
      goto error;
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
