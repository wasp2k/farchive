#ifndef FMEM_H
#define FMEM_H

#include "fobject.h"

/* ------------------------------------------------------------------------- */

class fmem : public ferror
{
    int m_lockCnt;

    char *m_buf;
    int m_bufSize;

    int *m_lenPtr;
public:
    fmem();
    virtual ~fmem();

    void *map(void);
    void unmap(void);

    int alloc(int size);
    int realloc(int size);
    void free();

    int append(void *ptr,int size);

    int getSize(void);

    friend class farchive;
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
