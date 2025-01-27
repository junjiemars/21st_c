#include "../_lang_.h"

#define N 32

#if (MSVC) && (WINNT)
#define setenv(name, value, _) _putenv_s (name, value)
#define unsetenv(name) _putenv_s (name, "");
#endif /* MSVC */

static void test_setenv (const char *name, const char *value);

int
main (int argc, const char **argv)
{
  if (argc < 3)
    {
      fprintf (stderr, "please, input an env, such as NAME VALUE\n");
      return 1;
    }

  test_setenv (argv[1], argv[2]);

  return 0;
}

void
test_setenv (const char *name, const char *value)
{
  int rc;
  char *env;
  char *new_value = 0;
  char *new_env = 0;

  rc = setenv (name, value, 0);
  if (rc)
    {
      perror (0);
      return;
    }
  env = getenv (name);
  printf ("%s=%s\n", name, env);

  new_value = malloc (N + 1);
  if (!new_value)
    {
      perror (0);
      goto clean_exit;
    }

  new_env = malloc (N + 1);
  if (!new_env)
    {
      perror (0);
      goto clean_exit;
    }

  strcpy (new_value, value);
  strcat (new_value, "Zzz");
  rc = setenv (name, new_value, 1);
  if (rc)
    {
      perror (0);
      return;
    }
  printf ("%s=%s\n", name, new_value);

  rc = unsetenv (name);
  if (rc)
    {
      perror (0);
      goto clean_exit;
    }
  env = getenv (name);
  printf ("%s=%s\n", name, env);

clean_exit:
  free (new_value);
  free (new_env);
}
