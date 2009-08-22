#include <stdio.h>
#include <stdarg.h>
#include "fdbg.h"

#define FCDBG_FILE   "fc.log"

/* ------------------------------------------------------------------------- */

static FDBG_LEVELS g_fcDbgLvl = FLVL_DBG;

/* ------------------------------------------------------------------------- */

void fdbgSetLevel(const FDBG_LEVELS lvl)
{
   g_fcDbgLvl = lvl;
}

/* ------------------------------------------------------------------------- */

static void fdbgMessage(const char *tag, const char *func, const char *format, va_list args )
{
   FILE *f;
   f = fopen(FCDBG_FILE, "at+" );
   fputs( tag, f );
   fputs( func, f );
   fputs( ":", f );
   vfprintf (f, format, args);
   fputs( "\n", f );
   fclose(f);
}

/* ------------------------------------------------------------------------- */

void fdbgErr(const char *func,const char *format, ... )
{
   if ( g_fcDbgLvl <= FLVL_ERR )
   {
      va_list args;
      va_start (args, format);
      fdbgMessage("ERR:", func, format, args);
      va_end (args);
   }
}

/* ------------------------------------------------------------------------- */

void fdbgWarn(const char *func,const char *format, ... )
{
   if ( g_fcDbgLvl <= FLVL_WARN )
   {
      va_list args;
      va_start (args, format);
      fdbgMessage("WRN:", func, format, args);
      va_end (args);
   }
}

/* ------------------------------------------------------------------------- */

void fdbgInfo(const char *func,const char *format, ... )
{
   if ( g_fcDbgLvl <= FLVL_INFO )
   {
      va_list args;
      va_start (args, format);
      fdbgMessage("INF:", func, format, args);
      va_end (args);
   }
}

/* ------------------------------------------------------------------------- */

void fdbgDbg(const char *func,const char *format, ... )
{
   if ( g_fcDbgLvl <= FLVL_DBG )
   {
      va_list args;
      va_start (args, format);
      fdbgMessage("DBG:", func, format, args);
      va_end (args);
   }
}

/* ------------------------------------------------------------------------- */
