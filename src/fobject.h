#ifndef FOBJECT_H
#define FOBJECT_H

#include "ffile.h"

#define FCOBJECT_INCLUDE_PATTEN  1

class fobject
{
private:
   long int m_ofs;

   struct OBJECT
   {
      #if FCOBJECT_INCLUDE_PATTEN
      unsigned int pattern;
      #endif
      unsigned int id;
      unsigned int size;
      unsigned int options;
   };

   OBJECT m_obj;
   char *m_data;
   unsigned int m_dataSize;
   bool m_dirty;
   bool m_onlyHeader;

public:
   enum
   {
      REMOVED  = 0x00000001
   };

public:
   fobject(long int ofs = -1, unsigned int size = 0);
   virtual ~fobject();

   void readHeader(ffile &file);
   void read(ffile &file);
   void flush(ffile &file);

   inline unsigned int getOfs() const{return m_ofs; }
   inline void setOfs(long int ofs){ m_ofs = ofs; }

   inline unsigned int getId() const {return m_obj.id; }
   void setId(unsigned int id);

   inline unsigned int getOptions() const {return m_obj.options; }
   void setOptions(unsigned int options);

   inline unsigned int getSize(){return m_obj.size; }
   inline unsigned int getBlockSize(){ return m_obj.size + sizeof( OBJECT ); }

   inline void *getPtr(){ return m_data; }

   inline bool isDirty(){ return m_dirty; }
   inline void setDirty(){ m_dirty = true; }

   void zero();

   template<class X>
         inline operator X(){ return (X)( (char*)m_data ); }

#if 0
   inline OBJECT *operator->(){ return (OBJECT*)m_raw; }
#endif


#if 0
   template<class X>
         inline X getPtr(){ return NULL; }
#endif
};

#endif // FCOBJECT_H
