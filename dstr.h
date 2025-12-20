/* dstr.h
   Copyright (C) 2025 vs-123
   Licensed under the GNU Affero General Public License v3.0 or later.
   No warranty is provided.
   Full license text is located at the end of this file and in the LICENSE
   file.

   ~=~=~=~=~=~
   || USAGE ||
   ~=~=~=~=~=~
      In exactly ONE source file, do this:
         #define DSTR_IMPL
         #include "dstr.h"

      In all other files, simply:
         #include "dstr.h"
*/

#ifndef __DSTR_H
#define __DSTR_H

#include <stdarg.h>
#include <stddef.h>

typedef struct
{
   char *str;
   size_t len;
} dstr_t;

dstr_t dstr_new (void);
void dstr_free (dstr_t *);

void dstr_clear (dstr_t *); /* destructive */
dstr_t dstr_cpy (const dstr_t *d);

void dstr_catd (dstr_t *, const char *); /* destructive */
dstr_t dstr_cat (const dstr_t *, const char *);

void dstr_catfmtvd (dstr_t *d, const char *fmt, va_list ap);
void dstr_catfmtd (dstr_t *, const char *fmt, ...); /* destructive */
dstr_t dstr_catfmt (const dstr_t *, const char *fmt, ...);

void dstr_putc (dstr_t *, char);        /* destructive */
void dstr_putl (dstr_t *, long);        /* destructive */
void dstr_putd (dstr_t *, double, int); /* destructive */

#endif /* __DSTR_H */

/**********/

#ifdef DSTR_IMPL

#include <stdarg.h>
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
dstr_catfmtvd (dstr_t *d, const char *fmt, va_list ap)
{
   const char *p = fmt;

   while (*p)
      {
         if (*p == '%')
            {
               if (*(p + 1) == '\0')
                  break;
               p++;
               switch (*p)
                  {
                  case 's':
                     {
                        char *s = va_arg (ap, char *);
                        dstr_catd (d, s ? s : "(NULL)");
                        break;
                     }
                  case 'd':
                     {
                        dstr_putl (d, (long)va_arg (ap, int));
                        break;
                     }
                  case 'l':
                     {
                        if (*(p + 1) == 'd')
                           {
                              p++;
                              dstr_putl (d, va_arg (ap, long));
                           }
                        break;
                     }
                  case 'f':
                     {
                        dstr_putd (d, va_arg (ap, double), 6);
                     }
                  case '%':
                     {
                        dstr_putc (d, '%');
                        break;
                     }
                  }
            }
         else
            {
               dstr_putc (d, *p);
            }
         p++;
      }
}

void
dstr_catfmtd (dstr_t *d, const char *fmt, ...)
{
   va_list ap;

   va_start (ap, fmt);
   dstr_catfmtvd (d, fmt, ap);
   va_end (ap);
}

dstr_t
dstr_catfmt (const dstr_t *d, const char *fmt, ...)
{
   va_list ap;
   dstr_t dcpy = dstr_cpy (d);

   va_start (ap, fmt);
   dstr_catfmtvd (&dcpy, fmt, ap);
   va_end (ap);

   return dcpy;
}

void
dstr_putc (dstr_t *d, char c)
{
   memcpy (d->str + d->len, &c, 1);
   d->len++;
}

void
dstr_putl (dstr_t *d, long n)
{
   char buffer[32];
   unsigned long ul;
   int i = 0;

   if (n == 0)
      {
         dstr_putc (d, '0');
         return;
      }

   if (n < 0)
      {
         dstr_putc (d, '-');
         ul = (unsigned long)-(n + 1) + 1;
      }
   else
      {
         ul = (unsigned long)n;
      }

   while (ul > 0)
      {
         buffer[i++] = (ul % 10) + '0';
         ul /= 10;
      }

   while (i > 0)
      {
         dstr_putc (d, buffer[--i]);
      }
}

void
dstr_putd (dstr_t *d, double n, int precision)
{
   long ipart = (long)n;
   double fpart = n - (double)ipart;
   int i;

   dstr_putl (d, ipart);

   if (precision > 0)
      {
         dstr_putc (d, '.');
         if (fpart < 0)
            fpart = -fpart;

         for (i = 0; i < precision; i++)
            {
               fpart *= 10;
               dstr_putc (d, ((int)fpart % 10) + '0');
            }
      }
}

#endif /* DSTR_IMPL */

/*
~=~=~=~=~=~=~=~=~=~=~=~=~=~
||   AGPLv3.0-or-later   ||
~=~=~=~=~=~=~=~=~=~=~=~=~=~
    dstr.h -- a dead-simple single-header dynamic string library
    Copyright (C) 2025  vs-123

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
