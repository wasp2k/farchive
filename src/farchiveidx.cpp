#include "farchiveidx.h"

#define FARCHIVEIDX_FIRST_INDEX_ID     2
#define FINDEX_GROW_BY                 512

/* ------------------------------------------------------------------------- */

farchiveidx::farchiveidx()
{
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

int farchiveidx::close(void)
{
   setLastError(UNDEFINED);
   m_index.free();
   farchive::close();
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::createArchiveHeader(void)
{
   setLastError(UNDEFINED);

   m_index.free();
   m_index.alloc( sizeof( ITEM ) * FINDEX_GROW_BY );
   m_index.zero();

   if ( farchive::createArchiveHeader() == -1 )    /* Create standard header */
   {
      /* failed */
   } else
   {
      if ( add(m_index) == -1 )         /* Add empty index table */
      {
         /* failed */
      } else
      {
         if ( m_index.getId() != FARCHIVEIDX_FIRST_INDEX_ID )
         {
            /* First index must have object ID = 2 */
            FERR1( "Bad index object ID: %d", m_index.getId());
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
            FERR1( "Bad index object ID: %d", m_currObj.getId());
            setLastError(BAD_INDEX_OBJECT);
         } else
         {
            read( m_index );
         }
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::add(fmem &mem)
{
   setLastError(UNDEFINED);
   if ( farchive::add(mem) == -1 )
   {
      /* failed */
   } else
   {
      // FDBG3( "add obj: %d at: %d siz: %d", mem.getId(), mem.getOfs(), mem.getSize() );
      registerIndex(mem);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::write(fmem &mem)
{
   setLastError(UNDEFINED);
   if ( farchive::write(mem) == -1 )
   {
      /* failed */
   } else
   {
      // FDBG3( "add obj: %d at: %d siz: %d", mem.getId(), mem.getOfs(), mem.getSize() );
      registerIndex(mem);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::findObject(const unsigned int objId, int &pos )
{
   int found = 0;
   ITEM *index;

   setLastError(UNDEFINED);

   index = (ITEM*)m_index.map();
   if ( index == NULL )
   {
      setLastError(m_index);
   } else
   {
      int indexSize = m_index.getSize();
      if ( indexSize % sizeof( ITEM ) )
      {
         setLastError(BAD_INDEX_POOL);
      } else
      {
         int idx;

         indexSize /= sizeof( ITEM );
         for ( idx = 0; (idx < indexSize) && (found == 0); idx ++ )
         {
            if ( index[idx].id == objId )
            {
               pos = idx;
               found = 1;
            }
         }
         setLastError(NO_ERROR);
      }
      m_index.unmap();
   }
   return getStatus(found);
}

/* ------------------------------------------------------------------------- */

int farchiveidx::addObject(const fobject &obj)
{
   int itemIdx;
   int res;
   ITEM *index;

   res = findObject( 0, itemIdx );
   if ( (res == 0) && (getLastError() == OBJECT_NOT_FOUND))
   {
      m_index.grow( sizeof(ITEM) * FINDEX_GROW_BY );
      res = findObject( 0, itemIdx );
   }

   if ( getStatus() == -1 )
   {
      /* failed */
   } else
   {

      index = (ITEM *)m_index.map();
      if ( index == NULL )
      {
         setLastError(m_index);
      } else
      {
         index[itemIdx].id = obj.getId();
         index[itemIdx].ofs = obj.getOfs();
         index[itemIdx].options = obj.getOptions();

         m_index.unmap();
      }
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::registerIndex(const fmem &mem)
{
   int itemIdx;
   int res;
   int idx;
   bool dirty = false;

   setLastError(NO_ERROR);
   for ( idx = 0; (idx < mem.getObjCnt()) && (getStatus() != -1);idx ++ )
   {
      const fobject &obj = mem.getObj(idx);

      res = findObject( obj.getId(), itemIdx );
      switch(res)
      {
      case 0:
         addObject( obj );
         dirty = true;
         break;
      case 1:
         /* Object already in the index table */
         break;
      default:
         break;
      }
   }

   if ( ( getStatus() != -1 ) && dirty )
   {
      write(m_index);
   }
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::moveTo(const unsigned int /*objId*/)
{
   setLastError(UNDEFINED);
#if 0
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
#endif
   return getLastError();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::moveToOfs(int ofs)
{
   setLastError(UNDEFINED);
   m_currObj.setOfs( ofs );
   m_currObj.readHeader( m_file );
   setLastError(m_currObj);

   FDBG3( "FCArchive::moveToOfs Ofs: %d Obj: %d Siz: %d", ofs, m_currObj.getOfs(), m_currObj.getSize() );
   return getStatus();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::remove(const unsigned int /*objId*/)
{
   setLastError(UNDEFINED);
#if 0
   DATA *data;
   int dataIdx;
   int res;


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
#endif
   return getStatus();
}

/* ------------------------------------------------------------------------- */
