#ifndef FMEM_H
#define FMEM_H

#include "farchive.h"

/* ------------------------------------------------------------------------- */

class fmem : public ferror
{
    int m_lockCnt;

    void *m_dataPtr;
    int *m_lenPtr;


    int realloc(int size);

    farchive &m_arch;

public:
    fmem(farchive &arch);
    virtual ~fmem();

    void *map(void);
    void unmap(void);

    int setSize(int size);
    int getSize(void);

    int free();
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
