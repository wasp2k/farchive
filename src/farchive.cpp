#include <stdlib.h>
#include <string.h>
#include "farchive.h"

/*
  Reserved object ID's

  0 - Reserved.
  1 - Archive header
  2 - First index object

  */

const farchive::VERSION farchive::m_version = { { 'f', 'a', 'r', 'c', 'h' }, 0x00, 0x0101 };

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
      setLastError(m_file);
      /* failed */
   } else
   {
      if ( m_file.write( &m_version, sizeof( m_version ) ) == -1 )         /* Write file pattern */
      {
         setLastError(m_file);
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

   FDBG( "create file %s (%d)", fileName, getLastError());

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

   if ( m_file.open( fileName ) == -1 )                     /* Open file */
   {
      setLastError(m_file);
   } else
   {
      if ( m_file.read(&ver,sizeof(ver)) == -1 )            /* Read file pattern */
      {
         setLastError(m_file);
      } else
      {
         if (memcmp( &ver, &m_version, sizeof( VERSION ) ) != 0)  /* Check file version */
         {
            setLastError(BAD_ARCHIVE_VERSION);
         } else
         {
            m_header.setOfs( sizeof( m_version ) );
            if ( m_header.read(m_file) == -1 )                       /* Read header information */
            {
               setLastError(m_header);
            } else
            {
               m_currObj.setOfs(m_header.getOfs());/* Adjust current object */
               if ( m_currObj.readHeader(m_file) == -1 )
               {
                  setLastError(m_currObj);
               } else
               {
                  setLastError(NO_ERROR);
               }
            }
         }
      }
   }
   FDBG( "Open file %s (%d)", fileName, getLastError());

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::close(void)
{
   m_file.close();
   setLastError(m_file);

   FDBG( "Close file (%d)", getLastError());

   return getStatus();
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

   FDBG( "Add Object: %d (%d)", obj.getId(), getLastError());

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::moveFirst()
{ 
   setLastError(UNDEFINED);
   m_currObj.setOfs( m_header.getOfs() );
   if ( m_currObj.readHeader( m_file ) == -1 )
   {
      setLastError(m_currObj);
   } else
   {
      setLastError(NO_ERROR);
   }
   FDBG( "MoveFirst (%d)", m_currObj.getId(), getLastError());
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::moveNext()
{
   long int nextOfs;
   setLastError(UNDEFINED);
   nextOfs = m_currObj.getOfs() + m_currObj.getBlockSize();
   m_currObj.setOfs( nextOfs );

   if ( m_currObj.readHeader( m_file ) == -1 )
   {
      setLastError(m_currObj);
   } else
   {
      setLastError(NO_ERROR);
   }
   FDBG( "moveNext Ofs: %d Obj: %d Siz: %d (%d)", nextOfs, m_currObj.getOfs(), m_currObj.getSize(), getLastError());
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::getObject(fobject &obj)
{
   setLastError(UNDEFINED);
   obj.setOfs( m_currObj.getOfs() );
   if ( obj.read( m_file ) == -1 )
   {
      setLastError(obj);
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void farchive::moveToOfs(long int ofs)
{
   setLastError(UNDEFINED);
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
