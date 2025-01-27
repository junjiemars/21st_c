#include "../_algo_.h"

struct A
{
  int i1;
  int i2[2];
};

union B
{
  int i1;
  int i2[2];
};

static void test_struct_layout (void);
static void test_union_layout (void);

int
main (void)
{
  test_struct_layout ();
	test_union_layout ();
  return 0;
}

void
test_struct_layout (void)
{
  struct A sa1;
  sa1.i1 = 0x11111111;
  sa1.i2[0] = 0x22222222;
  sa1.i2[1] = 0x33333333;

  printf ("%-12s %16p\n%-12s %16p\n%-12s %16p\n%-12s %16p\n", _str_ (&sa1),
          &sa1, _str_ (&sa1.i1), &sa1.i1, _str_ (&sa1.i2[0]), &sa1.i2[0],
          _str_ (&sa1.i2[1]), &sa1.i2[1]);

  struct A *ha1 = malloc (sizeof (struct A));
  *ha1 = sa1;
  printf ("%-12s %16p\n%-12s %16p\n%-12s %16p\n%-12s %16p\n", _str_ (ha1), ha1,
          _str_ (&ha1->i1), &ha1->i1, _str_ (&ha1->i2[0]), &ha1->i2[0],
          _str_ (&ha1->i2[1]), &ha1->i2[1]);
}

void
test_union_layout (void)
{
  union B sb1;
  sb1.i1 = 0x11111111;
  sb1.i2[0] = 0x22222222;
  sb1.i2[1] = 0x33333333;

  printf ("%-12s %16p\n%-12s %16p\n%-12s %16p\n%-12s %16p\n", _str_ (&sb1),
          &sb1, _str_ (&sb1.i1), &sb1.i1, _str_ (&sb1.i2[0]), &sb1.i2[0],
          _str_ (&sb1.i2[1]), &sb1.i2[1]);

  union B *hb1 = malloc (sizeof (union B));
  *hb1 = sb1;
  printf ("%-12s %16p\n%-12s %16p\n%-12s %16p\n%-12s %16p\n", _str_ (hb1), hb1,
          _str_ (&hb1->i1), &hb1->i1, _str_ (&hb1->i2[0]), &hb1->i2[0],
          _str_ (&hb1->i2[1]), &hb1->i2[1]);
}
