#ifndef FERROR_H
#define FERROR_H

#include "fdbg.h"

class ferror
{
public:
   enum ERRORCODE
   {
      UNDEFINED = 0,
      NO_ERROR,

      ALLOC_FAILED,
      BAD_PARAMETER,

      /* FC File error codes */
      FILE_OPEN_FAILED,
      FILE_CREATE_FAILED,
      FILE_CLOSE_FAILED,
      FILE_WRITE_FAILED,
      FILE_READ_FAILED,
      FILE_SEEK_FAILED,
      FILE_SEEK_SYNC_FAILED,
      FILE_TELL_FAILED,
      FILE_EOF,

      /* farchive */
      BAD_ARCHIVE_VERSION,

      /* fobject */
      BAD_OBJECT_PAYLOAD,
      BAD_OBJECT_PATTERN,
      BAD_FILE_OBJECT,
      BAD_ARCHIVE,

      /* farchiveidx */
      BAD_INDEX_OBJECT,
      BAD_INDEX_POOL,
      EXPAND_INDEX_FAILED,
      OBJECT_NOT_FOUND,

      /* fmem */
      OBJECT_LOCKED,
      OBJECT_TOO_MANY_LOCKS,
      OBJECT_NOT_LOCKED,


      LAST_CODE
   };

protected:
   ERRORCODE  m_lastError;

public:
   ferror();
   ferror(ERRORCODE lastError);

   inline ERRORCODE getLastError() const { return m_lastError; }
   inline void setLastError(ERRORCODE lastError){ m_lastError = lastError; }
   inline void setLastError(const ferror &error){ m_lastError = error.getLastError(); }

   inline int getStatus(int retValue=0) const
   {
      if ( m_lastError != NO_ERROR )
      {
         FINFO1( "getStatus %d", m_lastError );
      }
      return ((m_lastError == NO_ERROR) ? retValue : -1);
   }
};

#endif // FERROR_H
