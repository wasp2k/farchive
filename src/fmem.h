#ifndef FMEM_H
#define FMEM_H

#include "ferror.h"
#include "fobject.h"

/* ------------------------------------------------------------------------- */

class fmem : public ferror
{
   int m_lockCnt;

   char *m_buf;
   int m_bufSize;
   int *m_lenPtr;

protected:
   fobject *m_objList;
   int m_objCnt;
   int m_objAllocated;

   inline int getObjCnt(void){ return m_objCnt; }
   inline fobject &getObj(int idx){ return m_objList[idx]; }

   int addObj(const fobject &obj);
   int growObjList(void);
   int freeObjList(void);

   inline char *getBuf(void){ return m_buf; }
   inline int getBufSize(void){ return *m_lenPtr + sizeof( int ); }

public:
   fmem();
   virtual ~fmem();

   void *map(void);
   void unmap(void);

   int alloc(int size);
   int realloc(int size);
   int free();
   int append(void *ptr,int size);

   int getSize(void);

   int zero(void);

   friend class farchive;
};

/* ------------------------------------------------------------------------- */

template<class TYPE>
class fmemT : public fmem
{
   TYPE *m_ptr;
public:
   fmemT(void)
   {
      alloc(sizeof(TYPE));
      m_ptr = (TYPE*)map();
   }
   virtual ~fmemT(void)
   {
      unmap();
   }
   inline TYPE *operator->(){ return m_ptr; }
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
