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
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "dir.h"
#include "str.h"

#define MAX_SIZE 2056

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

/*
 *  struct to hold info about each file and directory
 *  refer to struct dirent in <dirent.h>
 */
struct file_entry
{
  char *fe_name;
  unsigned char fe_name_length;
  unsigned char fe_type;
};

/*
  if (fe_raw_type == DT_DIR)
    {
      *fe_type = 'd';
      return;
    }

  if (fe_raw_type == DT_LNK)
    {
      *fe_type = 'l';
      return;
    }

  if (fe_raw_type == DT_SOCK)
    {
      *fe_type = 's';
      return;
    }

  *fe_type = '.';
*/
void
file_entry_determine_type (unsigned char *fe_raw_type, char *fe_type)
{
  switch (*fe_raw_type)
    {
    case DT_DIR:
      *fe_type = 'd';
      break;

    case DT_LNK:
      *fe_type = 'l';
      break;

    case DT_SOCK:
      *fe_type = 's';
      break;

    default:
      *fe_type = '.';
      break;
    }
}

void
tui_print_list (WINDOW *win, int *win_cur_pos,
                struct file_entry **file_entries_list, int num_entries)
{

  int cen = 0;
  char file_entry_type = '.';

  wmove (win, 0, 0);
  refresh ();

  for (cen = 0; cen < num_entries; ++cen)
    {
      file_entry_determine_type (&file_entries_list[cen]->fe_type,
                                 &file_entry_type);
      wmove (win, cen, 2);
      waddstr (win, file_entries_list[cen]->fe_name);
      wmove (win, cen, 0);
      refresh ();
    }
  *win_cur_pos = cen;
  refresh ();

  wmove (win, *win_cur_pos, 0);

  wprintw (win, _ ("listed: %d entries\n"), num_entries);
  refresh ();

  wmove (win, 0, 0);
  refresh ();
}

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

  struct file_entry **file_entries_list = NULL;
  int file_entries_list_length = MAX_SIZE * sizeof (struct file_entry);

  file_entries_list = malloc (file_entries_list_length + 1);

  if (file_entries_list == NULL)
    {
      goto error;
    }

  memset (file_entries_list, 0, sizeof (struct file_entry));

  int i = 0;
  int dir_name_length = 0;

  for (i = 0; i < num_entries; ++i)
    {
      dir_name_length = strlen (dir_list[i]->d_name);

      file_entries_list[i]->fe_name = NULL;
      file_entries_list[i]->fe_name
          = malloc (MAX_STR_SIZE * dir_name_length + 1);

      if (file_entries_list[i]->fe_name == NULL)
        {
          goto error;
        }

      memset (file_entries_list[i]->fe_name, 0, sizeof (char));
      strcpy (file_entries_list[i]->fe_name, dir_list[i]->d_name);

      file_entries_list[i]->fe_name_length = dir_name_length;
      file_entries_list[i]->fe_type = dir_list[i]->d_type;
    }

  int stdscr_max_y = 0;

  int input_key;

  int tui_stdscr_welcome_message_length = MAX_STR_SIZE * sizeof (char);

  char *tui_stdscr_welcome_message = NULL;

  tui_stdscr_welcome_message = malloc (tui_stdscr_welcome_message_length + 1);

  memset (tui_stdscr_welcome_message, 0, sizeof (char));

  strcpy (tui_stdscr_welcome_message,
          _ ("Hello to 'dr' your tui file manager."));

  int stdscr_cur_pos = 0;

  int length_entry = 0;

  use_env (TRUE);
  use_tioctl (TRUE);

  initscr ();

  cbreak ();
  noecho ();
  intrflush (stdscr, FALSE);
  keypad (stdscr, TRUE);

  stdscr_max_y = getmaxy (stdscr);

  wmove (stdscr, stdscr_max_y - 1, 0);
  refresh ();

  waddstr (stdscr, tui_stdscr_welcome_message);
  refresh ();

  tui_print_list (stdscr, &stdscr_cur_pos, file_entries_list, num_entries);
  refresh ();

  stdscr_cur_pos = 0;

  // TODO: add keybindings

  while (1)
    {
      length_entry = strlen (dir_list[stdscr_cur_pos]->d_name);

      mvchgat (0, 0, length_entry, A_BOLD | A_UNDERLINE, 1, NULL);
      refresh ();

      input_key = wgetch (stdscr);
      refresh ();

      switch (input_key)
        {
        case 'q':
        case 'Q':
          goto quit_tui;
        default:
          refresh ();
          break;
        }
    }

quit_tui:
  refresh ();
  endwin ();

  /*
   * Handle most error case here in defer way
   * to not repeat though the function.
   * if there is a better way maybe we can change this.
   */
error:
  if (errno != 0)
    {
      printf (_ ("%s: error %d: %s\n"), exec_name, errno, strerror (errno));

      refresh ();
      endwin ();

      exit (EXIT_FAILURE);
    }

  exit (EXIT_SUCCESS);
}
