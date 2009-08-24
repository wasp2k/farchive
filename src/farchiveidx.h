#ifndef FARCHIVEIDX_H
#define FARCHIVEIDX_H

#include "farchive.h"

#define FINDEX_SIZE    512

class farchiveidx : public farchive
{
   struct ITEM
   {
      unsigned long id;
      long int ofs;
      unsigned long options;
   };

   struct INDEX
   {
      ITEM   records[ FINDEX_SIZE + 1 ];
      long int nextObjId;
   };

   struct DATA
   {
      fobjectT<INDEX> obj;
      DATA *nextData;
   };

   DATA *m_firstData;

   bool findObject(const unsigned int objId, DATA *(&data), int &dataIdx );
   void registerIndex(const fobject &obj);

   void readIndex(void);

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
};

#endif // FARCHIVEIDX_H
