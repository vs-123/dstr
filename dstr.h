/* dstr.h
   Copyright (C) 2025 vs-123
   Licensed under the GNU Affero General Public License v3.0 or later.
   No warranty is provided.
   Full license text is located at the end of this file and in the LICENSE file.

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

#include <stddef.h>

typedef struct
{
   char *str;
   size_t len;
} dstr_t;

dstr_t dstr_new (void);
void dstr_free (dstr_t *);

void dstr_clear(dstr_t *); /* destructive */
dstr_t dstr_cpy (const dstr_t *d);

void dstr_catd (dstr_t *, const char *); /* destructive */
dstr_t dstr_cat (const dstr_t *, const char *);

void dstr_putc (dstr_t *, char); /* destructive */

#endif /* __DSTR_H */

/**********/

#ifdef DSTR_IMPL

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

#endif // DSTR_IMPL

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
