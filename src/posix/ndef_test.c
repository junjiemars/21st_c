#include "ndef.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


static void test_unused(int);
static void test_unused_fn(void);
static void test_fallthrough(int);

static void test_restrict(const int *restrict, const char *restrict);
static void test_static_assert(void);
static void test_alignof(void);
static void test_alignas(void);

static void test_isut(void);
static void test_nof(void);
static void test_swp(void);

struct X {
  char alignas(int) ss[3];
  int  i;
};


int
main(int argc, char **argv)
{
  test_unused(1);
  test_fallthrough(1);

  test_restrict((const int *restrict) &argc, (const char *restrict) argv[0]);
  test_static_assert();
  test_alignof();
  test_alignas();

  test_isut();
  test_nof();
  test_swp();

  return 0;
}


void
test_unused(__attribute__((unused)) int a)
{
  __attribute__((unused)) int b;
  /* void */
}

__attribute__((unused))
void
test_unused_fn(void)
{
  /* void */
}

void
test_fallthrough(int a)
{
  switch (a)
    {
    case 0:
      __attribute__((fallthrough));
    case 1:
      break;
    }
}

void
test_restrict(const int *restrict ii, const char *restrict ss)
{
  printf("%s(%i, %s)\n", __FUNCTION__, *ii, ss);
}

void
test_static_assert(void)
{
  static_assert(sizeof(char) == 1, "sizeof(char) must be 1 byte");
}

void
test_alignof(void)
{
  size_t s = alignof(short);
  assert(s == 2);
}

void
test_alignas(void)
{
#define _m_(x, y)  ((size_t) &(x)) == (((size_t) &(x))/(y))*(y)
  char alignas(double) c1  =  'A';
  assert(_m_(c1, sizeof(double)));

  struct X  x1 = {0};
  assert(_m_(x1, sizeof(int) * 2));
#undef _m_
}

void
test_isut(void)
{
  printf("_isut_(char) = %d\n_isut_(unsigned char) = %d\n",
         _isut_(char), _isut_(unsigned char));
}


void
test_nof(void)
{
  int  a[]  =  { 1, 2, 3, };
  assert(_nof_(a) == sizeof(a)/sizeof(*a));
  printf("_nof_(a) = %zu\n", _nof_(a));
}


void
test_swp(void)
{
	int i1 = 0x1122, i2 = 0x3344;
	_swp_(&i1, &i2, sizeof(int));
	assert(0x1122 == i2 && 0x3344 == i1);

	int *p1 = &i1, *p2 = &i2;
	_swp_(&p1, &p2, sizeof(int*));
	assert(0x1122 == *p1 && 0x3344 == *p2);

	char *s1 = "abcdef", *s2 = "12345";
	_swp_(&s1, &s2, sizeof(char*));
	assert(strcmp("abcdef", s2) == 0 && strcmp("12345", s1) == 0);

	char sa1[] = "abcdef", sa2[] = "12345";
	char *ss1 = sa1, *ss2 = sa2;
	_swp_(&ss1, &ss2, sizeof(char*));
	assert(strcmp("abcdef", ss2) == 0 && strcmp("12345", ss1) == 0);
}
