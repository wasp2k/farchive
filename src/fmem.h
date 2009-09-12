#ifndef FMEM_H
#define FMEM_H

#include "ferror.h"

/* ------------------------------------------------------------------------- */

class fmem : public ferror
{
   int m_objId;

   int m_lockCnt;

   char *m_buf;
   int m_bufSize;

   int *m_lenPtr;

protected:
   inline void setObjId(int objId){ m_objId = objId; }
public:
   inline int getObjId(void){ return m_objId; }

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

   friend class farchive;
};

/* ------------------------------------------------------------------------- */

#endif // FMEM_H
