# DSTR 

dstr is a dead-simple, C89-compatible, single-header dynamic string library. dstr stands for `dynamic string`.

# FEATURES 
- Single-header library, zero installation required. Just clone and `#include`, it's that easy.

- Dead-simple API. 
   
- 100% ANSI C compliant, zero GNU extensions were used. This is achieved with strict compiler flags (see `Makefile`).
   
- Minimal dependencies, relies only on core C89 headers i.e. `<stddef.h>`, `<stdlib.h>` & `<string.h>`

- Supports printf-style formatters, like `%s`, `%010.*f`, `%i`, etc., written manually from scratch.
       
- Even a C compiler from 1990 would happily compile this without a single warning.


# HOW TO USE 
- Clone this repository into your project.

```
   %  git clone https://github.com/vs-123/dstr.git
```
	  
- Include this project to your build system to access `dstr.h` within your code.
   
- In your `main.c` (or any main file), add the following at the top BEFORE you include `dstr.h`: **(IMPORTANT)**
       
```c
   #define DSTR_IMPL
```

**[NOTE]** the above `#define` directive should be used only **ONCE** in your main C file.

- Finally, use `#include "dstr.h"` wherever you'd like.

# EXAMPLE 
```c
#define DSTR_IMPL
#include "dstr.h"

dstr_t dstr, dcpy;
double pi = 3.1415926535;
long debt = -2147483647;
char *name = "vs-123";

dstr = dstr_new ();

dstr_catd (&dstr, "hello,");
dstr_putc (&dstr, ' ');
dstr_catd (&dstr, "world!");

/* before clear: dstr.str -> hello, world!, dstr.len -> 13 */

dstr_clear (&dstr);
dstr_catd (&dstr, "hello again!");

/* after clear: dstr.str -> hello again!, dstr.len -> 12 */

dcpy ` dstr_cpy (&dstr);
dstr_free (&dstr);

/* copied: dcpy.str -> hello again!, dcpy.len -> 12 */

dstr_toupperd (&dcpy);

/* upper: dcpy.str -> HELLO AGAIN! */

dstr_clear (&dcpy);

dstr_catfmtd (&dcpy, "User: %-15s | Balance: %+.10ld | Constant: %010.*f | %% complete", name, debt, 4, pi);

/* User: vs-123          | Balance: -2147483647 | Constant: 00003.1416 | % complete */
    
dstr_free(&dcpy);
```

# LICENSE 

This project is licensed under the GNU Affero General Public License Version 3.0 or later.

**NO WARRANTY IS PROVIDED.**

For full terms, see `LICENSE` file or visit **https://www.gnu.org/licenses/agpl-3.0.en.html**.
