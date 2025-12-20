#ifndef __DSTR_H
#define __DSTR_H

#include <stddef.h>

typedef struct
{
   char *str;
   size_t len;
} dstr_t;

dstr_t dstr_new (size_t init_capacity);
void dstr_free (dstr_t*);
void dstr_catd (dstr_t*, const char *);
void dstr_putc (dstr_t*, char);

#endif // __DSTR_H
