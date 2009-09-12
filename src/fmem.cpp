#include <stdlib.h>
#include <string.h>
#include "fmem.h"

#define FMEM_OBJS_GROW_BY     16

/* ------------------------------------------------------------------------- */

fmem::fmem()
{
   m_lockCnt = 0;
   m_buf = 0;
   m_bufSize = 0;
   m_size = 0;

   m_objList = NULL;
   m_objCnt = 0;
   m_objAllocated = 0;
}

/* ------------------------------------------------------------------------- */

fmem::~fmem()
{
   free();
}

/* ------------------------------------------------------------------------- */

int fmem::alloc(int size)
{
   setLastError(UNDEFINED);
   if ( m_lockCnt != 0 )
   {
      setLastError(OBJECT_MAPPED);
   } else
   {
      free();
      if ( realloc(size) == -1 )
      {
         /* failed */
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::realloc(int size)
{
   int oldSize = getSize();
   char *dataPtr;
   setLastError(UNDEFINED);

   if ( m_lockCnt != 0 )
   {
      setLastError(OBJECT_MAPPED);
   } else
   {
      dataPtr = new char[size];
      if ( dataPtr == NULL )
      {
         setLastError(ALLOC_FAILED);
      } else
      {
         memset( dataPtr, 0, size );

         if ( m_buf != NULL )             /* Keep the content of the data buffer */
         {
            int oldSize = m_size;
            int toCopy;

            if ( oldSize >= size )        /* Calculate number of bytes to copy */
               toCopy = size;
            else
               toCopy = oldSize;

            memcpy( dataPtr, m_buf, toCopy );

            delete[] m_buf;           /* release old buffer */
         }
         m_buf = dataPtr;
         m_size = size;

         setLastError(NO_ERROR);
      }
   }
   return getStatus(oldSize);
}

/* ------------------------------------------------------------------------- */

int fmem::grow(int size)
{
   return realloc( getSize() + size );
}

/* ------------------------------------------------------------------------- */

int fmem::free()
{
   setLastError(UNDEFINED);
   if ( m_lockCnt != 0 )
   {
      setLastError(OBJECT_MAPPED);
   } else
   {
      if ( m_buf != NULL )
      {
         delete[] m_buf;
         m_buf = NULL;
         m_size = 0;
      }
      setLastError(NO_ERROR);
   }

   if( getStatus() != -1 )
   {
      freeObjList();
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void *fmem::map(void)
{
   void *retVal = NULL;
   setLastError(UNDEFINED);

   if ( m_buf == NULL )
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
         retVal = m_buf;
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

int fmem::append(void *ptr,int size)
{
   int oldSize = getSize();
   setLastError(UNDEFINED);
   if ( ptr == NULL )
   {
      setLastError(BAD_PARAMETER);
   } else
   {
      if ( m_lockCnt != 0 )
      {
         setLastError(OBJECT_MAPPED);
      } else
      {
         int ofs;
         if ( m_buf == NULL )
         {
            alloc(size);
            ofs = 0;
         } else
         {
            ofs = m_size;
            realloc(m_size + size);
         }

         if ( getStatus() != -1 )
         {
            char *dstPtr;

            dstPtr = (char*)map();
            if ( dstPtr == NULL)
            {
               /* failed */
            } else
            {
               memcpy( dstPtr + ofs, ptr, size );
               unmap();
               setLastError(NO_ERROR);
            }
         }
      }
   }
   return getStatus(oldSize);
}

/* ------------------------------------------------------------------------- */

int fmem::addObj(const fobject &obj)
{
   setLastError(UNDEFINED);

   if ( m_objCnt >= m_objAllocated )
   {
      growObjList();
   } else
   {
      setLastError(NO_ERROR);
   }

   if ( getStatus() != -1 )
   {
      m_objList[m_objCnt++] = obj;
      setLastError(NO_ERROR);
   }
   return getLastError();
}

/* ------------------------------------------------------------------------- */

int fmem::growObjList(void)
{
   fobject *objList;
   int objAllocated;
   setLastError(UNDEFINED);

   objAllocated = m_objAllocated + FMEM_OBJS_GROW_BY;
   objList = new fobject[objAllocated];
   if ( objList == NULL )
   {
      setLastError(ALLOC_FAILED);
   } else
   {
      if ( m_objList != NULL )
      {
         for ( int n = 0; n < m_objAllocated; n ++ )
         {
            objList[n] = m_objList[n];
         }
      }

      m_objList = objList;
      m_objAllocated  = objAllocated;

      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::freeObjList(void)
{
   setLastError(UNDEFINED);
   if ( m_objList != NULL )
   {
      delete[] m_objList;
      m_objList = NULL;
      m_objCnt = 0;
      m_objAllocated = 0;
      setLastError(NO_ERROR);
   } else
   {
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::zero(void)
{
   void *ptr;
   setLastError(UNDEFINED);

   ptr = map();
   if ( getStatus() == -1 )
   {
      /* failed */
   } else
   {
      memset( ptr, 0, getSize() );
      unmap();
      setLastError(NO_ERROR);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int fmem::getId(void)
{
   int objId;
   setLastError(UNDEFINED);

   if ( m_objList == NULL )
   {
      setLastError(BAD_OBJECT_ID);
   } else
   {
      objId = m_objList[0].getId();
      setLastError(NO_ERROR);
   }
   return getStatus(objId);
}

/* ------------------------------------------------------------------------- */
