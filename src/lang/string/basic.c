#include "../_lang_.h"

size_t
self_strlen (const char *s)
{
  size_t len = 0;
  while (*s++)
    {
      len++;
    }
  return len;
}

int
self_strcmp (const char *s1, const char *s2)
{
  while (*s1 && (*s1 == *s2))
    {
      s1++, s2++;
    }
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int
self_strncmp (const char *s1, const char *s2, size_t n)
{
  while (--n > 0 && *s1 && (*s1 == *s2))
    s1++, s2++;
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

char *
self_strcpy (char *dst, const char *src)
{
  char *r = dst;
  while (0 != (*dst++ = *src++))
    ;
  return r;
}

char *
self_strncpy (char *dst, const char *src, size_t n)
{
  char *r = dst;
  while (n-- > 0 && 0 != (*dst++ = *src++))
    ;
  return r;
}

char *
self_strcat (char *s1, const char *s2)
{
  size_t len = strlen (s1);
  return strcpy (s1 + len, s2);
}

void
test_empty (void)
{
  __attribute__ ((unused)) char *s = "";
  __attribute__ ((unused)) char a[] = "";

  assert (0 == strlen (s));
  assert (0 == strlen (a));
}

void
test_static_len (void)
{
  assert ((sizeof ("abc") - 1) == strlen ("abc"));
}

void
test_strlen (char *s)
{
  __attribute__ ((unused)) size_t len1 = strlen (s);
  __attribute__ ((unused)) size_t len2 = self_strlen (s);

  assert ((len1 == len2) && "self_strlen() failed");
}

void
test_strcpy (void)
{
  char buf1[8], buf2[8];
  int len = sizeof (buf1) / sizeof (*buf1);

  memset (buf1, 0x11, len);
  assert (0x11 == buf1[len - 1] && "strcpy(), 0x11 != <tailing>");

  strcpy (buf1, "");
  assert (0 == strlen (buf1) && "strcpy(), copy empty string failed");

  strcpy (buf1, "abc");
  assert (0 == buf1[strlen (buf1)] && "strcpy(), 0 != <tailing>");
  assert (0x11 == buf1[len - 1] && "strcpy(), 0x11 != <tailing>");

  self_strcpy (buf2, "abc");
  assert (0 == strncmp (buf1, buf2, len) && "self_strcpy(), 0 != <tailing>");
}

void
test_strncpy (void)
{
  char b1[8];
  __attribute__ ((unused)) char _z1[8]; /* stack overflow zone */
  char b2[8];
  __attribute__ ((unused)) char _z2[8]; /* stack overflow zone */
  char *s1 = "abc";
  char *s2 = "abcd";
  char *sx8 = "aaaaaaaa";

  /* len < len(src), dst should not terminated */
  memset (b1, 'X', sizeof (b1));
  memset (b2, 'X', sizeof (b2));
  strncpy (b1, s2, 2);
  self_strncpy (b2, s2, 2);
  assert (0 == memcmp (s1, b1, 2) && 0 == memcmp (s1, b2, 2));

  /* len > len(src), dst should terminated */
  memset (b1, 'X', sizeof (b1));
  memset (b2, 'X', sizeof (b2));
  strncpy (b1, s2, 5);
  self_strncpy (b2, s2, 5);
  assert (0 == strcmp (s2, b1) && 0 == strcmp (s2, b2));

  /* len == len(src), dst should not terminated */
  memset (b1, 'X', sizeof (b1));
  memset (b2, 'X', sizeof (b2));
  strncpy (b1, s2, 4);
  self_strncpy (b2, s2, 4);
  assert (0 == memcmp (s2, b1, 4) && 0 == memcmp (s2, b2, 4));

  /* strncpy cannot avoid overflow on src */
  memset (b1, 'X', sizeof (b1) - 1);
  memset (b2, 'X', sizeof (b2) - 1);
  strncpy (b1, sx8, sizeof (b1));
  self_strncpy (b2, sx8, sizeof (b2));
  assert (0 == memcmp (b1, sx8, sizeof (b1))
          && 0 == memcmp (b2, sx8, sizeof (b2)));
}

void
test_strcmp (char *s)
{
  size_t len = strlen (s);
  char *d1 = malloc (len + 1);
  char *d2 = strcpy (d1, s);
  char buf[8];

  assert (d1 == d2 && "strcpy(), diff between dest and retval");
  assert (0 == strcmp (s, d1) && 0 == self_strcmp (s, d1)
          && 0 == self_strcmp (d1, s) && "self_strcpy(), failed");

  d2 = buf;
  strcpy (d1, "abc");
  strcpy (d2, "abx");
  assert (0 != strcmp (d1, d2) && 0 != self_strcmp (d1, d2)
          && "self_strcmp(), 0 !=");
  assert (0 > strcmp (d1, d2) && 0 > self_strcmp (d1, d2)
          && 0 < self_strcmp (d2, d1) && "self_strcmp(), 0 ><");
  free (d1);
}

void
test_strncmp (void)
{
  char *s1 = "abc";
  char *s2 = "abcd";

  assert (0 == strncmp (s1, s2, 3) && 0 == self_strncmp (s1, s2, 3));
  assert (0 > strncmp (s1, s2, 4) && 0 > self_strncmp (s1, s2, 4));
  assert (0 > strncmp (s1, s2, 10) && 0 > self_strncmp (s1, s2, 10));
}

void
test_strcat (void)
{
  char buf1[8];
  __attribute__ ((unused)) char buf2[8];

  memset (buf1, 0, sizeof (buf1) / sizeof (*buf1));
  memset (buf2, 0, sizeof (buf2) / sizeof (*buf2));

  strcat (&buf1[0], "abc");
  assert (0 == strcmp (buf1, "abc") && "strcat(), failed");

  self_strcat (buf2, "abc");
  assert (0 == strcmp (buf2, buf1) && "self_strcat(), failed");

  self_strcat (buf2, "");
  assert (0 == strcmp (buf2, buf1) && "self_strcat(), failed");

  self_strcat (buf2, "x");
  assert (0 == strcmp (buf2, "abcx") && "self_strcat(), failed");
}

int
main (__attribute__ ((unused)) int argc, char **argv)
{
  test_empty ();
  test_static_len ();
  test_strlen (argv[0]);
  test_strcpy ();
  test_strncpy ();
  test_strcmp (argv[0]);
  test_strncmp ();
  test_strcat ();

  return 0;
}
