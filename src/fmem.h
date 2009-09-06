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

public:
    fmem();
    virtual ~fmem();


    int setSize(int size);

    int free();
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
