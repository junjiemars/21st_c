#include <_io_.h>
#include <stdio.h>
#include <string.h>

#define NUM_LEN 7
#define OUT_FMT                                 \
  "> No.%d found\n"                             \
  "------------\n"                              \
  "  num: %s\n"                                 \
  " name: %s\n"                                 \
  "stock: %d\n"                                 \
  "price: %lf\n"

#define PANIC "!panic"

typedef struct record_s
{
  char num[NUM_LEN];
  char name[25];
  int stock;
  double price;
} record_s;

typedef struct index_s
{
  char num[NUM_LEN];
  int idx;
} index_s;

/* generate bin and idx files */
void out_records(const char *inpath,
                 const char *binpath,
                 const char *idxpath);

/* seek record by idx */

void seek_record(const char *path, int n);

/* find record by num filed */
void find_record(const char *binpath,
                 const char *idxpath,
                 const char *num);

void
out_records(const char *inpath,
            const char *binpath,
            const char *idxpath)
{
  FILE *in = 0, *out_bin = 0, *out_idx = 0;
  in = fopen(inpath, "r");
  if (!in)
    {
      perror(PANIC);
      return;
    }
  out_bin = fopen(binpath, "wb");
  if (!out_bin)
    {
      perror(PANIC);
      goto clean_exit;
    }
  out_idx = fopen(idxpath, "wb");
  if (!out_idx)
    {
      perror(PANIC);
      goto clean_exit;
    }

  record_s ss;
  index_s is;
  memset(&ss, 0, sizeof(record_s));
  memset(&is, 0, sizeof(index_s));

  int n = 0;
  while (4 == fscanf(in, "%s %s %d %lf",
                     ss.num,
                     ss.name,
                     &ss.stock,
                     &ss.price))
    {
      if (1 != fwrite(&ss, sizeof(record_s), 1, out_bin))
        {
          perror(PANIC);
          goto clean_exit;
        }
      strncpy(is.num, ss.num, NUM_LEN);
      is.idx = n;
      if (1 != fwrite(&is, sizeof(index_s), 1, out_idx))
        {
          perror(PANIC);
          goto clean_exit;
        }
      n++;
    }

 clean_exit:
  if (out_idx)
    {
      fclose(out_idx);
    }
  if (out_bin)
    {
      fclose(out_bin);
    }
  if (in)
    {
      fclose(in);
    }
}

void
seek_record(const char *path, int n)
{
  fprintf(stdout, "\nseeking %i ...\n", n);

  FILE *in = fopen(path, "rb");
  if (!in)
    {
      perror(PANIC);
      return;
    }

  if (fseek(in, (n-1)*sizeof(record_s), SEEK_SET))
    {
      perror(PANIC);
      goto clean_exit;
    }

  record_s ss;
  memset(&ss, 0, sizeof(record_s));
  if (1 != fread(&ss, sizeof(record_s), 1, in))
    {
      if (ferror(in))
        {
          perror(PANIC);
        }
      if (feof(in))
        {
          fprintf(stderr, "! No.%i no found\n------------\n", n);
        } 
      goto clean_exit;
    }
  fprintf(stdout, OUT_FMT,
          n, ss.num, ss.name, ss.stock, ss.price);

 clean_exit:
  if (in)
    {
      fclose(in);
    }
}

void
find_record(const char *binpath,
            const char *idxpath,
            const char *num)
{
  fprintf(stdout, "\nfinding %s ...\n", num);

  FILE *idx = 0, *bin = 0;
  idx = fopen(idxpath, "rb");
  if (!idx)
    {
      perror(PANIC);
      return;
    }
  bin = fopen(binpath, "rb");
  if (!bin)
    {
      perror(PANIC);
      goto clean_exit;
    }

  index_s is;
  record_s ss;
  memset(&is, 0, sizeof(index_s));
  memset(&ss, 0, sizeof(record_s));

  while (1 == fread(&is, sizeof(index_s), 1, idx))
    {
      if (0 == strncmp(num, is.num, NUM_LEN))
        {
          if (0 == fseek(bin, (is.idx)*sizeof(record_s), SEEK_SET))
            {
              if (1 == fread(&ss, sizeof(record_s), 1, bin))
                {
                  fprintf(stdout, OUT_FMT,
                          is.idx, ss.num, ss.name, ss.stock, ss.price);
                  goto clean_exit;
                }
            }
        }
    }

  if (ferror(idx))
    {
      perror(PANIC);
    }
  if (feof(idx))
    {
      fprintf(stdout, "! %s no found\n------------\n", num);
    }

 clean_exit:
  if (idx)
    {
      fclose(idx);
    }
  if (bin)
    {
      fclose(bin);
    }
}


int
main(int argc, char **argv)
{
  if (argc < 4)
    {
      fprintf(stderr,
              "where the records.txt/records.bin/records.idx"
              "located?\n");
      return 1;
    }

  const char *txt = argv[1];
  const char *bin = argv[2];
  const char *idx = argv[3];

  out_records(txt, bin, idx);

  seek_record(bin, 1);
  seek_record(bin, 3);
  seek_record(bin, 10);

  find_record(bin, idx, "PKL070");
  find_record(bin, idx, "DKP080");
  find_record(bin, idx, "DKP081");

  return 0;
}
