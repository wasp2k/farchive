#ifndef FARCHIVE_H
#define FARCHIVE_H

#include "ferror.h"
#include "ffile.h"
#include "fobject.h"

class farchive : ferror
{
private:
   struct VERSION
   {
      char pattern[5];
      char dummy;
      unsigned short version;
   };

   struct HEADER
   {
      unsigned int lastID;
   };

   static const VERSION m_version;
   fobjectT<HEADER> m_header;

protected:
   ffile m_file;
   fobject m_currObj;

   virtual int createFileHeader(void);

   void moveToOfs(long int ofs);

public:
   farchive();
   virtual ~farchive();

   virtual int create(const char *fileName);
   virtual int open(const char *fileName);
   virtual int close(void);

   virtual int moveFirst(void);
   virtual int moveNext(void);

   virtual bool moveToObject(const unsigned int objId);

   virtual int getObject(fobject &obj);

   virtual int add(fobject &obj);

   virtual bool remove(const unsigned int objId);
};

#endif // FARCHIVE_H
