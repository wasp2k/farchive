#include <stdio.h>
#include <string.h>

#include "farchive.h"
#include "fmem.h"
#include "ffile.h"

struct DATA
{
   int a;
   int b;
};

char buf[128] = {};

void test_fmem()
{
   farchive f;

   f.create("test.arc");

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
      memset( p + 1024, 0xbb, 1024 );
   }
   m.unmap();
   f.write(m);

   m.realloc(4096);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p + 2048, 0xcc, 2048 );
   }
   m.unmap();
   f.write(m);


   m.realloc(128);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p, 0xdd, 128 );
   }
   m.unmap();
   f.write(m);

   m.realloc(8192);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p, 0xee, 8192);
   }
   m.unmap();
   f.write(m);

   f.close();


   f.open("test.arc");
   f.moveTo(2);

   fmem m2;
   f.read(m2);

   {
      ffile f;
      f.create("raw.raw");
      f.write(m2.map(),m2.getSize());
      m2.unmap();
      f.close();
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
