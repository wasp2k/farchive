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

void fdbgErr(const char *func, const char *format, ... );
void fdbgWarn(const char *func,const char *format, ... );
void fdbgInfo(const char *func,const char *format, ... );
void fdbgDbg(const char *func,const char *format, ... );

void fdbgSetLevel(const FDBG_LEVELS lvl);

#define FDBG(FORMAT, ...)      /*fdbgDbg(__FUNCTION__, FORMAT, __VA_ARGS__ )*/
#define FINFO(FORMAT, ...)     /*fdbgInfo(__FUNCTION__,FORMAT, __VA_ARGS__ )*/
#define FWARN(FORMAT, ...)     /*fdbgWarn(__FUNCTION__,FORMAT, __VA_ARGS__ )*/
#define FERR(FORMAT, ...)      /*fdbgErr(__FUNCTION__,FORMAT, __VA_ARGS__ )*/


#endif // FCDBG_H
