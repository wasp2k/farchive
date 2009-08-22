#include "ffile.h"

/* ------------------------------------------------------------------------- */

ffile::ffile()
{
   m_file = NULL;
}

/* ------------------------------------------------------------------------- */

ffile::~ffile()
{
   if ( isOpen() )
      close();
}

/* ------------------------------------------------------------------------- */

int ffile::create(const char *fileName)
{
   setLastError(UNDEFINED);
   if ( isOpen() )
   {
      close();
   } else
   {
      setLastError(NO_ERROR);
   }

   if ( succeeded() != -1 )
   {
      m_file = fopen( fileName, "w+" );
      if ( m_file == NULL )
      {
         setLastError(FILE_CREATE_FAILED);
      }
      else
      {
         setLastError(NO_ERROR);
      }
   }
   return succeeded();
}

/* ------------------------------------------------------------------------- */

int ffile::open(const char *fileName)
{
   setLastError(UNDEFINED);
   if ( isOpen() )
   {
      close();
   } else
   {
      setLastError(NO_ERROR);
   }

   if ( succeeded() != -1 )
   {
      m_file = fopen( fileName, "r+" );
      if ( m_file == NULL )
      {
         setLastError(FILE_OPEN_FAILED);
      }
      else
      {
         setLastError(NO_ERROR);
      }
   }
   return succeeded();
}

/* ------------------------------------------------------------------------- */

int ffile::close(void)
{
   setLastError(UNDEFINED);
   if (isOpen())
   {
      if (fclose(m_file) != 0)
      {
         setLastError(FILE_CLOSE_FAILED);
      }
      else
      {
         m_file = NULL;
         setLastError(NO_ERROR);
      }
   }
   return succeeded();
}

/* ------------------------------------------------------------------------- */

void ffile::write(const void *buf, const size_t size)
{
   size_t elementsWritten;

   elementsWritten = fwrite(buf, size, 1, m_file );

   if ( elementsWritten != 1 )
   {
      //throw( new FCException( FCException::WRITE_FILE_FAILED ) );
   }
}

/* ------------------------------------------------------------------------- */

void ffile::read(void *buf, const size_t size)
{
   size_t elementsRead;

   elementsRead = fread(buf, size, 1, m_file );
   if ( elementsRead != 1 )
   {
      //throw( new FCException( FCException::READ_FILE_FAILED ) );
   }
}

/* ------------------------------------------------------------------------- */

long int ffile::seek(const long int ofs, const ffile::ORIGIN origin)
{
   if ( fseek( m_file, ofs, origin ) != 0 )
   {
      //throw( new FCException( FCException::SEEK_FAILED ) );
   }
   return ftell( m_file );
}

/* ------------------------------------------------------------------------- */

long int ffile::tell(void)
{
   long int ofs;

   ofs = ftell( m_file );
   if ( ofs == -1 )
   {
      //throw( new FCException( FCException::TELL_FAILED ) );
   }

   /*if ( ofs != m_ofs )
   {
      //throw( new FCException( FCException::SEEK_SYNC_FAILED ) );
   }*/
   return ofs;
}

/* ------------------------------------------------------------------------- */
