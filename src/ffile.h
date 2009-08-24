#ifndef FFILE_H
#define FFILE_H

#include <stdio.h>

#include "fdbg.h"
#include "ferror.h"

class ffile : public ferror
{
public:
   enum ORIGIN
   {
      SET   = SEEK_SET,
      CUR   = SEEK_CUR,
      END   = SEEK_END
   };

private:
   FILE *m_file;

public:
    ffile();
    virtual ~ffile();

    int open(const char *fileName);
    int create(const char *fileName);
    int close(void);

    int write(const void *buf, const int size);
    int read(void *buf, const int size);

    int seek(const int ofs, const ORIGIN origin); /* Origin: SEEK_SET, SEEK_CUR, SEEK_END */
    int tell(void);

    inline bool isOpen(void){ return m_file != NULL; }

    int isEOF(void);
};

#endif // FFILE_H
