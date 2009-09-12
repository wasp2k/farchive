#ifndef FARCHIVE_H
#define FARCHIVE_H

#include "ferror.h"
#include "ffile.h"
#include "fobject.h"
#include "fmem.h"

class farchive : public ferror
{
private:
   /**
     * Archive version structure
     */
   struct VERSION
   {
      char pattern[5];
      char dummy;
      unsigned short version;
   };

   /**
     * Archive header structure
     */
   struct HEADER
   {
      unsigned int lastID;
   };

   static const VERSION m_version;
#if 0
   fobjectT<HEADER> m_header;
#endif

protected:
   ffile m_file;
   fobject m_currObj;

   virtual int createFileHeader(void);

public:
   farchive();
   virtual ~farchive();

   virtual int create(const char *fileName);
   virtual int open(const char *fileName);
   virtual int close(void);

   virtual int isOpen(void);

   virtual int moveFirst(void);
   virtual int moveNext(void);

   virtual int moveTo(const unsigned int objId);

   virtual int read(fobject &obj);
   virtual int write(fobject &obj);

   virtual int read(fmem &obj);
   virtual int write(fmem &obj);

   virtual int add(fobject &obj);
   virtual int remove(const unsigned int objId);
};

#endif // FARCHIVE_H
