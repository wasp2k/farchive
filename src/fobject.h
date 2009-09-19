#ifndef FOBJECT_H
#define FOBJECT_H

#include "ferror.h"
#include "ffile.h"

#define FOBJECT_INCLUDE_DEBUG_PATTEN  1

class fobject : public ferror
{
public:
   enum TYPE
   {
      TYPE_UNKNOWN = 0,
      TYPE_HEADER,
      TYPE_INDEX,
      TYPE_OBJECT
   };

   enum OPTIONS
   {
      OPT_FRAGMENT = 0x0001,
   };

private:
   int m_ofs;

   struct OBJECT
   {
      #if FOBJECT_INCLUDE_DEBUG_PATTEN
      unsigned int pattern;
      #endif
      unsigned int id;              /* Object ID */
      unsigned int size;            /* Number of bytes used from this object */
      unsigned int payloadSize;     /* Object size */
      unsigned char type;           /* Object type */
      unsigned char dummy;          /* Alignment byte */
      unsigned short options;       /* Options */
      unsigned int nextChain;       /* Id of the next object */
   };

   OBJECT m_obj;

protected:
   inline void setOfs(int ofs)                           { m_ofs = ofs; }
   inline void setId(unsigned int id)                    { m_obj.id = id; }
   inline void setSize(unsigned int size)                { m_obj.size = size; }
   inline void setPayloadSize(unsigned int payloadSize)  { m_obj.payloadSize = payloadSize; }
          void setOptions(unsigned short options, unsigned short mask);
   inline void setNextChain(unsigned int nextChain)      { m_obj.nextChain = nextChain; }
   inline void setType(TYPE type)                        { m_obj.type = (unsigned char)( type & 0xFF ); }
public:
   inline          int getOfs() const                    { return m_ofs; }
   inline unsigned int getId() const                     { return m_obj.id; }
   inline unsigned short getOptions() const                { return m_obj.options; }
   inline unsigned int getNextChain() const              { return m_obj.nextChain; }
   inline unsigned int getSize() const                   { return m_obj.size; }
   inline unsigned int getPayloadSize() const            { return m_obj.payloadSize; }
   inline TYPE         getType() const                   { return (TYPE)m_obj.type; }

public:
   fobject(void);
   virtual ~fobject();

   int readHeader(ffile &file);
   int readPayload(ffile &file, void *buf);
   int writeHeader(ffile &file);
   int writePayload(ffile &file, void *buf);

   inline unsigned int getObjectSize(){ return m_obj.payloadSize + sizeof( OBJECT ); }

   fobject &operator=(const fobject &fobj);

   friend class farchive;
   friend class farchiveidx;
   friend class fmem;
};

#endif // FCOBJECT_H
