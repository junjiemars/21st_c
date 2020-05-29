#include <_memory_.h>
#include <stdio.h>

void 
int_types() {
	uint8_t  x1 = 'A';
	printf("uint8_t: %c \t\t=  " BIT_FMT_8 "\n", x1, BIT_8(x1));

	uint8_t x1s = (uint8_t)(~x1 + 1);
	printf("uint8_t-: -%c \t\t=  " BIT_FMT_8 "\n", x1, BIT_8(x1s));

	uint16_t x2 = 'A';
	printf("uint16_t: %"PRIu16" \t\t=  " BIT_FMT_16 "\n", x2, BIT_16(x2));

	uint16_t x2s = (uint16_t)(~x2 + 1);
	printf("uint16_t-: %"PRIu16" \t=  " BIT_FMT_16 "\n", x2s, BIT_16(x2s));

	uint32_t x4 = 'A';
	printf("uint32_t: %"PRIu32" \t\t=  " BIT_FMT_32 "\n", x4, BIT_32(x4));

	uint64_t x8 = 'A';
	printf("uint64_t: %"PRIu64" \t\t=  " BIT_FMT_64 "\n", x8, BIT_64(x8));
}

void
float_types() {
	ufloat32_t x0f = { .f = 0.0f };
	printf("float: %f \t=  " BIT_FMT_32 "\n", x0f.f, BIT_32(x0f.u));

	ufloat32_t x4f = { .f = 65.0f };
	printf("float: %f \t=  " BIT_FMT_32 "\n", x4f.f, BIT_32(x4f.u));

	ufloat32_t x4fs = { .f = -65.0f };
	printf("float-: %f \t=  " BIT_FMT_32 "\n", x4fs.f, BIT_32(x4fs.u));

	ufloat64_t x8f = { .f = 65.0l };
	printf("double: %lf \t=  " BIT_FMT_64 "\n", x8f.f, BIT_64(x8f.u));

	ufloat64_t x8fs = { .f = -65.0l };
	printf("double-: %lf \t=  " BIT_FMT_64 "\n", x8fs.f, BIT_64(x8fs.u));
}

void 
ieee_float() {
	ufloat32_t x4f =  { .f = 3.14f };
	printf("float: %f \t=  " BIT_FMT_32 "\n", x4f.f, BIT_32(x4f.u));

	ufloat32_t x4fs = { .f = -3.14f };
	printf("float: %f \t=  " BIT_FMT_32 "\n", x4fs.f, BIT_32(x4fs.u));

	printf("   sign     =  " BIT_FMT_32 "\n", BIT_32(x4fs.layout.sign));
	printf("   exponent =  " BIT_FMT_32 "\n", BIT_32(x4fs.layout.exponent));
	printf("   mantissa =  " BIT_FMT_32 "\n", BIT_32(x4fs.layout.mantissa));

	ufloat32_t x4fxs = { .f = -31.4f };
	printf("float: %f \t=  " BIT_FMT_32 "\n", x4fxs.f, BIT_32(x4fxs.u));

	printf("   sign     =  " BIT_FMT_32 "\n", BIT_32(x4fxs.layout.sign));
	printf("   exponent =  " BIT_FMT_32 "\n", BIT_32(x4fxs.layout.exponent));
	printf("   mantissa =  " BIT_FMT_32 "\n", BIT_32(x4fxs.layout.mantissa));

	ufloat32_t x4fxxs = { .f = -314.0f };
	printf("float: %f \t=  " BIT_FMT_32 "\n", x4fxxs.f, BIT_32(x4fxxs.u));

	printf("   sign     =  " BIT_FMT_32 "\n", BIT_32(x4fxxs.layout.sign));
	printf("   exponent =  " BIT_FMT_32 "\n", BIT_32(x4fxxs.layout.exponent));
	printf("   mantissa =  " BIT_FMT_32 "\n", BIT_32(x4fxxs.layout.mantissa));
}

int
main(int argc, char *argv[]) {
	_unused_(argc);
	_unused_(argv);
	
	printf("\n*%s ENDIAN*\n", NM_CPU_LITTLE_ENDIAN ? "LITTLE" : "BIG");
	printf("----------\n");

	printf("\nINT TYPES\n");
	printf("----------\n");
	int_types();

  printf("\nFLOAT TYPES\n");
	printf("----------\n");
	float_types();

	printf("\nIEEE FLOAT\n");
	printf("----------\n");
	ieee_float();
}
