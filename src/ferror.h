#ifndef FCEXCEPTION_H
#define FCEXCEPTION_H

class ferror
{
public:
   enum ERRORCODE
   {
      UNDEFINED,
      NO_ERROR,

      ALLOC_FAILED,

      /* FC File error codes */
      FILE_OPEN_FAILED,
      FILE_CREATE_FAILED,
      FILE_CLOSE_FAILED,
      WRITE_FILE_FAILED,
      READ_FILE_FAILED,
      SEEK_FAILED,
      SEEK_SYNC_FAILED,
      TELL_FAILED,

      /* FC Archive codes */
      BAD_ARCHIVE_VERSION,

      /* FC Object */
      BAD_OBJECT_PAYLOAD,
      BAD_OBJECT_PATTERN,

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

   inline ERRORCODE getLastError(){ return m_lastError; }
   inline void setLastError(ERRORCODE lastError){ m_lastError = lastError; }

   inline int succeeded(void){ return ((m_lastError == NO_ERROR) ? 0 : -1); }
};

#endif // FCEXCEPTION_H
