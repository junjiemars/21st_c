#include <_io_.h>


/*
 * stat file type.
 *
 */


int
main(int argc, char *argv[])
{
  int           rc;
  char         *s;
  struct stat   buf;

  if (argc < 2)
    {
      fprintf(stderr, "usage: %s <pathname> [pathname...]\n",
              basename(argv[0]));
      exit(EXIT_FAILURE);
    }

  for (int i = 1; i < argc; i++)
    {
      rc = lstat(argv[i], &buf);
      if (rc == -1)
        {
          perror(argv[i]);
          continue;
        }

      if (S_ISREG(buf.st_mode))
        {
          s = "regular";
        }
      else if (S_ISDIR(buf.st_mode))
        {
          s = "directory";
        }
      else if (S_ISCHR(buf.st_mode))
        {
          s = "character special";
        }
      else if (S_ISBLK(buf.st_mode))
        {
          s = "block special";
        }
      else if (S_ISFIFO(buf.st_mode))
        {
          s = "fifo";
        }
      else if (S_ISLNK(buf.st_mode))
        {
          s = "symbolic link";
        }
      else if (S_ISSOCK(buf.st_mode))
        {
          s = "socket";
        }
      else
        {
          s = "(unknown file type)";
        }

      printf("%s: %s\n", argv[i], s);

    }

  exit(EXIT_SUCCESS);
}
