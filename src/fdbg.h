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

#define FDBG0(FORMAT)      fdbgDbg(__FILE__, __LINE__, FORMAT )
#define FDBG1(FORMAT, A)      fdbgDbg(__FILE__, __LINE__, FORMAT, A )
#define FDBG2(FORMAT, A, B)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B )
#define FDBG3(FORMAT, A, B, C)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C )
#define FDBG4(FORMAT, A, B, C, D)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C, D )
#define FDBG5(FORMAT, A, B, C, D, E)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C, D, E )
#define FDBG6(FORMAT, A, B, C, D, E, F)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F )
#define FDBG7(FORMAT, A, B, C, D, E, F, G)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G )
#define FDBG8(FORMAT, A, B, C, D, E, F, G, H)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H )
#define FDBG9(FORMAT, A, B, C, D, E, F, G, H, I)      fdbgDbg(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H, I )

#define FINFO0(FORMAT)      fdbgInfo(__FILE__, __LINE__, FORMAT )
#define FINFO1(FORMAT, A)      fdbgInfo(__FILE__, __LINE__, FORMAT, A )
#define FINFO2(FORMAT, A, B)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B )
#define FINFO3(FORMAT, A, B, C)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C )
#define FINFO4(FORMAT, A, B, C, D)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C, D )
#define FINFO5(FORMAT, A, B, C, D, E)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C, D, E )
#define FINFO6(FORMAT, A, B, C, D, E, F)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F )
#define FINFO7(FORMAT, A, B, C, D, E, F, G)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G )
#define FINFO8(FORMAT, A, B, C, D, E, F, G, H)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H )
#define FINFO9(FORMAT, A, B, C, D, E, F, G, H, I)      fdbgInfo(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H, I )

#define FWARN0(FORMAT)      fdbgWarn(__FILE__, __LINE__, FORMAT )
#define FWARN1(FORMAT, A)      fdbgWarn(__FILE__, __LINE__, FORMAT, A )
#define FWARN2(FORMAT, A, B)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B )
#define FWARN3(FORMAT, A, B, C)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C )
#define FWARN4(FORMAT, A, B, C, D)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C, D )
#define FWARN5(FORMAT, A, B, C, D, E)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C, D, E )
#define FWARN6(FORMAT, A, B, C, D, E, F)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F )
#define FWARN7(FORMAT, A, B, C, D, E, F, G)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G )
#define FWARN8(FORMAT, A, B, C, D, E, F, G, H)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H )
#define FWARN9(FORMAT, A, B, C, D, E, F, G, H, I)      fdbgWarn(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H, I )

#define FERR0(FORMAT)      fdbgErr(__FILE__, __LINE__, FORMAT )
#define FERR1(FORMAT, A)      fdbgErr(__FILE__, __LINE__, FORMAT, A )
#define FERR2(FORMAT, A, B)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B )
#define FERR3(FORMAT, A, B, C)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C )
#define FERR4(FORMAT, A, B, C, D)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C, D )
#define FERR5(FORMAT, A, B, C, D, E)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C, D, E )
#define FERR6(FORMAT, A, B, C, D, E, F)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F )
#define FERR7(FORMAT, A, B, C, D, E, F, G)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G )
#define FERR8(FORMAT, A, B, C, D, E, F, G, H)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H )
#define FERR9(FORMAT, A, B, C, D, E, F, G, H, I)      fdbgErr(__FILE__, __LINE__, FORMAT, A, B, C, D, E, F, G, H, I )


#endif // FCDBG_H
