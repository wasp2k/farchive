#ifndef FERROR_H
#define FERROR_H

class ferror
{
public:
   enum ERRORCODE
   {
      UNDEFINED = 0,
      NO_ERROR,

      ALLOC_FAILED,

      /* FC File error codes */
      FILE_OPEN_FAILED,
      FILE_CREATE_FAILED,
      FILE_CLOSE_FAILED,
      FILE_WRITE_FAILED,
      FILE_READ_FAILED,
      FILE_SEEK_FAILED,
      FILE_SEEK_SYNC_FAILED,
      FILE_TELL_FAILED,

      /* FC Archive codes */
      BAD_ARCHIVE_VERSION,

      /* FC Object */
      BAD_OBJECT_PAYLOAD,
      BAD_OBJECT_PATTERN,
      BAD_FILE_OBJECT,

      /* FC Indexed Archive */
      BAD_INDEX_OBJECT,
      BAD_INDEX_POOL,
      EXPAND_INDEX_FAILED,

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

   inline int getStatus(int retValue=0) const { return ((m_lastError == NO_ERROR) ? retValue : -1); }
};

#endif // FERROR_H
