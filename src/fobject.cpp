#include <string.h>
#include <stdlib.h>
#include "fobject.h"

#if FOBJECT_INCLUDE_DEBUG_PATTEN
   #define FOBJECT_PATTERN      0x3a4a424f
#endif

/* ------------------------------------------------------------------------- */

fobject::fobject(long int ofs, unsigned int size)
{
   allocPayload(size);

   m_ofs = ofs;
   m_obj.id = 0;
   m_obj.size = size;
   m_obj.options = 0;
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
         FDBG("FCObject::readHeader id: %d ofs: %d siz: %d", obj.id, m_ofs, obj.size );

         if ( m_data != NULL )
         {
            free( m_data );
            m_data = NULL;
         }

         #if FOBJECT_INCLUDE_DEBUG_PATTEN
         if ( obj.pattern != FOBJECT_PATTERN )
         {
            /*throw( new FCException(FCException::BAD_OBJECT_PATTERN) );*/
         }
         #endif

         m_obj = obj;
         m_onlyHeader = true;
      }
   }

   return getStatus();
}

/* ------------------------------------------------------------------------- */

void fobject::read(ffile &file)
{
   OBJECT obj;

   file.seek(m_ofs,ffile::SET);                             /* Seek to the offset */
   file.read(&obj, sizeof(obj));                             /* Read object header */

   if ( m_data != NULL )
   {
      if ( m_dataSize != obj.size )
      {
         free(m_data);
         m_data = NULL;
      }
   }

   m_obj = obj;
   if ( m_data == NULL )
   {
      m_data = (char *)malloc(m_obj.size);           /* Allocate object memory */
      if ( m_data == NULL )
      {
         /* throw( new FCException(FCException::ALLOC_FAILED) ); */
      }
      m_dataSize = m_obj.size;
   }
   file.read(m_data, m_obj.size);          /* Read object payload */
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
      if (m_data == NULL)
      {
         setLastError(BAD_OBJECT_PAYLOAD);
      } else
      {
         if (m_ofs == -1)
         {
            m_ofs = file.seek(0, ffile::END );        /* Move to the end of the file */


            #if FOBJECT_INCLUDE_DEBUG_PATTEN
            m_obj.pattern = FOBJECT_PATTERN;
            #endif

            file.write( &m_obj, sizeof( m_obj ) );    /* write object header */
            if ( file.getStatus() == -1 )
               setLastError(file);
         } else
         {
            file.seek( m_ofs + sizeof( OBJECT ), ffile::SET ); /* Move to the object offset */
            if ( file.getStatus() == -1 )
               setLastError(file);
         }


         if (getStatus())
         {
            file.write( m_data, m_obj.size );            /* write object payload */
            if ( file.getStatus() == -1 )
               setLastError(file);
         }
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void fobject::setId(unsigned int id)
{
   if ( m_obj.id != id )
   {
      m_obj.id = id;
   }
}

/* ------------------------------------------------------------------------- */

void fobject::setOptions(unsigned int options)
{
   if ( m_obj.options != options )
   {
      m_obj.options = options;
   }
}

/* ------------------------------------------------------------------------- */

void fobject::zero()
{
   memset( m_data, 0, m_obj.size );
}

/* ------------------------------------------------------------------------- */
