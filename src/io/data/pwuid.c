#include <_io_.h>
#include <pwd.h>

extern void print_passwd(const struct passwd*);

int
main(int argc, char **argv)
{
  int             err;
  uid_t           uid;
  struct passwd  *pwd  =  NULL;

  if (argc < 2)
    {
      fprintf(stderr, "usage: <login>\n");
      exit(EXIT_FAILURE);
    }
  uid = (uid_t) atol(argv[1]);

  errno = err = 0;
  if ((pwd = getpwuid(uid)) == NULL)
    {
      err = errno;
      if (err)
        {
          perror(NULL);
          exit(EXIT_FAILURE);
        }
    }
  print_passwd(pwd);


  exit(EXIT_SUCCESS);
}
