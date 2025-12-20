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
   size_t cap;
} dstr_t;

dstr_t dstr_new (void);
void dstr_free (dstr_t *);

void dstr_clear (dstr_t *); /* destructive */
dstr_t dstr_cpy (const dstr_t *d);

void dstr_catd (dstr_t *, const char *); /* destructive */
dstr_t dstr_cat (const dstr_t *, const char *);

void dstr_putl_ext (dstr_t *d, long n, int width, int precision, int flags);
void dstr_putd_ext (dstr_t *d, double n, int width, int precision, int flags);
void dstr_catd_ext (dstr_t *d, const char *str, int width, int precision,
                    int flags);

void dstr_catfmtvd (dstr_t *d, const char *fmt, va_list ap);
void dstr_catfmtd (dstr_t *, const char *fmt, ...); /* destructive */
dstr_t dstr_catfmt (const dstr_t *, const char *fmt, ...);

void dstr_putc (dstr_t *, char); /* destructive */

void dstr_putl (dstr_t *, long);        /* destructive */
void dstr_putd (dstr_t *, double, int); /* destructive */

void dstr_putul_internal (dstr_t *d, unsigned long ul, int base, int uppercase,
                          int width, int precision, int flags);

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
   dstr.len    = 0;
   dstr.cap    = 16;
   dstr.str    = malloc (dstr.cap * sizeof (char));
   dstr.str[0] = '\0';

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
   dstr_t dcpy;

   dcpy.len = d->len;
   dcpy.cap = d->len + 1;
   dcpy.str = malloc (dcpy.cap);

   if (d->len > 0)
      {
         memcpy (dcpy.str, d->str, d->len);
      }

   dcpy.str[dcpy.len] = '\0';

   return dcpy;
}

void
dstr_catd (dstr_t *d, const char *str)
{
   dstr_catd_ext (d, str, 0, -1, 0);
}

dstr_t
dstr_cat (const dstr_t *d, const char *str)
{
   dstr_t dcpy = dstr_cpy (d);
   dstr_catd (&dcpy, str);
   return dcpy;
}

#define DSTR_F_LEFT (1 << 0)  /* '-' left justify */
#define DSTR_F_PLUS (1 << 1)  /* '+' forced sign */
#define DSTR_F_ZERO (1 << 2)  /* '0' zero padding */
#define DSTR_F_SPACE (1 << 3) /* ' ' positive sign */
#define DSTR_F_HASH (1 << 4)  /* '#' alternate form */

