#include "_stdio1_.h"


extern char  *strerror(int);


static  unsigned char  _stdin_buf_[BUFSIZ];
static  unsigned char  _stdout_buf_[BUFSIZ];

static FILE  _stdin_  =
  {
    .fd        =  STDIN_FILENO,
    .buf_type  =  _IOLBF,
    .buf_read  =  _stdin_buf_,
    .buf_size  =  BUFSIZ
  };

static FILE  _stdout_  =
  {
    .fd         =  STDOUT_FILENO,
    .buf_type   =  _IOLBF,
    .buf_write  =  _stdout_buf_,
    .buf_size   =  BUFSIZ
  };

static FILE  _stderr_  =
  {
    .fd        =  STDERR_FILENO,
    .buf_type  =  _IONBF,
    .buf_size  =  0
  };

FILE  *stdin   =  &_stdin_;
FILE  *stdout  =  &_stdout_;
FILE  *stderr  =  &_stderr_;



int
feof(FILE *stream)
{
  return stream->eof;
}

int
ferror(FILE *stream)
{
  return stream->err;
}


void
clearerr(FILE *stream)
{
  stream->err = stream->eof = 0;
}


int
fileno(FILE *stream)
{
  return stream->fd;
}


int
fclose(FILE *stream)
{
  if (stream == NULL)
    {
      return 0;
    }

  fflush(stream);

  if (stream == stdin || stream == stdout || stream == stderr)
    {
      return 0;
    }

  if (stream->buf_read != NULL)
    {
      free(stream->buf_read);
      stream->buf_read = NULL;
    }

  if (stream->buf_write != NULL)
    {
      free(stream->buf_write);
      stream->buf_write = NULL;
    }

  if (stream->stat != NULL)
    {
      free(stream->stat);
      stream->stat = NULL;
    }

  if (close(stream->fd) == -1)
    {
      return EOF;
    }

  free(stream);

  return 0;
}

int
fflush(FILE *stream)
{
  if (stream->ptr_write > 0)
    {
      if (write(stream->fd, stream->buf_write, stream->ptr_write) == -1)
        {
          stream->err = errno;
          return EOF;
        }
    }

  return 0;
}


FILE *
fopen(const char *restrict path, const char *restrict mode)
{
  FILE  *ss;

  ss = calloc(1, sizeof(FILE));
  if (ss == NULL)
    {
      return NULL;
    }

  if (strcmp("r", mode) == 0 || strcmp("rb", mode) == 0)
    {
      ss->fd = open(path, O_RDONLY);
    }
  else if (strcmp("r+", mode) == 0 || strcmp("rb+", mode) == 0
           || strcmp("r+b", path) == 0)
    {
      ss->fd = open(path, O_RDWR);
    }
  else if (strcmp("w", mode) == 0 || strcmp("wb", mode) == 0)
    {
      ss->fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
  else if (strcmp("w+", mode) == 0 || strcmp("wb+", mode) == 0
           || strcmp("w+b", mode) == 0)
    {
      ss->fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);
    }
  else if (strcmp("a", path) == 0)
    {
      ss->fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
  else if (strcmp("a+", path) == 0 || strcmp("ab+", path) == 0
           || strcmp("a+b", path) == 0)
    {
      ss->fd = open(path, O_RDWR | O_CREAT | O_APPEND, 0666);
    }
  else
    {
      errno = EINVAL;
      goto clean_exit;
    }

  if (ss->fd == -1)
    {
      goto clean_exit;
    }

  ss->stat = calloc(1, sizeof(*ss->stat));
  if (ss->stat == NULL)
    {
      ss->err = errno;
      goto clean_exit;
    }

  if (fstat(ss->fd, ss->stat) == -1)
    {
      ss->err = errno;
      goto clean_exit;
    }

  ss->buf_size = ss->stat->st_blksize;

  ss->buf_read = calloc(1, ss->buf_size);
  if (ss->buf_read == NULL)
    {
      ss->err = errno;
      goto clean_exit;
    }

  ss->buf_write = calloc(1, ss->buf_size);
  if (ss->buf_write == NULL)
    {
      ss->err = errno;
      goto clean_exit;
    }

  return ss;

 clean_exit:
  fclose(ss);

  return NULL;
}

int
fgetc(FILE *stream)
{
  ssize_t  n;

  if (stream->eof)
    {
      return stream->eof;
    }

  if (stream->ptr_read < stream->n_read)
    {
      return stream->buf_read[stream->ptr_read++];
    }

  n = read(stream->fd, stream->buf_read, stream->buf_size);

  if (n == -1)
    {
      stream->err = errno;
      return EOF;
    }

  if (n == 0)
    {
      return stream->eof = EOF;
    }

  stream->n_read = n;
  stream->ptr_read = 0;

  return stream->buf_read[stream->ptr_read++];
}

int
fputc(int c, FILE *stream)
{
  ssize_t  n;

  if (stream->ptr_write == stream->buf_size)
    {
      n = write(stream->fd, stream->buf_write, stream->buf_size);

      if (n == -1)
        {
          stream->err = errno;
          return EOF;
        }

      stream->ptr_write = 0;
    }

  stream->buf_write[stream->ptr_write++] = (unsigned char ) c;

  switch (stream->buf_type)
    {
    case _IOLBF:
      if (c == '\n')
        {
          n = write(stream->fd, stream->buf_write, stream->ptr_write);
          if (n == -1)
            {
              stream->err = errno;
              return EOF;
            }
          stream->ptr_write = 0;
        }
      break;

    case _IONBF:
      n = write(stream->fd, stream->buf_write, 1);
      if (n == -1)
        {
          stream->err = errno;
          return EOF;
        }
      stream->ptr_write = 0;
      break;
    }

  return c;
}


size_t
fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream)
{
  size_t          n, m, sum;
  ssize_t         r;
  unsigned char  *cur;

  if (size == 0 || nitems == 0)
    {
      return 0;
    }

  n = (size * nitems) / stream->buf_size;
  m = (size * nitems) % stream->buf_size;
  sum = 0;
  cur = (unsigned char *) ptr;

  if (m > 0)
    {
      r = read(stream->fd, cur, m);
      if (r == -1)
        {
          stream->err = errno;
          return 0;
        }
      if (r == 0)
        {
          return 0;
        }

      sum += r;
      cur += r;
    }

  for (size_t i = 0; i < n; i++)
    {
      r = read(stream->fd, cur, stream->buf_size);
      if (r == -1)
        {
          stream->err = errno;
          return 0;
        }
      if (r == 0)
        {
          break;
        }

      sum += r;
      cur += r;
    }

  return (sum / size);
}


