#ifndef REAL_H
#define REAL_H

/**
 * Real number
 */
#include <stddef.h>
#include <stdbool.h>

struct Real;

bool real_from_decimal (bool, unsigned int, unsigned int, struct Real *);
bool real_add (struct Real *, struct Real *, struct Real *);
char *real_str (struct Real *, char *, size_t);
struct Real *new_real (void);
void free_real (struct Real *);

#endif