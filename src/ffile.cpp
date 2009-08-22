#include "ffile.h"

/* ------------------------------------------------------------------------- */

ffile::ffile()
{
   m_file = NULL;
}

/* ------------------------------------------------------------------------- */

ffile::~ffile()
{
   if ( isOpen() )   /* close the file before destruct */
   {
      FINFO( "File was not closed before destruct" );
      close();
   }
}

/* ------------------------------------------------------------------------- */

int ffile::create(const char *fileName)
{
   setLastError(UNDEFINED);

   FDBG( "Create file %s", fileName );

   if ( isOpen() )
   {
      FINFO( "file already open" );
      if (close()==-1)
      {
      }
   } else
   {
      setLastError(NO_ERROR);
   }

   if ( getStatus() != -1 )
   {
      m_file = fopen( fileName, "w+" );
      if ( m_file == NULL )
      {
         FERR( "File create failed" );
         setLastError(FILE_CREATE_FAILED);
      }
      else
      {
         setLastError(NO_ERROR);
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int ffile::open(const char *fileName)
{
   setLastError(UNDEFINED);

   FDBG( "Open file %s", fileName );

   if ( isOpen() )
   {
      FINFO( "file already open" );
      if (close()==-1)
      {
      }
   } else
   {
      setLastError(NO_ERROR);
   }

   if ( getStatus() != -1 )
   {
      m_file = fopen( fileName, "r+" );
      if ( m_file == NULL )
      {
         FERR( "Failed to open" );
         setLastError(FILE_OPEN_FAILED);
      }
      else
      {
         setLastError(NO_ERROR);
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int ffile::close(void)
{
   setLastError(UNDEFINED);
   if (isOpen())
   {
      if (fclose(m_file) != 0)
      {
         FERR( "Failed to close file" );
         setLastError(FILE_CLOSE_FAILED);
      }
      else
      {
         m_file = NULL;
         setLastError(NO_ERROR);
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int ffile::write(const void *buf, const int size)
{
   size_t written;
   setLastError(UNDEFINED);
   written = fwrite(buf, (size_t)size, 1, m_file );
   if ( written != 1 )
   {
      FERR( "Write failed" );
      setLastError(FILE_WRITE_FAILED);
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int ffile::read(void *buf, const int size)
{
   size_t read;
   setLastError(UNDEFINED);
   read = fread(buf, (size_t)size, 1, m_file );
   if ( read != 1 )
   {
      setLastError(FILE_READ_FAILED);
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int ffile::seek(const int ofs, const ffile::ORIGIN origin)
{
   setLastError(UNDEFINED);
   if ( fseek( m_file, (long int)ofs, origin ) != 0 )
   {
      setLastError(FILE_SEEK_FAILED);
   } else
   {
      setLastError(NO_ERROR);
   }

   if ( getStatus() != -1 )
   {
      return getStatus( ftell(m_file) );
   } else
   {
      return -1;
   }
}

/* ------------------------------------------------------------------------- */

int ffile::tell(void)
{
   long int ofs;
   setLastError(UNDEFINED);
   ofs = ftell( m_file );
   if ( ofs == -1 )
   {
      setLastError(FILE_TELL_FAILED);
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus((int)ofs);
}

/* ------------------------------------------------------------------------- */
