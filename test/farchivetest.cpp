#include <stdio.h>
#include <string.h>

#include "farchive.h"
#include "farchiveidx.h"
#include "fmem.h"
#include "ffile.h"

struct DATA
{
   int a;
   int b;
};

char buf[128] = {};

void test_addObj(farchive &f, int pattern)
{
   fmem m;
   m.alloc(1024);
   char *p = (char*)m.map();
   if ( p != NULL )
      memset( p, 0xaa, 1024 );
   m.unmap();

   f.add(m);

   m.realloc(2048);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p + 1024, pattern, 1024 );
   }
   m.unmap();
   f.write(m);

   m.realloc(4096);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p + 2048, pattern, 2048 );
   }
   m.unmap();
   f.write(m);


   m.realloc(128);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p, pattern, 128 );
   }
   m.unmap();
   f.write(m);

   m.realloc(8192);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p, pattern, 8192);
   }
   m.unmap();
   f.write(m);
}

void test_fmem()
{
   farchiveidx f;

   f.create("test.arc");
   for ( int i = 0; i < 1024; i ++ )
   {
      test_addObj( f, i );
   }
   f.close();
}

int main(int /*argc*/, char */*argv*/[])
{
   test_fmem();


#if 0
   fobjectT<DATA> obj;

   farchiveidx f;
   f.create("test.arc");
   for (int i = 0; i < 1024; i ++ )
   {
      obj->a = i;
      obj->b = i + 100;
      f.add(obj);

      printf( "Obj added: idx: %d, id: %d ofs: %d siz: %d\n", i, obj.getId(), obj.getOfs(), obj.getSize() );
   }
   f.close();

   f.open("test.arc");

   do
   {
      if ( f.getObject(obj) != -1 )
      {
         printf( "Obj read: id: %d ofs: %d siz: %d\n", obj.getId(), obj.getOfs(), obj.getSize() );
      }
   } while( f.moveNext() != -1 );

   if ( f.moveTo(900) != -1 )
   {
      if ( f.getObject(obj) != -1 )
      {
         printf( "Obj move: id: %d ofs: %d siz: %d\n", obj.getId(), obj.getOfs(), obj.getSize() );

         f.remove(obj.getId());
      }
   }

   f.moveFirst();
   do
   {
      if ( f.getObject(obj) != -1 )
      {
         printf( "Obj read: id: %d ofs: %d siz: %d\n", obj.getId(), obj.getOfs(), obj.getSize() );
      }
   } while( f.moveNext() != -1 );

   f.close();
#endif

   return 0;
}
