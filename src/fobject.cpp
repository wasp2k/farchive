#include <string.h>
#include <stdlib.h>
#include "fobject.h"

#if FCOBJECT_INCLUDE_PATTEN
   #define FCOBJECT_PATTERN      0x3a4a424f
#endif

/* ------------------------------------------------------------------------- */

fobject::fobject(long int ofs, unsigned int size)
{
   if ( size > 0 )
   {
      m_data = (char *)malloc( size );
      if ( m_data == NULL )
      {
         /*throw( new FCException(FCException::ALLOC_FAILED) );*/
      }
      m_dataSize = size;
   } else
   {
      m_data = NULL;
   }

   m_ofs = ofs;
   m_obj.id = 0;
   m_obj.size = size;
   m_obj.options = 0;
   m_dirty = true;
}

/* ------------------------------------------------------------------------- */

fobject::~fobject()
{
   if ( m_data != NULL )
   {
      free(m_data);
   }
}

/* ------------------------------------------------------------------------- */

void fobject::readHeader(ffile &file)
{
   OBJECT obj;

   file.seek(m_ofs,ffile::SET);                             /* Seek to the offset */
   file.read(&obj, sizeof(obj));                             /* Read object header */

   fdbg(9, "FCObject::readHeader id: %d ofs: %d siz: %d", obj.id, m_ofs, obj.size );

   if ( m_data != NULL )
   {
      free( m_data );
      m_data = NULL;
   }

   #if FCOBJECT_INCLUDE_PATTEN
   if ( obj.pattern != FCOBJECT_PATTERN )
   {
      /*throw( new FCException(FCException::BAD_OBJECT_PATTERN) );*/
   }
   #endif

   m_obj = obj;
   m_dirty = false;
   m_onlyHeader = true;
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
   m_dirty = false;
}

/* ------------------------------------------------------------------------- */

void fobject::flush(ffile &file)
{
   if ( 1 || m_dirty )
   {
      if ( m_data == 0 )
      {
         /*throw( new FCException(FCException::BAD_OBJECT_PAYLOAD) );*/
      }
      
      if ( m_ofs == -1 )
      {
         file.seek(0, ffile::END );
         m_ofs = file.tell();
         #if FCOBJECT_INCLUDE_PATTEN
         m_obj.pattern = FCOBJECT_PATTERN;
         #endif
         file.write( &m_obj, sizeof( m_obj ) );
      } else
      {
         file.seek( m_ofs + sizeof( OBJECT ), ffile::SET );
      }
      file.write( m_data, m_obj.size );
      m_dirty = false;
   }
}

/* ------------------------------------------------------------------------- */

void fobject::setId(unsigned int id)
{
   if ( m_obj.id != id )
   {
      m_obj.id = id;
      m_dirty = true;
   }
}

/* ------------------------------------------------------------------------- */

void fobject::setOptions(unsigned int options)
{
   if ( m_obj.options != options )
   {
      m_obj.options = options;
      m_dirty = true;
   }
}

/* ------------------------------------------------------------------------- */

void fobject::zero()
{
   memset( m_data, 0, m_obj.size );
}

/* ------------------------------------------------------------------------- */
