#ifndef FFILE_H
#define FFILE_H

#include <stdio.h>

#include "ferror.h"
#include "fdbg.h"

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

    void write(const void *buf, const size_t size);
    void read(void *buf, const size_t size);

    long int seek(const long int ofs, const ORIGIN origin); /* Origin: SEEK_SET, SEEK_CUR, SEEK_END */
    long int tell(void);

    inline bool isOpen(void){ return m_file != NULL; }

    long int getSize(void);
};

#endif // FFILE_H
