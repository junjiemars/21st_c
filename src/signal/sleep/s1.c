#include "_signal_.h"


/*
 * a sleep function implementation.
 *
 */


unsigned sleep(unsigned);

extern void race(long);
extern unsigned  N;

unsigned
sleep(unsigned nsecs)
{
  alarm(nsecs);

  /* race condition: between alarm and pause */
  race(N);

  pause();

  return alarm(0);
}
