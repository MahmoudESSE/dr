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

#include <config.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gettext.h"
#include "str.h"

#define _(str) gettext (str)

/* Flag set by '--verbose'. */
static int verbose_flag;

/* List of Options */
static struct option options[] = {
  /* These options set a flag. */
  { "verbose", no_argument, &verbose_flag, 1 },
  { "quiet", no_argument, &verbose_flag, 0 },

  /*
   * These options don't set a flag.
   * We distinguish them by their indices.
   */
  { "list", no_argument, 0, 'l' },
  { 0 },
};

int
main (int argc, char **argv)
{
  STR_CSTR exec_path = argv[0];
  STR_CSTR exec_name = basename (exec_path);

  setlocale (LC_ALL, NULL);

  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  int c;

  while (1)
    {
      /* 'getopt_long' stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "l::", options, &option_index);

      /* Detect the end of the option */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (options[option_index].flag != 0)
            break;
          printf ("option: %s", options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;
        case 'l':
          puts ("option: -l\n");
          break;
        case '?':
        case 'h':
          /* 'getopt_long' already printed an error message */
          /* TODO: change this to an actual help option */
          break;
        default:
          abort ();
        }
    }

  if (verbose_flag)
    puts ("verbose flag is set");

  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s", argv[optind++]);
      putchar ('\n');
    }

  if (errno != 0)
    {
      printf (_ ("%s: error %d: %s\n"), exec_name, errno, strerror (errno));
      exit (EXIT_FAILURE);
    }

  exit (EXIT_SUCCESS);
}
