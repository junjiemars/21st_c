#include "_signal_.h"
#include <stdio.h>

/*
 * alarm recursively
 *
 */


#define ALRM_N  8
static volatile int  count  =  ALRM_N;



static void on_sig_segv(int sig);
static void on_sig_alrm(int sig);


int
main(void)
{
  volatile int  i  =  0;

  signal(SIGALRM, on_sig_alrm);
  signal(SIGSEGV, on_sig_segv);

  setvbuf(stdout, NULL, _IONBF, 0);

  alarm(1);

  for (;;)
    {
      i += 1;
    }

  exit(EXIT_SUCCESS);
}


void
on_sig_segv(int sig)
{
  if (SIGSEGV == sig)
    {
      printf("# %s\n", _str_(SIGSEGV));
    }

  exit(EXIT_SUCCESS);
}

void
on_sig_alrm(int sig)
{
  if (SIGALRM == sig)
    {
      printf("# %s\n", _str_(SIGALRM));

      if (--count < 1)
        {
          exit(EXIT_SUCCESS);
        }
    }

  alarm(1);
}
