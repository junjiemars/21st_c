#include <_lang_.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#if !(NM_HAVE_C11_STATIC_ASSERT)
#  ifdef static_assert
#    undef static_assert
#  endif
#  define sa_cat_(a, b) a##b
#  define sa_cat(a, b) sa_cat_(a, b)
#  define static_assert(e, m)  enum {sa_cat(static_assert_, __LINE__) = 1/!!(e)}
#endif

int
sqr(int x) {
	assert((0 < x) && (x < 100));
	return (x * x);
}

int
main(int argc, const char* argv[]) {

	assert((argc > 1) && "usage: one <int>");

#ifdef NDEBUG
	printf("in RELEASE mode\n");
#else
	printf("in DEUBG mode\n");
#endif

	static_assert(4 == sizeof(int), "sizeof(int) != 4 bytes");

	if (argc > 1) {
		int n = atoi(argv[1]);
		printf("sqr(%i)=%u\n", n, sqr(n));
	}


	return 0;
}