size_t
fwrite(const void *restrict ptr, size_t size, size_t nitems,
       FILE *restrict stream)
{
  size_t          n, m, sum;
  ssize_t         w;
  unsigned char  *cur;

  n = (size * nitems) / stream->buf_size;
  m = (size * nitems) % stream->buf_size;
  sum = 0;
  cur = (unsigned char *) ptr;

  if (m > 0)
    {
      w = write(stream->fd, cur, m);
      if (w == -1)
        {
          stream->err = errno;
          return 0;
        }
      if (w == 0)
        {
          return 0;
        }

      sum += w;
      cur += w;
    }


  for (size_t i = 0; i < n; i++)
    {
      w = write(stream->fd, cur, stream->buf_size);
      if (w == -1)
        {
          stream->err = errno;
          return 0;
        }
      if (w == 0)
        {
          return 0;
        }

      cur += w;
      sum += w;
    }

  return (sum / size);
}


int
vsnprintf(char *restrict s, size_t n, const char *restrict format, va_list ap)
{
  char                          *ps;
  __attribute__((unused)) char   zero;
  unsigned int                   width;

  ps = s;

  while (*format && (size_t) (ps - s) < n)
    {
      if (*format == '%')
        {
          zero = *++format == '0' ? '0' : ' ';
          width = 0;

          while (*format >= '0' && *format <= '9')
            {
              width = width * 10 + (*format++ - '0');
            }

          switch (*format)
            {
            /* case 'i': */

            case 's':
              {
                char *s1 = va_arg(ap, char *);
                while (*s1)
                  {
                    *ps++ = *s1++;
                  }
                format++;
                continue;
              }

            default:
              {
                *ps++ = *format++;
                continue;
              }
            }

          format++;
        }
      else
        {
          *ps++ = *format++;
        }
    }

  return (int) (ps - s);
}


int
fprintf(FILE * restrict stream, const char * restrict format, ...)
{
  static char  buf[NM_LINE_MAX];
  int          len;
  size_t       n;

  va_list ap;
  va_start(ap, format);

  len = vsnprintf(buf, NM_LINE_MAX-1, format, ap);
  va_end(ap);

  n = fwrite(buf, sizeof(*buf), len, stream);

  return n;
}
