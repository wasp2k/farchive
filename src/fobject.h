#ifndef FOBJECT_H
#define FOBJECT_H

#include "ferror.h"
#include "ffile.h"

#define FOBJECT_INCLUDE_DEBUG_PATTEN  1

class fobject : public ferror
{
private:
   int m_ofs;

   struct OBJECT
   {
      #if FOBJECT_INCLUDE_DEBUG_PATTEN
      unsigned int pattern;
      #endif
      unsigned int id;
      unsigned int size;
      unsigned int options;
   };

   OBJECT m_obj;
   char *m_data;
   int m_dataSize;

   int allocPayload(int size);
   void freePayload(void);

public:
   enum
   {
      REMOVED  = 0x00000001
   };

public:
   fobject(long int ofs = -1, unsigned int size = 0);
   virtual ~fobject();

   int readHeader(ffile &file);
   int read(ffile &file);
   int flush(ffile &file);

   inline unsigned int getOfs() const{return m_ofs; }
   inline void setOfs(long int ofs){ m_ofs = ofs; }

   inline unsigned int getId() const {return m_obj.id; }
   void setId(unsigned int id);

   inline unsigned int getOptions() const {return m_obj.options; }
   void setOptions(unsigned int options);

   inline unsigned int getSize(){return m_obj.size; }
   inline unsigned int getBlockSize(){ return m_obj.size + sizeof( OBJECT ); }

   inline void *getPtr(){ return m_data; }

   void zero();
};

template<class TYPE>
class fobjectT : public fobject
{
public:
   fobjectT(void) : fobject( -1, sizeof( TYPE ) ){}
   fobjectT(long int offset) : fobject( offset, sizeof( TYPE ) ){}

   inline TYPE *operator->(){ return (TYPE*)getPtr(); }
};


#endif // FCOBJECT_H
