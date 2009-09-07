#include <stdlib.h>
#include <string.h>
#include "fmem.h"

#define FMEM_GROW_OBJ_BY   16

/* ------------------------------------------------------------------------- */

fmem::fmem(farchive &arch) : m_arch( arch )
{
   m_lockCnt = 0;
   m_dataPtr = 0;
   m_lenPtr = 0;

   m_objList = NULL;
   m_objCnt = 0;

   growObjList();
}

/* ------------------------------------------------------------------------- */

fmem::~fmem()
{
   freeObjList();
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

int fmem::getSize(void)
{
   int retVal = 0;
   setLastError(UNDEFINED);

   if ( m_dataPtr == NULL )
   {
      setLastError(BAD_OBJECT_PAYLOAD);
   } else
   {
      retVal = *m_lenPtr;
      setLastError(NO_ERROR);
   }
   return getStatus(retVal);
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

void *fmem::map(void)
{
   void *retVal = NULL;
   setLastError(UNDEFINED);

   if ( m_dataPtr == NULL )
   {
      setLastError(BAD_OBJECT_PAYLOAD);
   } else
   {
      if ( m_lockCnt >= 1024 )
      {
         setLastError(OBJECT_TOO_MANY_LOCKS);
      } else
      {
         m_lockCnt++;
         retVal = (void*)((int*)m_dataPtr + 1);
         setLastError(NO_ERROR);
      }
   }
   return retVal;
}

/* ------------------------------------------------------------------------- */

void fmem::unmap(void)
{
   setLastError(UNDEFINED);
   if ( m_lockCnt == 0 )
   {
      setLastError(OBJECT_NOT_LOCKED);
   } else
   {
      m_lockCnt--;
      setLastError(NO_ERROR);
   }
}

/* ------------------------------------------------------------------------- */

int fmem::write(void)
{
   setLastError(NO_ERROR);

   if ( m_dataPtr == NULL )
   {
      setLastError(BAD_OBJECT_PAYLOAD);
   } else
   {
      if ( !m_arch.isOpen() )
      {
         setLastError(BAD_ARCHIVE);
      } else
      {
         int bytesLeft = *m_lenPtr + sizeof( int );

         while(bytesLeft>0)
         {

         }
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::growObjList(void)
{
   fobject **objList;

   setLastError(UNDEFINED);

   objList = (fobject **)malloc( m_objCnt + FMEM_GROW_OBJ_BY );
   memset(objList, 0, sizeof(fobject*) * m_objCnt + FMEM_GROW_OBJ_BY);
   if ( objList == NULL )
   {
      setLastError(ALLOC_FAILED);
   } else
   {
      if ( m_objList == NULL )
      {
      } else
      {
      }

      m_objList = objList;
      m_objCnt += FMEM_GROW_OBJ_BY;
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::freeObjList()
{
   setLastError(UNDEFINED);

   if ( m_objList != NULL )
   {
      for (int i=0; i<m_objCnt; i++)
      {
         if ( m_objList[i] != NULL )
         {
            delete m_objList[i];
            m_objList[i] = NULL;
         }
      }
      ::free(m_objList);

      m_objList = NULL;
      m_objCnt = 0;
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::getObj(int ofs, int size)
{
   setLastError(UNDEFINED);





   return getStatus();
}

/* ------------------------------------------------------------------------- */
