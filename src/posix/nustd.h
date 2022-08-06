/**
 * Nore Unix standard
 */

#ifndef _NUSTD_H_
#define _NUSTD_H_


#include <nore.h>

#if (DARWIN)
#  if !defined(_DARWIN_C_SOURCE)
#    define _DARWIN_C_SOURCE
#  endif
#endif


#if (LINUX)
#  if !defined(_GNU_SOURCE)
#    define _GNU_SOURCE
#  endif
#endif


#include <npstd.h>
#include <ncstd.h>


#if (MSVC)
#  if defined(__attribute__)
#    if defined(unused)
#      define __attribute____unused__  unused
#      undef unused
#    endif
#    if defined(fallthrough)
#      define __attribute____fallthrough__  fallthrough
#      undef fallthrough
#    endif
#  endif
#  include <BaseTsd.h>
#  include <windows.h>
#  include <process.h>
#  if defined(__attribute____unused__)
#    define unused  __attribute____unused__
#    undef __attribute____unused__
#  endif
#  if defined(__attribute____fallthrough__)
#    define fallthrough  __attribute____fallthrough__
#    undef __attribute____fallthrough__
#  endif
#else
#  include <sys/types.h>
#  include <unistd.h>
#endif


#if (MSVC) && (NM_HAVE_SSIZE_T)
typedef SSIZE_T  ssize_t
#endif  /* ssize_t */



#if (MSVC)
#  define sleep(x)  Sleep((x) * 1000)
#endif  /* sleep */


#if (MSVC)
#  define getpid  _getpid
#endif  /* getpid */




#endif /* _NUSTD_H_ */
