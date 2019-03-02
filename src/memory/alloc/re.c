#include <_memory_.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>


#if defined(NM_HAVE_MALLOC_SIZE_FN)
#  if defined(DARWIN)
#    include <malloc/malloc.h>
#  elif defined(WINNT)
#    define malloc_size(x) _msize((x))
#  elif defined(LINUX)
#    include <malloc.h>
#    define malloc_size(x) malloc_usable_size((x))
#  endif
#endif

void
as_malloc(void) {
	char *s = realloc(0, sizeof(*s)*8);
	assert((0 != s) && strerror(errno));
	if (0 == s[0]) {
		printf("realloc: zero-initialized\n");
	} else {
		printf("realloc: non zero-initialized\n");
	}
	free(s);
}

void
as_free(void) {
	char *s = calloc(8, sizeof(*s));
	assert((0 != s) && strerror(errno));
	char *s1 = realloc(s, 0);
	printf("realloc: freed? = %i\n", (0 == malloc_size(s)));
	printf("realloc: minimum size = %zu\n", malloc_size(s1));
	free(s1);
}

void
enlarge(void) {
	int *i = calloc(8, sizeof(*i));
	assert((0 != i) && strerror(errno));
	printf("calloc: allocated = %zu\n", malloc_size(i));
	i[0] = 0x11223344;
	
	int *i1 = realloc(i, sizeof(*i1)*8*2);
	assert((0 != i1) && strerror(errno));
	assert((0x11223344 == i1[0]) && "realloc: copied");
	printf("realloc: freed? = %i\n", (0 == malloc_size(i)));
	printf("realloc: allocated = %zu\n", malloc_size(i1));
	free(i1);
}

void
shrink(void) {
	int *i = calloc(16, sizeof(*i));
	assert((0 != i) && strerror(errno));
	printf("calloc: allocated = %zu\n", malloc_size(i));
	i[15] = 0x11223344;

	int *i1 = realloc(i, sizeof(*i1)*8);
	assert((0 != i1) && strerror(errno));
	assert((0x11223344 != i1[7]) && "realloc: no copied");
	printf("realloc: freed? = %i\n", (0 == malloc_size(i)));
	printf("realloc: allocated = %zu\n", malloc_size(i1));
	free(i1);
}

int
main(int argc, char **argv) {
	_unused_(argc);
	_unused_(argv);

	as_malloc();
	as_free();
	enlarge();
	shrink();

	return 0;
}
