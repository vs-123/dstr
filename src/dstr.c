#include "dstr.h"

#include <stdlib.h>
#include <string.h>

dstr_t
dstr_new (size_t init_capacity)
{
   dstr_t dstr = { 0 };
   dstr.str = malloc (init_capacity * sizeof (char));
   dstr.len = 0;

   return dstr;
}

void
dstr_free (dstr_t *d)
{
   free (d->str);
   d->len = 0;
}

void
dstr_catd (dstr_t *d, const char *str)
{
   size_t s_len = strlen(str);
   memcpy (d->str + d->len, str, s_len);
   d->len += s_len;
}

void
dstr_putc (dstr_t *d, char c)
{
   memcpy (d->str + d->len, &c, 1);
   d->len++;
}
