#include "dir.h"

int
dir_select_entries (const struct dirent *ep)
{
  int check_dot_current_directory = (0 != strcmp (ep->d_name, "."));
  int check_dot_dot_parent_directory = (0 != strcmp (ep->d_name, ".."));

  return check_dot_current_directory && check_dot_dot_parent_directory;
}

int
dir_typesort (const struct dirent **a, const struct dirent **b)
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

int
dir_get_directory_entries (const char *const dir_list_name,
                           __attribute__ ((unused)) struct dirent ***dir_list,
                           int *num_entries)
{
  struct dirent **eps;
  *num_entries
      = scandir (dir_list_name, &eps, dir_select_entries, dir_typesort);

  if (*num_entries < 0)
    {
      perror ("Couldn't open the directory");
      return 1;
    }

  *dir_list = eps;

  return 0;
}
