#include <nm_auto_config.h>
#include <nm_auto_headers.h>
#include <stdio.h>

#define HI_VERSION_MAJOR 2
#define HI_VERSION_MINOR 7
#define HI_VERSION_PATCH 1

unsigned long
hi_version(void) {
  return HI_VERSION_MAJOR * 0x10000 |
         HI_VERSION_MINOR * 0x00100 |
         HI_VERSION_PATCH * 0x00001;
}

int main(int argc, const char *argv[]) {

	if (argc > 1) {
		printf("Hi, %s, Version:%d\n", argv[1], HI_VERSION);
	} else {
		unsigned long ver = hi_version();
		printf("Hello, world!, Version:%lu<=[major:%lu minor:%lu patch:%lu]\n"
					, ver
					, (ver >> 16) & 0xff
					, (ver >> 8) & 0xff
					, ver & 0xff);
	}

	return 0;
}
