#include "farchiveidx.h"

/* ------------------------------------------------------------------------- */

farchiveidx::farchiveidx()
{
   m_firstData = NULL;
}

/* ------------------------------------------------------------------------- */

farchiveidx::~farchiveidx()
{
   close();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::open(const char *fileName)
{
   setLastError(UNDEFINED);
   farchive::open(fileName);
   readIndex();
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
   setLastError(UNDEFINED);
   fobjectT<INDEX> obj;
   obj.zero();

   farchive::createFileHeader();
   farchive::add(obj);
   return getStatus();
}

/* ------------------------------------------------------------------------- */

void farchiveidx::readIndex(void)
{
   moveNext();
   if ( m_currObj.getId() != 2 )
   {
      /*throw( new FCException( FCException::BAD_INDEX_OBJECT ) );*/
   }

   DATA *data;
   do
   {
      data = new DATA;
      data->nextData = m_firstData;
      m_firstData = data;

      getObject(data->obj);
      /*FCDbg( 9, "farchiveidx::readIndex index at ofs: %d %p Next: %d", data->obj.getOfs(), data, data->obj->nextObjId );*/

      INDEX &idx = *((INDEX*)data->obj.getPtr());

      if ( idx.nextObjId != 0 )
      {
         if ( !moveTo(idx.nextObjId) )
         {
            /*throw( new FCException( FCException::BAD_INDEX_OBJECT ) );*/
         }
      }

   } while( data->obj->nextObjId != 0 );
}

/* ------------------------------------------------------------------------- */

int farchiveidx::add(fobject &obj)
{
   setLastError(UNDEFINED);
   farchive::add(obj);

   FDBG( "farchiveidx::add obj: %d at: %d siz: %d", obj.getId(), obj.getOfs(), obj.getSize() );
   registerIndex(obj);
   return getStatus();
}

/* ------------------------------------------------------------------------- */

bool farchiveidx::findObject(const unsigned int objId, DATA *(&data), int &dataIdx )
{
   DATA *internalData = m_firstData;
   bool found = false;
   int searchTo;

   if ( objId == 0 )
      searchTo = FINDEX_SIZE;
   else
      searchTo = FINDEX_SIZE + 1;

   while( ( internalData != NULL ) && !found )
   {
      for ( int n = 0; n < searchTo; n ++ )
      {
         ITEM &item = internalData->obj->records[ n ];
         if ( item.id == objId )
         {
            FDBG( "farchiveidx::findObject obj: %d found at at: %d", item.id, item.ofs );
            data = internalData;
            dataIdx = n;
            found = true;
            break;
         }
      }
      internalData = internalData->nextData;
   }

   if ( !found )
   {
      FDBG( "farchiveidx::findObject obj: %d not found", objId );
   }

   return found;
}

/* ------------------------------------------------------------------------- */

void farchiveidx::registerIndex(const fobject &obj)
{
   DATA  *data = NULL;
   int dataIdx;

   if ( !findObject( 0, data, dataIdx ) )
   {
      DATA *newIdxBlock = new DATA;
      if ( newIdxBlock == NULL )
      {
         /*throw( new FCException( FCException::ALLOC_FAILED ) );*/
      }
      newIdxBlock->obj.zero();
      farchive::add(newIdxBlock->obj);
      FDBG( "farchiveidx::registerObject extend index obj: %d ofs: %d", newIdxBlock->obj.getId(), newIdxBlock->obj.getOfs() );

      if ( m_firstData->obj->records[FINDEX_SIZE].id != 0 )
      {
         /*throw( new FCException( FCException::EXPAND_INDEX_FAILED ) );*/
      }

      if ( m_firstData != NULL )
      {
         m_firstData->obj->records[FINDEX_SIZE].id = newIdxBlock->obj.getId();
         m_firstData->obj->records[FINDEX_SIZE].ofs = newIdxBlock->obj.getOfs();
         m_firstData->obj->records[FINDEX_SIZE].options = newIdxBlock->obj.getOptions();
         m_firstData->obj->nextObjId = newIdxBlock->obj.getId();
         m_firstData->obj.flush( m_file );
      }

      newIdxBlock->nextData = m_firstData;
      m_firstData = newIdxBlock;

      if ( !findObject( 0, data, dataIdx ) )
      {
         /*throw( new FCException( FCException::EXPAND_INDEX_FAILED ) );*/
      }
   }

   FDBG( "farchiveidx::registerObject obj: %d %p at: %d", obj.getId(), data, dataIdx );

   ITEM &item = data->obj->records[ dataIdx ];
   item.id = obj.getId();
   item.ofs = obj.getOfs();
   item.options = obj.getOptions();
   data->obj.flush(m_file);
}

/* ------------------------------------------------------------------------- */

int farchiveidx::moveTo(const unsigned int objId)
{
   setLastError(UNDEFINED);
   bool found;
   DATA  *data = NULL;
   int dataIdx;

   found = findObject( objId, data, dataIdx );
   if ( found )
   {
      ITEM &item = data->obj->records[ dataIdx ];
      moveToOfs(item.ofs);
   }
   return getLastError();
}

/* ------------------------------------------------------------------------- */

int farchiveidx::moveToOfs(int ofs)
{
   setLastError(UNDEFINED);
   m_currObj.setOfs( ofs );
   m_currObj.readHeader( m_file );
   FDBG( "FCArchive::moveToOfs Ofs: %d Obj: %d Siz: %d", ofs, m_currObj.getOfs(), m_currObj.getSize() );
   return getStatus();
}

/* ------------------------------------------------------------------------- */
