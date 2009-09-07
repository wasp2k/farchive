#ifndef FMEM_H
#define FMEM_H

#include "farchive.h"

/* ------------------------------------------------------------------------- */

class fmem : public ferror
{
    int m_lockCnt;

    char *m_dataPtr;
    int *m_lenPtr;

    fobject **m_objList;
    int m_objCnt;

    int growObjList(void);
    int freeObjList();

    int realloc(int size);

public:
    fmem();
    virtual ~fmem();

    void *map(void);
    void unmap(void);

    int setSize(int size);
    int getSize(void);

    int write(farchive &arch);

    int free();
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
