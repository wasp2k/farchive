#ifndef FARCHIVEIDX_H
#define FARCHIVEIDX_H

#include "farchive.h"

#define FINDEX_SIZE    512

class farchiveidx : public farchive
{
   struct ITEM                            /* Index item */
   {
      unsigned int id;
      int ofs;
      unsigned int options;
   };

   struct INDEX                           /* Index structure */
   {
      ITEM   items[ FINDEX_SIZE + 1 ];
      long int nextObjId;
   };

   struct DATA
   {
      fobjectT<INDEX> obj;
      DATA *nextData;
   };

   DATA *m_firstData;

   int findObject(const unsigned int objId, DATA *(&data), int &dataIdx );
   int registerIndex(const fobject &obj);

   int readIndex(void);

   void freeIndex(void);

protected:
   virtual int createFileHeader(void);

private:
   int moveToOfs(int ofs);

public:
    farchiveidx();
    virtual ~farchiveidx();

   virtual int open(const char *fileName);
   virtual int close(void);

   virtual int moveTo(const unsigned int objId);

   virtual int add(fobject &obj);

   virtual int remove(const unsigned int objId);
};

#endif // FARCHIVEIDX_H
