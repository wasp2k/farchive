#include <stdlib.h>
#include <string.h>
#include "farchive.h"

/*
  Reserved object ID's

  0 - Reserved.
  1 - Archive header
  2 - First index object

  */

const farchive::VERSION farchive::m_version = { { 'f', 'a', 'r', 'c', 'h', 'v' }, 0x0101 };

/* ------------------------------------------------------------------------- */

farchive::farchive()
{
   setLastError(UNDEFINED);
}

/* ------------------------------------------------------------------------- */

farchive::~farchive()
{
   close();
}

/* ------------------------------------------------------------------------- */

int farchive::create(const char *fileName)
{
   setLastError(UNDEFINED);
   if ( m_file.create( fileName ) == -1 )                                  /* Create file */
   {
      /* failed */
   } else
   {
      if ( m_file.write( &m_version, sizeof( m_version ) ) == -1 )         /* Write file pattern */
      {
         /* failed */
      } else
      {
         if ( createFileHeader() == -1 )                                   /* create file header */
         {
            /* failed */
         } else
         {
            close();                                                             /* Close file */
         }
      }
   }

   if ( getStatus() != -1)
   {
      open(fileName);                                                      /* Open file again */
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::createFileHeader(void)
{
   setLastError(UNDEFINED);

   m_header.zero();
   m_header->lastID = 1;

   if ( farchive::add(m_header) == -1 )
   {
      /* failed */
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::open(const char *fileName)
{
   VERSION ver;

   m_file.open( fileName );                                 /* Open file */

   m_file.read(&ver,sizeof(ver));                           /* Read file pattern */
   if (memcmp( &ver, &m_version, sizeof( VERSION ) ) != 0)  /* Check file version */
   {
      /*throw( new FCException( FCException::BAD_ARCHIVE_VERSION ) );*/
   }
   m_header.setOfs( sizeof( m_version ) );
   m_header.read(m_file);                                    /* Read header information */

   m_currObj.setOfs(m_header.getOfs());                     /* Adjust current object */
   m_currObj.readHeader(m_file);

   return -1;
}

/* ------------------------------------------------------------------------- */

int farchive::close(void)
{
   return m_file.close();
}

/* ------------------------------------------------------------------------- */

int farchive::add(fobject &obj)
{
   setLastError(UNDEFINED);

   obj.setOfs(-1);
   obj.setId( m_header->lastID++ );
   if ( obj.flush(m_file) == -1 )
   {
      setLastError(obj);
   } else
   {
      if ( m_header.flush(m_file) == -1 )
      {
         setLastError(m_header);
      } else
      {
         setLastError(NO_ERROR);
      }
   }

   return getStatus();
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
   FDBG( "FCArchive::moveNext Ofs: %d Obj: %d Siz: %d", nextOfs, m_currObj.getOfs(), m_currObj.getSize() );
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
   bool retVal = false;
   /* TODO
   long int fileSize = m_file.getSize();
   long int nextOfs = m_currObj.getOfs() + m_currObj.getBlockSize();

   if ( nextOfs >= fileSize )
   {
      retVal = true;
   } else
   {
      retVal = false;
   }*/
   return retVal;
}

/* ------------------------------------------------------------------------- */

void farchive::moveToOfs(long int ofs)
{
   m_currObj.setOfs( ofs );
   m_currObj.readHeader( m_file );
   FDBG( "FCArchive::moveToOfs Ofs: %d Obj: %d Siz: %d", ofs, m_currObj.getOfs(), m_currObj.getSize() );
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
