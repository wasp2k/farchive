#include <stdlib.h>
#include <string.h>
#include "farchive.h"

/*
  Reserved object ID's

  0 - Reserved.
  1 - Archive header
  2 - First index object

  */

const farchive::VERSION farchive::m_version = { { 'F', 'I', 'N', 'D', 'I', 'T' }, 0x0101 };

/* ------------------------------------------------------------------------- */

farchive::farchive()
{
}

/* ------------------------------------------------------------------------- */

farchive::~farchive()
{
   close();
}

/* ------------------------------------------------------------------------- */

void farchive::create(const char *fileName)
{
   m_file.create( fileName );                               /* Create file */
   m_file.write( &m_version, sizeof( m_version ) );         /* Write file pattern */

   createFileHeader();

   close();                                                 /* Close file */

   open(fileName);                                          /* Open file again */
}

/* ------------------------------------------------------------------------- */

void farchive::createFileHeader(void)
{
   m_header.zero();
#if 0 /* TODO */
   m_header->lastID = 1;
#endif
   farchive::add(m_header);
}

/* ------------------------------------------------------------------------- */

void farchive::open(const char *fileName)
{
   VERSION ver;

   m_file.open( fileName );                                 /* Open file */

   m_file.read(&ver,sizeof(ver));                           /* Read file pattern */
   if (memcmp( &ver, &m_version, sizeof( VERSION ) ) != 0)  /* Check file version */
   {
      /*throw( new FCException( FCException::BAD_ARCHIVE_VERSION ) );*/
   }
   m_header.setOfs( sizeof( m_version ) );
   m_header.read(m_file);                                   /* Read header information */

   m_currObj.setOfs(m_header.getOfs());                     /* Adjust current object */
   m_currObj.readHeader(m_file);
}

/* ------------------------------------------------------------------------- */

void farchive::close(void)
{
   m_file.close();
}

/* ------------------------------------------------------------------------- */

void farchive::add(fobject &obj)
{
   obj.setOfs(-1);
   /*obj.setId( m_header->lastID++ ); TODO */
   obj.flush(m_file);

   m_header.setDirty();
   m_header.flush(m_file);
}

/* ------------------------------------------------------------------------- */

void farchive::moveFirst()
{ 
   m_currObj.setOfs( m_header.getOfs() );
   m_currObj.readHeader( m_file );
}

/* ------------------------------------------------------------------------- */

void farchive::moveNext()
{
   long int nextOfs;
   nextOfs = m_currObj.getOfs() + m_currObj.getBlockSize();
   m_currObj.setOfs( nextOfs );
   m_currObj.readHeader( m_file );
   fdbg( 9, "FCArchive::moveNext Ofs: %d Obj: %d Siz: %d", nextOfs, m_currObj.getOfs(), m_currObj.getSize() );
}

/* ------------------------------------------------------------------------- */

void farchive::getObject(fobject &obj)
{
   obj.setOfs( m_currObj.getOfs() );
   obj.read( m_file );
}

/* ------------------------------------------------------------------------- */

bool farchive::isEOF(void)
{
   bool retVal;
   long int fileSize = m_file.getSize();
   long int nextOfs = m_currObj.getOfs() + m_currObj.getBlockSize();

   if ( nextOfs >= fileSize )
   {
      retVal = true;
   } else
   {
      retVal = false;
   }
   return retVal;
}

/* ------------------------------------------------------------------------- */

void farchive::moveToOfs(long int ofs)
{
   m_currObj.setOfs( ofs );
   m_currObj.readHeader( m_file );
   fdbg( 9, "FCArchive::moveToOfs Ofs: %d Obj: %d Siz: %d", ofs, m_currObj.getOfs(), m_currObj.getSize() );
}

/* ------------------------------------------------------------------------- */

bool farchive::moveToObject(const unsigned int objId)
{
   bool found = false;
   moveFirst();
   do
   {
      if  ( m_currObj.getId() == objId )
      {
         found = true;
         break;
      }

      if ( isEOF() )
         break;

      moveNext();

   } while( 1 );
   return found;
}

/* ------------------------------------------------------------------------- */

bool farchive::remove(const unsigned int objId)
{
   bool found;
   found = moveToObject(objId);
   if ( found )
   {
      m_currObj.setId(0);
      m_currObj.flush(m_file);
   }
   return found;
}

/* ------------------------------------------------------------------------- */
