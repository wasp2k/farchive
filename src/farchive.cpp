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
   if ( isOpen() )
   {
      close();
   }
}

/* ------------------------------------------------------------------------- */

int farchive::create(const char *fileName)
{
   setLastError(UNDEFINED);
   if ( m_file.create( fileName ) == -1 )                                  /* Create file */
   {
      /* failed */
      setLastError(m_file);
   } else
   {
      if ( m_file.write( &m_version, sizeof( m_version ) ) == -1 )         /* Write file pattern */
      {
         setLastError(m_file);
         /* failed */
      } else
      {
         if ( createArchiveHeader() == -1 )                                   /* create file header */
         {
            /* failed */
         } else
         {
            close();                                                       /* Close file */
         }
      }
   }

   if ( getStatus() != -1)
   {
      open(fileName);                                                      /* Open file again */
   }

   FDBG2( "create file %s (%d)", fileName, getLastError());

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::createArchiveHeader(void)
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
            moveFirst();
            m_header.unmap();
            m_header.free();
            if ( read(m_header) == -1 )                       /* Read header information */
            {
               setLastError(m_header);
            } else
            {
#if 0
               m_currObj.setOfs(m_header.getOfs());/* Adjust current object */
               if ( m_currObj.readHeader(m_file) == -1 )
               {
                  setLastError(m_currObj);
               } else
               {
                  setLastError(NO_ERROR);
               }
#endif
            }
            m_header.map();
         }
      }
   }
   FDBG2( "Open file %s (%d)", fileName, getLastError());

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::close(void)
{
   m_file.close();
   setLastError(m_file);

   FDBG1( "Close file (%d)", getLastError());

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::add(fmem &mem)
{
   setLastError(UNDEFINED);

   if ( mem.freeObjList() == -1 )
   {
      /* failed */
      setLastError(mem);
   } else
   {
      if ( write(mem) == -1 )
      {
         /* failed */
      } else
      {
         setLastError(NO_ERROR);
      }
   }
   FDBG1( "Add Object: (%d)", getLastError());
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::moveFirst()
{ 
   setLastError(UNDEFINED);
   m_currObj.setOfs( m_header.getObj(0).getOfs() );
   if ( m_currObj.readHeader( m_file ) == -1 )
   {
      setLastError(m_currObj);
   } else
   {
      setLastError(NO_ERROR);
   }
   FDBG2( "MoveFirst (%d)", m_currObj.getId(), getLastError());
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::moveNext()
{
   int repeat;
   long int nextOfs;
   setLastError(UNDEFINED);

   do
   {
      repeat = 0;
      nextOfs = m_currObj.getOfs() + m_currObj.getObjectSize();
      m_currObj.setOfs( nextOfs );

      if ( m_currObj.readHeader( m_file ) == -1 )
      {
         setLastError(m_currObj);
      } else
      {
         setLastError(NO_ERROR);
      }
      FDBG4( "moveNext Ofs: %d Obj: %d Siz: %d (%d)", nextOfs, m_currObj.getOfs(), m_currObj.getSize(), getLastError());

      if ( getStatus() != -1 )
      {
         if ( m_currObj.getId() == 0 )    /* Removed object detected, move to the next object again */
         {
            repeat = 1;
         }
      }
   } while( repeat != 0 );
   return getStatus();
}

/* ------------------------------------------------------------------------- */

#if 0
int farchive::read(fmem &/*obj*/)
{
   setLastError(UNDEFINED);
#if 0
   obj.setOfs( m_currObj.getOfs() );
   if ( obj.read( m_file ) == -1 )
   {
      setLastError(obj);
   } else
   {
      setLastError(NO_ERROR);
   }
#endif
   return getStatus();
}
#endif

/* ------------------------------------------------------------------------- */

#if 0
int farchive::write(fmem &/*obj*/)
{
   setLastError(UNDEFINED);
#if 0
   if ( obj.getOfs() == -1 )
   {
      add(obj);
   } else
   {
      if ( obj.flush( m_file ) == -1 )
      {
         setLastError(obj);
      } else
      {
         setLastError(NO_ERROR);
      }
   }
#endif
   return getStatus();
}
#endif

/* ------------------------------------------------------------------------- */

int farchive::moveTo(const unsigned int objId)
{
   bool found = false;
   setLastError(UNDEFINED);
   if ( m_currObj.getId() == objId )
   {
      found = true;
   } else
   {
      if ( moveFirst() == -1 )
      {
         /* failed */
      } else
      {
         do
         {
            if  ( m_currObj.getId() == objId )
            {
               found = true;
               break;
            }
            moveNext();
         } while( getStatus() != -1 );
      }
   }

   if ( found == true )
   {
      FDBG2( "moveToObject id: %d at: %d", objId, m_currObj.getOfs() );
      setLastError(NO_ERROR);
   } else
   {
      FWARN1( "moveToObject id: %d not found", objId );
   }

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::remove(const unsigned int /*objId*/)
{
   setLastError(UNDEFINED);
#if 0
   if ( moveTo(objId) != -1 )
   {
      m_currObj.setId(0);
      if ( m_currObj.flush(m_file) == -1 )
      {
         setLastError(m_currObj);
      } else
      {
         setLastError(NO_ERROR);
      }
   }

   if ( getStatus() != -1 )
   {
      FDBG2( "remove obj id: %d ofs: %d", objId, m_currObj.getOfs() );
   } else
   {
      FERR1( "remove obj id: %d failed", objId );
   }
#endif
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::isOpen(void)
{
   int open;
   open = m_file.isOpen() != false;
   setLastError(NO_ERROR);
   return getStatus(open);
}

/* ------------------------------------------------------------------------- */

int farchive::read(fmem &mem)
{
   int repeat;
   int startFrom;
   char *ptr;
   setLastError(UNDEFINED);

   if ( mem.free() == -1 )
   {
      setLastError(mem);
   } else
   {
      do
      {
         repeat = 0;
         startFrom = mem.realloc( mem.getSize() + m_currObj.getSize() );

         if ( startFrom == -1 )
         {
            /* failed */
            setLastError(mem);
         } else
         {
            ptr = (char *)mem.map();
            if ( ptr == NULL )
            {
               /* failed */
               setLastError(mem);
            } else
            {
               ptr += startFrom;

               if ( m_currObj.readPayload( m_file, ptr ) == -1 )
               {
                  /* failed */
                  setLastError(m_currObj);
               } else
               {
                  mem.addObj(m_currObj);
                  setLastError(NO_ERROR);

                  if ( m_currObj.getNextChain() != 0 )
                  {
                     moveTo(m_currObj.getNextChain());
                     repeat = 1;
                  }
               }

               mem.unmap();
            }
         }
      } while( ( getStatus() != -1 ) && ( repeat != 0 ) );
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchive::write(fmem &mem)
{
   char *ptr;
   int bytesLeft;

   setLastError(UNDEFINED);

   ptr = (char *)mem.map();
   if ( ptr == NULL )
   {
      setLastError(mem);
   } else
   {
      fobject *lastObj = NULL;

      bytesLeft = mem.getSize();

      setLastError(NO_ERROR);
      for ( int n = 0; n < mem.getObjCnt(); n ++ )
      {
         fobject &obj = mem.getObj(n);

         if ( (int)obj.getSize() > bytesLeft )
         {
            obj.setSize(bytesLeft);
         }

         if ( (int)obj.getPayloadSize() <= bytesLeft )
         {
            obj.setSize(obj.getPayloadSize());
         }

         if ( obj.writePayload( m_file, ptr ) == -1 )
         {
            /* failed */
            setLastError(obj);
            break;
         } else
         {
            ptr += obj.getSize();
            bytesLeft -= obj.getSize();
         }
         lastObj = &obj;       /* Remember last object to update next chain ID of needed */
      }

      if ( getStatus() != -1 )
      {
         if ( bytesLeft > 0 )
         {
            fobject obj;

            obj.setOfs(-1);
            obj.setId(m_header->lastID++);
            obj.setSize(bytesLeft);

            if ( obj.writePayload(m_file,ptr) == -1 )
            {
               setLastError(obj);
            } else
            {
               if ( mem.addObj(obj) == -1 )
               {
                  /* failed */
                  setLastError(mem);
               } else
               {

                  if ( lastObj != NULL )
                  {
                     lastObj->setNextChain(obj.getId());
                     lastObj->writeHeader(m_file);
                  } else
                  {
                     setLastError(NO_ERROR);
                  }

                  if ( getStatus() != -1 )
                  {
                     if ( write(m_header) == -1 )
                     {
                        /* failed */
                     } else
                     {
                        setLastError(NO_ERROR);
                     }
                  }
               }
            }
         }
      }
      mem.unmap();
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */
