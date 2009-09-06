#include <string.h>
#include <stdlib.h>
#include "fobject.h"

#if FOBJECT_INCLUDE_DEBUG_PATTEN
   #define FOBJECT_PATTERN      0x3a4a424f
#endif

/* ------------------------------------------------------------------------- */

fobject::fobject(unsigned int size)
{
   m_data = NULL;
   m_dataSize = 0;

   allocPayload(size);

   m_ofs = -1;
   m_obj.id = 0;
   m_obj.size = size;
   m_obj.options = 0;
   m_obj.nextChain = 0;
}

/* ------------------------------------------------------------------------- */

fobject::~fobject()
{
   freePayload();
}

/* ------------------------------------------------------------------------- */

int fobject::allocPayload(int size)
{
   setLastError(NO_ERROR);

   if ( m_dataSize != size )
   {
      freePayload();

      if ( size > 0 )
      {
         m_data = (char *)malloc( size );
         if ( m_data != NULL )
         {
            m_dataSize = size;
            setLastError(NO_ERROR);
         }
         else
         {
            setLastError(ALLOC_FAILED);
            m_dataSize = 0;
         }
      } else
      {
         setLastError(NO_ERROR);
         m_data = NULL;
         m_dataSize = 0;
      }
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void fobject::freePayload(void)
{
   if ( m_data != NULL )
   {
      free(m_data);
      m_data = NULL;
      m_dataSize = 0;
   }
}

/* ------------------------------------------------------------------------- */

int fobject::readHeader(ffile &file)
{
   OBJECT obj;

   setLastError(UNDEFINED);

   freePayload();

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

int fobject::read(ffile &file)
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
         if ( allocPayload(obj.size) == -1 )
         {
            /* failed */
         } else
         {
            if ( file.read(m_data, obj.size) == -1 )          /* Read object payload */
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

   FDBG2( "Read ofs:%d %d", m_ofs, getLastError() );

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fobject::read(ffile &file, void *buf)
{
   OBJECT obj;

   freePayload();

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

int fobject::flush(ffile &file)
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
            if (m_data != NULL)
            {
               if ( file.write( m_data, m_obj.size ) == -1 )            /* write object payload */
               {
                  setLastError(file);
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

int fobject::flush(ffile &file, void *buf)
{
   setLastError(UNDEFINED);

   freePayload();

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
               if ( file.write( m_data, m_obj.size ) == -1 )            /* write object payload */
               {
                  setLastError(file);
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

void fobject::setId(unsigned int id)
{
   m_obj.id = id;
}

/* ------------------------------------------------------------------------- */

void fobject::setOptions(unsigned int options)
{
   m_obj.options = options;
}

/* ------------------------------------------------------------------------- */

void fobject::setNextChain(unsigned int nextChain)
{
    m_obj.nextChain = nextChain;
}

/* ------------------------------------------------------------------------- */

void fobject::zero()
{
   memset( m_data, 0, m_obj.size );
}

/* ------------------------------------------------------------------------- */

