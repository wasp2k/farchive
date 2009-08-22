#ifndef FDBG_H
#define FDBG_H

enum FDBG_LEVELS
{
   FLVL_OFF = 0,
   FLVL_ERR,
   FLVL_WARN,
   FLVL_INFO,
   FLVL_DBG
};

void fdbgErr(const char *format, ... );
void fdbgWarn(const char *format, ... );
void fdbgInfo(const char *format, ... );
void fdbgDbg(const char *format, ... );

void fdbgSetLevel(const FDBG_LEVELS lvl);

#define FDBG      fdbgDbg
#define FINFO     fdbgInfo
#define FWARN     fdbgWarn
#define FERR      fdbgErr


#endif // FCDBG_H
