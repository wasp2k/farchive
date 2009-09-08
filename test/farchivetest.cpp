#include <stdio.h>
#include <string.h>

#include "farchiveidx.h"
#include "fmem.h"

struct DATA
{
   int a;
   int b;
};

char buf[128] = {};

void test_fmem()
{
   fmem  m;
   memset(buf,0xaa,sizeof(buf));
   m.append(buf,sizeof(buf));
   memset(buf,0xbb,sizeof(buf));
   m.append(buf,sizeof(buf));
   m.free();

#if 0
   farchive f;
   fmem m;
   fmem m2;

   f.create("test.arc");

   m.setSize(1024);
   char *p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p, 0xaa, 1024 );
   }
   m.unmap();
   f.write(m);

   m.setSize(2048);
   p = (char*)m.map();
   if ( p!= NULL )
   {
      memset( p + 1024, 0xbb, 1024 );
   }
   m.unmap();
   f.write(m);

   f.moveFirst();

   //m2.read(f);
   f.close();
#endif

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
