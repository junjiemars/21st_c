#include <_x86_.h>
#include <stdio.h>

struct binky {
	int a;
	char b, c, d, e;
	short f;
};

void
structure() {
	struct binky x;
	
	x.e = 'A';
	x.f = x.a;
}

int 
main(int argc, const char *argv[]) {
	_unused_(argc);
	_unused_(argv);

	printf("sizeof(struct binky)=%zu\n", sizeof(struct binky));
	structure();
}
