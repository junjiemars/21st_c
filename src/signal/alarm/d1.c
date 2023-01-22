#include "_signal_.h"
#include <stdio.h>


/*
 * alarm recursively
 *
 */

extern unsigned int  alarm(unsigned int);

static void on_sig_segv(int sig);
static void on_sig_alrm(int sig);

volatile int  alrm_count  =  _ALRM_N_;


int
main(void)
{
  unsigned int  n;
  struct sigaction act;
  volatile int  i;

  setvbuf(stdout, NULL, _IONBF, 0);
  signal(SIGSEGV, on_sig_segv);

  if (sigaction(SIGALRM, NULL, &act) == -1)
    {
      perror(NULL);
      exit(EXIT_FAILURE);
    }

  n = alarm(0);
  assert(n == 0);

  signal(SIGALRM, on_sig_alrm);

  alarm(1);

  for (i = 0; ;i++)
    {
      /* void */
    }

  exit(EXIT_SUCCESS);
}


void
on_sig_segv(int signo)
{
  printf("# %s(%d)\n", _str_(SIGSEGV), signo);
  exit(EXIT_SUCCESS);
}

void
on_sig_alrm(int signo)
{
  printf("# %s(%d)\n", _str_(SIGALRM), signo);

  if (--alrm_count < 1)
    {
      exit(EXIT_SUCCESS);
    }

  alarm(1);
}