#include "farchiveidx.h"

#define FARCHIVEIDX_FIRST_INDEX_ID     2

/* ------------------------------------------------------------------------- */

farchiveidx::farchiveidx()
{
   m_firstData = NULL;
}

/* ------------------------------------------------------------------------- */

farchiveidx::~farchiveidx()
{
   if ( isOpen() )
   {
      close();
   }
}

/* ------------------------------------------------------------------------- */

int farchiveidx::open(const char *fileName)
{
   setLastError(UNDEFINED);
   if ( farchive::open(fileName) == -1 )
   {
      /* failed */
   } else
   {
      if ( readIndex() == -1 )
      {
         /* failed */
      } else
      {
         moveFirst();
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void farchiveidx::freeIndex(void)
{
   while( m_firstData )
   {
      DATA *data = m_firstData;
      m_firstData = data->nextData;
      delete data;
   }
}

/* ------------------------------------------------------------------------- */

int farchiveidx::close(void)
{
   setLastError(UNDEFINED);
   freeIndex();
   farchive::close();
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::createFileHeader(void)
{
   fobjectT<INDEX> obj;
   obj.zero();

   setLastError(UNDEFINED);

   if ( farchive::createFileHeader() == -1 )    /* Create standard header */
   {
      /* failed */
   } else
   {
      if ( farchive::add(obj) == -1 )           /* Add empty index table */
      {
         /* failed */
      } else
      {
         if ( obj.getId() != FARCHIVEIDX_FIRST_INDEX_ID )
         {
            /* First index must have object ID = 2 */
            FERR( "Bad index object ID: %d", obj.getId());
            setLastError(BAD_INDEX_OBJECT);
         } else
         {
            setLastError(NO_ERROR);
         }
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::readIndex(void)
{
   setLastError(UNDEFINED);
   if ( moveFirst() == -1 )      /* Move to first object, must be lastID record */
   {
      /* failed */
   } else
   {
      if ( moveNext() == -1 )    /* Move to index table */
      {
         /* failed */
      } else
      {
         if ( m_currObj.getId() != FARCHIVEIDX_FIRST_INDEX_ID )      /* Check object ID */
         {
            FERR( "Bad index object ID: %d", m_currObj.getId());
            setLastError(BAD_INDEX_OBJECT);
         } else
         {
            DATA *data;
            do
            {
               data = new DATA;
               data->nextData = m_firstData;
               m_firstData = data;

               if ( getObject(data->obj) == -1 )
               {
                  /* failed */
               } else
               {
                  FDBG( "readIndex index at ofs: %d %p Next: %d", data->obj.getOfs(), data, data->obj->nextObjId );

                  INDEX &idx = *((INDEX*)data->obj.getPtr());
                  if ( idx.nextObjId != 0 )
                  {
                     if ( moveTo(idx.nextObjId) == -1 )
                     {
                        /* Failed */
                     }
                  }
               }
            } while( (data->obj->nextObjId != 0) && (getStatus() != -1));
         }
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::add(fobject &obj)
{
   setLastError(UNDEFINED);
   if ( farchive::add(obj) == -1 )
   {
      /* failed */
   } else
   {
      FDBG( "add obj: %d at: %d siz: %d", obj.getId(), obj.getOfs(), obj.getSize() );
      registerIndex(obj);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::findObject(const unsigned int objId, DATA *(&data), int &dataIdx )
{
   DATA *internalData = m_firstData;
   int found = 0;
   int searchTo;

   setLastError(UNDEFINED);

   if ( objId == 0 )
      searchTo = FINDEX_SIZE;       /* Reserve last item for next index block location */
   else
      searchTo = FINDEX_SIZE + 1;

   while( ( internalData != NULL ) && (found==0) )
   {
      INDEX &idx = *((INDEX*)internalData->obj.getPtr());
      for ( int n = 0; n < searchTo; n ++ )
      {
         ITEM &item = idx.items[ n ];
         if ( item.id == objId )
         {
            FDBG( "findObject obj: %d found at at: %d", item.id, item.ofs );
            data = internalData;
            dataIdx = n;
            found = 1;
            break;
         }
      }
      internalData = internalData->nextData;
   }

   if ( !found )
   {
      FDBG( "findObject obj: %d not found", objId );
   }
   setLastError(NO_ERROR);

   return getStatus(found);
}

/* ------------------------------------------------------------------------- */

int farchiveidx::registerIndex(const fobject &obj)
{
   DATA  *data = NULL;
   int dataIdx;
   int res;

   res = findObject( 0, data, dataIdx );
   if ( res == 0 )
   {
      DATA *newIdxBlock = new DATA;
      if ( newIdxBlock == NULL )
      {
         setLastError(ALLOC_FAILED);
      } else
      {
         newIdxBlock->obj.zero();

         if ( farchive::add(newIdxBlock->obj) == -1 )
         {
            /* failed */
         } else
         {
            FDBG( "registerObject extend index obj: %d ofs: %d", newIdxBlock->obj.getId(), newIdxBlock->obj.getOfs() );
            if ( m_firstData->obj->items[FINDEX_SIZE].id != 0 )
            {
               FERR( "registerObject extend index failed ID: %d", m_firstData->obj->items[FINDEX_SIZE].id );
               setLastError(EXPAND_INDEX_FAILED);
            } else
            {
               if ( m_firstData != NULL )
               {
                  m_firstData->obj->items[FINDEX_SIZE].id = newIdxBlock->obj.getId();
                  m_firstData->obj->items[FINDEX_SIZE].ofs = newIdxBlock->obj.getOfs();
                  m_firstData->obj->items[FINDEX_SIZE].options = newIdxBlock->obj.getOptions();
                  m_firstData->obj->nextObjId = newIdxBlock->obj.getId();
                  m_firstData->obj.flush( m_file );
               }

               newIdxBlock->nextData = m_firstData;
               m_firstData = newIdxBlock;
               newIdxBlock = NULL;

               if ( findObject( 0, data, dataIdx ) == -1 )
               {
                  setLastError(EXPAND_INDEX_FAILED);
               }
            }
         }
      }

      if ( newIdxBlock != NULL )  /* Clean memory */
         delete newIdxBlock;
   }

   if ( getStatus() != -1 )
   {
      FDBG( "registerObject obj: %d %p at: %d", obj.getId(), data, dataIdx );

      ITEM &item = data->obj->items[ dataIdx ];
      item.id = obj.getId();
      item.ofs = obj.getOfs();
      item.options = obj.getOptions();
      if ( data->obj.flush(m_file) == -1 )
      {
         setLastError(data->obj);
      }
   }

   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::moveTo(const unsigned int objId)
{
   setLastError(UNDEFINED);
   int found;
   DATA  *data = NULL;
   int dataIdx;

   found = findObject( objId, data, dataIdx );
   if ( found == -1 )
   {
      /* failed */
   } else
   {
      if ( found == 1 )
      {
         ITEM &item = data->obj->items[ dataIdx ];
         if ( moveToOfs(item.ofs) == -1 )
         {
            /* failed */
         } else
         {
            setLastError(NO_ERROR);
         }
      } else
      {
         setLastError(NO_ERROR);
      }
   }
   return getLastError();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::moveToOfs(int ofs)
{
   setLastError(UNDEFINED);
   m_currObj.setOfs( ofs );
   m_currObj.readHeader( m_file );
   setLastError(m_currObj);

   FDBG( "FCArchive::moveToOfs Ofs: %d Obj: %d Siz: %d", ofs, m_currObj.getOfs(), m_currObj.getSize() );
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::remove(const unsigned int objId)
{
   DATA *data;
   int dataIdx;
   int res;

   setLastError(UNDEFINED);

   res = findObject(objId, data, dataIdx );
   if ( res == -1 )
   {
      /* failed */
   } else
   {
      if ( ( res == 0 ) || ( data == NULL ) )
      {
         setLastError(OBJECT_NOT_FOUND);
      }else
      {
         if ( farchive::remove(objId) == -1 )
         {
            /* failed */
         } else
         {
            data->obj->items[dataIdx].id = 0;
            if ( data->obj.flush(m_file) == -1 )
            {
               /* failed */
               setLastError(data->obj);
            } else
            {
               setLastError(NO_ERROR);
            }
         }
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */
