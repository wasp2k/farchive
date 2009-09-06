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
      unsigned int nextChain;     /* Id of the next object */
   };

   OBJECT m_obj;
   char *m_data;
   int m_dataSize;

   int allocPayload(int size);
   void freePayload(void);

protected:
   inline void setOfs(long int ofs){ m_ofs = ofs; }
   void setId(unsigned int id);
   void setOptions(unsigned int options);
   void setNextChain(unsigned int nextChain);

public:
   fobject(unsigned int size = 0);
   virtual ~fobject();

   int readHeader(ffile &file);
   int read(ffile &file);
   int read(ffile &file, void *buf);
   int flush(ffile &file);
   int flush(ffile &file, void *buf);

   inline unsigned int getOfs() const{return m_ofs; }

   inline unsigned int getId() const {return m_obj.id; }

   inline unsigned int getOptions() const {return m_obj.options; }

   inline unsigned int getNextChain() const {return m_obj.nextChain; }

   inline unsigned int getSize(){return m_obj.size; }
   inline unsigned int getBlockSize(){ return m_obj.size + sizeof( OBJECT ); }

   inline void *getPtr(){ return m_data; }

   void zero();

   friend class farchive;
   friend class farchiveidx;
};

template<class TYPE>
class fobjectT : public fobject
{
public:
   fobjectT(void) : fobject( sizeof( TYPE ) ){}

   inline TYPE *operator->(){ return (TYPE*)getPtr(); }
};


#endif // FCOBJECT_H
