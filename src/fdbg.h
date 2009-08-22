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

void fdbgErr(const char *file,const int line,const char *format, ... );
void fdbgWarn(const char *file,const int line,const char *format, ... );
void fdbgInfo(const char *file,const int line,const char *format, ... );
void fdbgDbg(const char *file,const int line,const char *format, ... );

void fdbgSetLevel(const FDBG_LEVELS lvl);

#define FDBG(FORMAT, ...)      fdbgDbg(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__ )
#define FINFO(FORMAT, ...)     fdbgInfo(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__ )
#define FWARN(FORMAT, ...)     fdbgWarn(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__ )
#define FERR(FORMAT, ...)      fdbgErr(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__ )


#endif // FCDBG_H
