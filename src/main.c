#include <stdio.h>

#include "dstr.h"

int
main (void)
{
   dstr_t dstr = dstr_new ();
   
   dstr_catd (&dstr, "hello,");
   dstr_putc (&dstr, ' ');
   dstr_catd (&dstr, "world!");

   printf ("before clear: str -> %s, len -> %zu\n", dstr.str, dstr.len);

   dstr_clear (&dstr);

   dstr_catd (&dstr, "hello again!");
   printf ("after clear: str -> %s, len -> %zu\n", dstr.str, dstr.len);

   dstr_t dcpy = dstr_cpy (&dstr);
   dstr_free (&dstr);

   printf("copied: str -> %s, len -> %zu\n", dcpy.str, dcpy.len);
   
   dstr_free(&dcpy);
}
