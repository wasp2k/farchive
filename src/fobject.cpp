#include <string.h>
#include <stdlib.h>
#include "fobject.h"

#if FOBJECT_INCLUDE_DEBUG_PATTEN
   #define FOBJECT_PATTERN      0x3a4a424f
#endif

/* ------------------------------------------------------------------------- */

fobject::fobject(void)
{
   m_ofs = -1;
   m_obj.id = -1;
   m_obj.size = 0;
   m_obj.payloadSize = 0;
   m_obj.options = 0;
   m_obj.nextChain = 0;
   m_obj.type = TYPE_UNKNOWN;
}

/* ------------------------------------------------------------------------- */

fobject::~fobject()
{
}

/* ------------------------------------------------------------------------- */

int fobject::readHeader(ffile &file)
{
   OBJECT obj;

   setLastError(UNDEFINED);

   file.seek(m_ofs,ffile::SET);                                /* Seek to the offset */
   if ( file.getStatus() == -1 )
   {
      setLastError(file);
   } else
   {
      file.read(&obj, sizeof(obj));                               /* Read object header */
      if ( file.getStatus() == -1 )
      {
         setLastError(file);
      } else
      {
         FDBG3("FCObject::readHeader id: %d ofs: %d siz: %d", obj.id, m_ofs, obj.size );

         #if FOBJECT_INCLUDE_DEBUG_PATTEN
         if ( obj.pattern != FOBJECT_PATTERN )
         {
            setLastError(BAD_OBJECT_PATTERN);
         } else
         #endif
         {
            m_obj = obj;
            setLastError(NO_ERROR);
         }
      }
   }

   FDBG2( "Read header ofs:%d %d", m_ofs, getLastError() );

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fobject::readPayload(ffile &file, void *buf)
{
   OBJECT obj;

   if ( file.seek(m_ofs,ffile::SET) == -1)                             /* Seek to the offset */
   {
      setLastError(file);
   } else
   {
      if ( file.read(&obj, sizeof(obj)) == -1 )                                     /* Read object header */
      {
         setLastError(file);
      } else
      {
         #if FOBJECT_INCLUDE_DEBUG_PATTEN
         if ( obj.pattern != FOBJECT_PATTERN )
         {
            setLastError(BAD_OBJECT_PATTERN);
         } else
         #endif
         if ( buf == NULL )
         {
            setLastError(BAD_PARAMETER);
         } else
         {
            if ( file.read(buf, obj.size) == -1 )          /* Read object payload */
            {
               setLastError(file);
            } else
            {
               m_obj = obj;
               setLastError(NO_ERROR);
            }
         }
      }
   }

   FDBG3( "Read ofs:%d ptr: %p %d", m_ofs, buf, getLastError() );

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fobject::writePayload(ffile &file, void *buf)
{
   setLastError(UNDEFINED);

   if (!file.isOpen())
   {
      setLastError(BAD_FILE_OBJECT);
   } else
   {
      if (m_ofs == -1)
      {
         m_ofs = file.seek(0, ffile::END );        /* Move to the end of the file */
         if ( m_ofs == -1 )
         {
            setLastError(file);
         } else
         {
            m_obj.payloadSize = m_obj.size;
            FDBG2( "Append object %d at: %d", m_obj.id, m_ofs );
            setLastError(NO_ERROR);
         }
      } else
      {
         if ( file.seek( m_ofs, ffile::SET ) == -1 ) /* Move to the object offset */
         {
            setLastError(file);
         } else
         {
            setLastError(NO_ERROR);
         }
      }

      if (getStatus()!=-1)
      {
         #if FOBJECT_INCLUDE_DEBUG_PATTEN
         m_obj.pattern = FOBJECT_PATTERN;
         #endif

         if ( file.write( &m_obj, sizeof( m_obj ) ) == -1 )    /* write object header */
         {
            setLastError(file);
         } else
         {
            if (buf == NULL)
            {
                setLastError(BAD_PARAMETER);
            } else
            {
               if ( m_obj.size > 0 )
               {
                  if ( file.write( buf, m_obj.size ) == -1 )            /* write object payload */
                  {
                     setLastError(file);
                  } else
                  {
                     setLastError(NO_ERROR);
                  }
               } else
               {
                  setLastError(NO_ERROR);
               }
            }
         }
      }
   }

   FDBG2( "Flush ofs:%d %d", m_ofs, getLastError() );

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fobject::writeHeader(ffile &file)
{
   setLastError(UNDEFINED);

   if (!file.isOpen())
   {
      setLastError(BAD_FILE_OBJECT);
   } else
   {
      if (m_ofs == -1)
      {
         setLastError(BAD_OFFSET);
      } else
      {
         if ( file.seek( m_ofs, ffile::SET ) == -1 ) /* Move to the object offset */
         {
            setLastError(file);
         } else
         {
            setLastError(NO_ERROR);
         }
      }

      if (getStatus()!=-1)
      {
         #if FOBJECT_INCLUDE_DEBUG_PATTEN
         m_obj.pattern = FOBJECT_PATTERN;
         #endif

         if ( file.write( &m_obj, sizeof( m_obj ) ) == -1 )    /* write object header */
         {
            setLastError(file);
         } else
         {
            setLastError(NO_ERROR);
         }
      }
   }

   FDBG2( "Flush ofs:%d %d", m_ofs, getLastError() );
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void fobject::setOptions(unsigned short options, unsigned short mask)
{
   m_obj.options &= ~mask;
   m_obj.options |= options & mask;
}

/* ------------------------------------------------------------------------- */

fobject &fobject::operator=(const fobject &fobj)
{
   this->m_ofs = fobj.m_ofs;
   this->m_obj = fobj.m_obj;
   return *this;
}

/* ------------------------------------------------------------------------- */
