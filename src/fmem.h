#ifndef FMEM_H
#define FMEM_H

#include "fobject.h"

/* ------------------------------------------------------------------------- */

class fmem : public ferror
{
    int m_lockCnt;

    char *m_dataPtr;
    int *m_lenPtr;


    int realloc(int size);

private:
    fobject **m_objList;      /* Object list */
    int m_allocObjCnt;        /* Number of allocated objects */
    int m_objCnt;             /* Number of used objects */

    int growObjList(void);
    int freeObjList(void);
protected:
    int pushObject(fobject *pushObj);
    int commit(void);

public:
    fmem();
    virtual ~fmem();

    void *map(void);
    void unmap(void);

    int setSize(int size);
    int getSize(void);

    int free();

    friend class farchive;
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
