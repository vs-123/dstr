#include <stdio.h>

#define DSTR_IMPL
#include "dstr.h"

int
main (void)
{
   dstr_t dstr, dcpy;
   double pi  = 3.1415926535;
   long debt  = -2147483647;
   char *name = "vs-123";

   dstr = dstr_new ();

   dstr_catd (&dstr, "hello,");
   dstr_putc (&dstr, ' ');
   dstr_catd (&dstr, "world!");

   printf ("before clear: str -> %s, len -> %zu\n", dstr.str, dstr.len);

   dstr_clear (&dstr);

   dstr_catd (&dstr, "hello again!");
   printf ("after clear: str -> %s, len -> %zu\n", dstr.str, dstr.len);

   dcpy = dstr_cpy (&dstr);
   dstr_free (&dstr);

   printf ("copied: str -> %s, len -> %zu\n", dcpy.str, dcpy.len);

   dstr_toupperd (&dcpy);

   printf ("upper: str -> %s\n", dcpy.str);

   dstr_clear (&dcpy);

   dstr_catfmtd (
       &dcpy,
       "User: %-15s | Balance: %+.10ld | Constant: %010.*f | %% complete",
       name, debt, 4, pi);

   printf ("catfmtd: str -> %s, len -> %zu\n", dcpy.str, dcpy.len);

   dstr_free (&dcpy);
}
