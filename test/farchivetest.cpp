#include <stdio.h>
#include <string.h>

#include "farchiveidx.h"
#include "fmem.h"

struct DATA
{
   int a;
   int b;
};

void test_fmem()
{
   farchive f;
   fmem m(f);

   f.create("test.arc");

   m.setSize(1024);

   char *p = (char*)m.map();

   if ( p!= NULL )
   {
      memset( p, 0xaa, m.getSize() );
   }
   m.unmap();

   m.write();

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
