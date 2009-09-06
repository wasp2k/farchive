#include <stdlib.h>
#include <string.h>
#include "fmem.h"

/* ------------------------------------------------------------------------- */

fmem::fmem()
{
   m_lockCnt = 0;
   m_dataPtr = 0;
   m_lenPtr = 0;
}

/* ------------------------------------------------------------------------- */

fmem::~fmem()
{
}

/* ------------------------------------------------------------------------- */

int fmem::setSize(int size)
{
   setLastError(UNDEFINED);

   if ( realloc(size) == -1 )
   {
      /* failed */
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */
int fmem::realloc(int size)
{
   void *dataPtr;

   setLastError(UNDEFINED);

   dataPtr = malloc(size);
   if ( dataPtr == NULL )
   {
      setLastError(ALLOC_FAILED);
   } else
   {
      if ( m_dataPtr != NULL )         /* Keep the content of the data buffer */
      {
         int oldSize = *m_lenPtr;
         int toCopy;

         if ( oldSize >= size )        /* Calculate number of bytes to copy */
            toCopy = size;
         else
            toCopy = oldSize;

         toCopy += sizeof( int );      /* First int is buffer length, therefore add int length */

         memcpy( dataPtr, m_dataPtr, toCopy );

         ::free( m_dataPtr );            /* release old buffer */
      }
      m_dataPtr = dataPtr;
      m_lenPtr = (int*)dataPtr;

      *m_lenPtr = size;

      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::free()
{
   if ( m_dataPtr != NULL )
   {
      ::free(m_dataPtr);
      m_dataPtr = NULL;
      m_lenPtr = NULL;
   }
   setLastError(NO_ERROR);
   return getStatus();
}

/* ------------------------------------------------------------------------- */
