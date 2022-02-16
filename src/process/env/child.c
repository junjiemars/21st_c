#include "_process_.h"
#include <stdio.h>
#include <stdlib.h>


int
main(void)
{
  int    stat;
  int    overwrite  =  1;
  pid_t  pid;

  setenv("X1", "x", overwrite);

  pid = fork();
  if (0 == pid)
    {
      printf("child: X1 = %s\n", getenv("X1"));
      setenv("X2", "xx", overwrite);
      exit(0);
    }
  else
    {
      waitpid(pid, &stat, overwrite);
      printf("parent: X2 = %s\n", getenv("X2"));
    }
}
