#include <stdio.h>
#include <stdarg.h>
#include "fdbg.h"

#define FCDBG_FILE   "fc.log"

static int g_fcDbgLvl = 7;

void fdbg(int lvl, const char *format, ... )
{
   if ( lvl <= g_fcDbgLvl )
   {
      FILE *f;
      va_list args;
      va_start (args, format);

      f = fopen(FCDBG_FILE, "at+" );
      vfprintf (f, format, args);
      fputs( "\n", f );
      fclose(f);
      va_end (args);
   }
}
