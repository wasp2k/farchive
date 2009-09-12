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

protected:
   inline void setOfs(int ofs)                           { m_ofs = ofs; }
   inline void setId(unsigned int id)                    { m_obj.id = id; }
   inline void setOptions(unsigned int options)          { m_obj.options = options; }
   inline void setNextChain(unsigned int nextChain)      { m_obj.nextChain = nextChain; }
public:
   inline          int getOfs() const                    { return m_ofs; }
   inline unsigned int getId() const                     { return m_obj.id; }
   inline unsigned int getOptions() const                { return m_obj.options; }
   inline unsigned int getNextChain() const              { return m_obj.nextChain; }
   inline unsigned int getSize() const                   { return m_obj.size; }

public:
   fobject(void);
   virtual ~fobject();

   int readHeader(ffile &file);
   int readPayload(ffile &file, void *buf);
   int writePayload(ffile &file, void *buf);

   inline unsigned int getObjectSize(){ return m_obj.size + sizeof( OBJECT ); }

   fobject &operator=(const fobject &fobj);

   friend class farchive;
};

#endif // FCOBJECT_H
