#ifndef FARCHIVEIDX_H
#define FARCHIVEIDX_H

#include "farchive.h"
#include "fmem.h"

class farchiveidx : public farchive
{
   struct ITEM                            /* Index item */
   {
      unsigned int id;
      int ofs;
      unsigned int options;
   };

   fmem m_index;

   int findObject(const unsigned int objId, int &pos );
   int addObject(const fobject &obj);
   int registerIndex(const fmem &mem);

   int readIndex(void);

protected:
   virtual int createArchiveHeader(void);

private:
   int moveToOfs(int ofs);

public:
    farchiveidx();
    virtual ~farchiveidx();

   virtual int open(const char *fileName);
   virtual int close(void);

   virtual int moveTo(const unsigned int objId);

   virtual int add(fmem &mem);
   virtual int write(fmem &mem);

   virtual int remove(const unsigned int objId);
};

#endif // FARCHIVEIDX_H
