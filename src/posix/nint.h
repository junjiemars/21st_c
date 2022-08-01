#ifndef _NINT_H_
#define _NINT_H_

#include <nore.h>
#include <stdint.h>

#if (MSVC)
#  include <BaseTsd.h>
#endif

#if (NM_HAVE_SSIZE_T)
#  if (MSVC)
typedef SSIZE_T  ssize_t;
#  endif
#else
typedef long  ssize_t;
#endif  /* ssize_t */



/* #if (MSVC) */
/* typedef __int8            int8_t; */
/* typedef unsigned __int8   uint8_t; */
/* typedef __int16           int16_t; */
/* typedef unsigned __int16  uint16_t; */
/* typedef __int32           int32_t; */
/* typedef unsigned __int32  uint32_t; */
/* typedef __int64           int64_t; */
/* typedef unsigned __int64  uint64_t; */
/* #endif */




#endif /* end of _NINT_H_ */
