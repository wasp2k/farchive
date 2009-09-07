#include <stdlib.h>
#include <string.h>
#include "fmem.h"

#define FMEM_GROW_OBJ_BY   16

/* ------------------------------------------------------------------------- */

fmem::fmem()
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
   char *dataPtr;

   setLastError(UNDEFINED);

   dataPtr = new char[size+sizeof(int)];
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

         delete[] m_dataPtr;           /* release old buffer */
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
      delete[] m_dataPtr;
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

int fmem::write(farchive &arch)
{
   setLastError(NO_ERROR);

   if ( m_dataPtr == NULL )
   {
      setLastError(BAD_OBJECT_PAYLOAD);
   } else
   {
      if ( !arch.isOpen() )
      {
         setLastError(BAD_ARCHIVE);
      } else
      {
         int objIdx = 0;
         int bytesLeft = *m_lenPtr + sizeof( int );
         char *ptr = m_dataPtr;

         setLastError(NO_ERROR);
         while((bytesLeft>0) && (getStatus() != -1))
         {
            if ( objIdx >= m_objCnt )
            {
               growObjList();
            }

            if ( getStatus() != -1 )
            {
               fobject* &obj = m_objList[objIdx];
               if ( obj == NULL )
               {
                  obj = new fobject(bytesLeft);
                  if ( obj == NULL )
                  {
                     setLastError(ALLOC_FAILED);
                  } else
                  {
                     memcpy(obj->getPtr(),ptr, obj->getSize());

                  }
               }

               arch.writeObject(*obj);

               ptr += obj->getSize();
               bytesLeft -= obj->getSize();
               objIdx++;
            }
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

   objList = new fobject*[ m_objCnt + FMEM_GROW_OBJ_BY ];
   memset(objList, 0, sizeof(fobject*) * (m_objCnt + FMEM_GROW_OBJ_BY));
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