void
dstr_catfmtvd (dstr_t *d, const char *fmt, va_list ap)
{
   const char *p = fmt;

   /* %[flags][width][.precision][length]type */
   while (*p)
      {
         if (*p == '%' && *(p + 1) != '\0')
            {
               int flags     = 0;
               int width     = 0;
               int precision = -1;
               p++;

               /* flags */
               while (1)
                  {
                     if (*p == '-')
                        flags |= DSTR_F_LEFT;
                     else if (*p == '+')
                        flags |= DSTR_F_PLUS;
                     else if (*p == '0')
                        flags |= DSTR_F_ZERO;
                     else if (*p == ' ')
                        flags |= DSTR_F_SPACE;
                     else if (*p == '#')
                        flags |= DSTR_F_HASH;
                     else
                        break;
                     p++;
                  }

               /* width */
               if (*p == '*')
                  {
                     width = va_arg (ap, int);
                     p++;
                  }
               else
                  {
                     while (*p >= '0' && *p <= '9')
                        {
                           width = width * 10 + (*p - '0');
                           p++;
                        }
                  }

               /* precision */
               if (*p == '.')
                  {
                     p++;
                     if (*p == '*')
                        {
                           precision = va_arg (ap, int);
                           p++;
                        }
                     else
                        {
                           precision = 0;
                           while (*p >= '0' && *p <= '9')
                              {
                                 precision = precision * 10 + (*p - '0');
                                 p++;
                              }
                        }
                  }

               switch (*p)
                  {
                  case 's':
                     {
                        char *s = va_arg (ap, char *);
                        dstr_catd_ext (d, s ? s : "(NULL)", width, precision,
                                       flags);
                        break;
                     }
                  case 'd':
                  case 'i':
                     {
                        dstr_putl_ext (d, (long)va_arg (ap, int), width,
                                       precision, flags);
                        break;
                     }
                  case 'l':
                     {
                        if (*(p + 1) == 'd' || *(p + 1) == 'i')
                           {
                              p++;
                              dstr_putl_ext (d, va_arg (ap, long), width,
                                             precision, flags);
                           }
                        break;
                     }
                  case 'f':
                     {
                        dstr_putd_ext (d, va_arg (ap, double), width,
                                       (precision > -1) ? precision : 6,
                                       flags);
                        break;
                     }
                  case 'u':
                     dstr_putul_internal (d, va_arg (ap, unsigned int), 10, 0,
                                          width, precision, flags);
                     break;
                  case 'x':
                  case 'X':
                     dstr_putul_internal (d, va_arg (ap, unsigned int), 16,
                                          (*p == 'X'), width, precision,
                                          flags);
                     break;
                  case 'p':
                     {
                        void *ptr = va_arg (ap, void *);
                        if (ptr == NULL)
                           {
                              dstr_catd (d, "(nil)");
                           }
                        else
                           {
                              dstr_putul_internal (d, (unsigned long)ptr, 16,
                                                   0, width, precision,
                                                   flags | DSTR_F_HASH);
                           }
                     }
                     break;
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
   if (d->len + 1 >= d->cap)
      {
         d->cap *= 2;
         d->str = realloc (d->str, d->cap);
      }
   d->str[d->len++] = c;
   d->str[d->len]   = '\0';
}

void
dstr_putl (dstr_t *d, long n)
{
   dstr_putl_ext (d, n, 0, -1, 0);
}

void
dstr_putul_internal (dstr_t *d, unsigned long ul, int base, int uppercase,
                     int width, int precision, int flags)
{
   char buffer[64];
   int i              = 0, pad, len;
   const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
   char pad_char = (flags & DSTR_F_ZERO && !(flags & DSTR_F_LEFT)) ? '0' : ' ';

   if (ul == 0 && precision != 0)
      buffer[i++] = '0';
   while (ul > 0)
      {
         buffer[i++] = digits[ul % base];
         ul /= base;
      }

   while (i < precision && i < 64)
      buffer[i++] = '0';

   len = i;
   if ((flags & DSTR_F_HASH) && base == 16)
      len += 2;

   pad = width - len;

   if (!(flags & DSTR_F_LEFT) && pad_char == ' ')
      {
         while (pad-- > 0)
            dstr_putc (d, ' ');
      }

   if ((flags & DSTR_F_HASH) && base == 16)
      {
         dstr_putc (d, '0');
         dstr_putc (d, uppercase ? 'X' : 'x');
      }

   if (!(flags & DSTR_F_LEFT) && pad_char == '0')
      {
         while (pad-- > 0)
            dstr_putc (d, '0');
      }

   while (i > 0)
      dstr_putc (d, buffer[--i]);

   if (flags & DSTR_F_LEFT)
      {
         while (pad-- > 0)
            dstr_putc (d, ' ');
      }
}

static void
dstr_putd_internal (dstr_t *d, double n, int precision)
{
   long ipart;
   double fpart;
   int i;
   double rounding = 0.5;

   for (i = 0; i < precision; i++)
      rounding /= 10.0;
   n += rounding;

   ipart = (long)n;
   fpart = n - (double)ipart;

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
               fpart -= (int)((int)fpart % 10);
            }
      }
}

void
dstr_putd (dstr_t *d, double n, int precision)
{
   dstr_putd_internal (d, n, precision);
}

void
dstr_putl_ext (dstr_t *d, long n, int width, int precision, int flags)
{
   unsigned long ul;
   char sign     = 0;
   char pad_char = (flags & DSTR_F_ZERO && !(flags & DSTR_F_LEFT)) ? '0' : ' ';
   int pad;

   if (n < 0)
      {
         sign = '-';
         ul   = (unsigned long)-(n + 1) + 1;
      }
   else
      {
         if (flags & DSTR_F_PLUS)
            sign = '+';
         else if (flags & DSTR_F_SPACE)
            sign = ' ';
         ul = (unsigned long)n;
      }

   pad = width - (sign ? 1 : 0);

   if (sign && pad_char == '0')
      {
         dstr_putc (d, sign);
         dstr_putul_internal (d, ul, 10, 0, pad, precision, flags);
      }

   else if (sign)
      {
         char buf_temp[64];
         dstr_t scratch;
         scratch.str = buf_temp;
         scratch.len = 0;
         scratch.cap = 64;

         dstr_putul_internal (&scratch, ul, 10, 0, 0, precision, flags);

         pad = width - (int)scratch.len - 1;
         if (!(flags & DSTR_F_LEFT))
            {
               while (pad-- > 0)
                  dstr_putc (d, ' ');
            }
         dstr_putc (d, sign);
         {
            int i;
            for (i = 0; i < (int)scratch.len; i++)
               dstr_putc (d, scratch.str[i]);
         }
         if (flags & DSTR_F_LEFT)
            {
               while (pad-- > 0)
                  dstr_putc (d, ' ');
            }
      }
   else
      {
         dstr_putul_internal (d, ul, 10, 0, width, precision, flags);
      }
}

void
dstr_catd_ext (dstr_t *d, const char *str, int width, int precision, int flags)
{
   int s_len = (int)strlen (str);
   int pad;

   if (precision >= 0 && precision < s_len)
      s_len = precision;

   pad = width - s_len;

   if (!(flags & DSTR_F_LEFT))
      {
         while (pad-- > 0)
            dstr_putc (d, ' ');
      }

   {
      int i;
      for (i = 0; i < s_len; i++)
         dstr_putc (d, str[i]);
   }

   if (flags & DSTR_F_LEFT)
      {
         while (pad-- > 0)
            dstr_putc (d, ' ');
      }
}

void
dstr_putd_ext (dstr_t *d, double n, int width, int precision, int flags)
{
   char buf[128];
   dstr_t scratch;
   int pad, i;
   char pad_char = (flags & DSTR_F_ZERO && !(flags & DSTR_F_LEFT)) ? '0' : ' ';
   char sign     = 0;

   scratch.str = buf;
   scratch.len = 0;
   scratch.cap = 128;

   if (n < 0)
      {
         sign = '-';
         n    = -n;
      }
   else if (flags & DSTR_F_PLUS)
      {
         sign = '+';
      }
   else if (flags & DSTR_F_SPACE)
      {
         sign = ' ';
      }

   dstr_putd_internal (&scratch, n, precision);

   pad = width - (int)scratch.len - (sign ? 1 : 0);

   if (!(flags & DSTR_F_LEFT) && pad_char == ' ')
      {
         while (pad-- > 0)
            dstr_putc (d, ' ');
      }

   if (sign)
      dstr_putc (d, sign);

   if (!(flags & DSTR_F_LEFT) && pad_char == '0')
      {
         while (pad-- > 0)
            dstr_putc (d, '0');
      }

   for (i = 0; i < (int)scratch.len; i++)
      {
         dstr_putc (d, scratch.str[i]);
      }

   if (flags & DSTR_F_LEFT)
      {
         while (pad-- > 0)
            dstr_putc (d, ' ');
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
