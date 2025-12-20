#include "dstr.h"

#include <stdlib.h>
#include <string.h>

dstr_t
dstr_new (void)
{
   dstr_t dstr = { 0 };
   dstr.str = malloc (0);
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
dstr_clear (dstr_t *d)
{
   dstr_free (d);
   *d = dstr_new ();
}

dstr_t
dstr_cpy (const dstr_t *d)
{
   dstr_t dcpy = { 0 };
   dcpy.str = malloc (sizeof (char) * d->len);
   strcpy (dcpy.str, d->str);
   dcpy.len = d->len;
   return dcpy;
}

void
dstr_catd (dstr_t *d, const char *str)
{
   size_t s_len = strlen (str);
   memcpy (d->str + d->len, str, s_len);
   d->len += s_len;
}

dstr_t
dstr_cat (const dstr_t *d, const char *str)
{
   dstr_t dcpy = dstr_cpy (d);
   dstr_catd (&dcpy, str);
   return dcpy;
}

void
dstr_putc (dstr_t *d, char c)
{
   memcpy (d->str + d->len, &c, 1);
   d->len++;
}
