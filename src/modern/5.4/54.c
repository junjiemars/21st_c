#include <modern.h>
#include <stdio.h>

enum corvid { magpie, raven, jay, corvid_n, };
char const*const crows[corvid_n] = {
	[raven] = "raven",
	[magpie] = "magpie",
	[jay] = "jay",
};

int main(int argc, char* argv[]) {
	_unused_(argc);
	_unused_(argv);

	for (unsigned i=0; i < corvid_n; ++i) {
		printf("corvid[%u]=%s\n", i, crows[i]);
	}

	return 0;
}

