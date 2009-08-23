#include <stdio.h>

#include "farchive.h"

struct DATA
{
   int a;
   int b;
};

int main(int /*argc*/, char */*argv*/[])
{
   fobjectT<DATA> obj;

   farchive f;
   f.create("test.arc");
   for (int i = 0; i < 10; i ++ )
   {
      obj->a = i;
      obj->b = i + 100;
      f.add(obj);

      printf( "Obj added: idx: %d, id: %d ofs: %d\n", i, obj.getId(), obj.getOfs() );
   }
   f.close();

   f.open("test.arc");

   do
   {
      if ( f.getObject(obj) != -1 )
      {
         printf( "Obj read: id: %d ofs: %d\n", obj.getId(), obj.getOfs() );
      }
   } while( f.moveNext() != -1 );

   f.close();


   return 0;
}
