#ifndef FARCHIVE_H
#define FARCHIVE_H

#include "ffile.h"
#include "fobject.h"

class farchive
{
private:
   struct VERSION
   {
      char pattern[6];
      unsigned short version;
   };

   struct HEADER
   {
      unsigned int lastID;
   };

   static const VERSION m_version;
   fobject m_header;

protected:
   ffile m_file;
   fobject m_currObj;

   virtual void createFileHeader(void);

   void moveToOfs(long int ofs);

public:
   farchive();
   virtual ~farchive();

   virtual void create(const char *fileName);
   virtual void open(const char *fileName);
   virtual void close(void);

   virtual void moveFirst(void);
   virtual void moveNext(void);

   virtual bool moveToObject(const unsigned int objId);

   virtual void getObject(fobject &obj);

   virtual bool isEOF(void);

   virtual void add(fobject &obj);

   virtual bool remove(const unsigned int objId);
};

#endif // FARCHIVE_H
