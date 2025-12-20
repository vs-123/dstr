#ifndef __DSTR_H
#define __DSTR_H

#include <stddef.h>

typedef struct
{
   char *str;
   size_t len;
} dstr_t;

dstr_t dstr_new (void);
void dstr_free (dstr_t *);

void dstr_clear(dstr_t *); // destructive
dstr_t dstr_cpy (const dstr_t *d);

void dstr_catd (dstr_t *, const char *); // destructive
dstr_t dstr_cat (const dstr_t *, const char *);

void dstr_putc (dstr_t *, char); // destructive

#endif // __DSTR_H
