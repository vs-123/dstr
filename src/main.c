#include <stdio.h>

#include "dstr.h"

int
main (void)
{
   dstr_t dstr = dstr_new (32);
   dstr_catd (&dstr, "hello,");
   dstr_putc (&dstr, ' ');
   dstr_catd (&dstr, "world!");
   dstr_putc (&dstr, '\n');
   
   printf ("%s", dstr.str);
   dstr_free(&dstr);
}
