#include <stdio.h>

#include "farchive.h"

class A
{
public:

   template<typename TYPE>
   void func( const TYPE &x )
   {
   }
};

struct str
{
   int a;
};

int main(int /*argc*/, char */*argv*/[])
{
   farchive f;

   f.create("test.arc");
   f.close();

   A a;
   /*str i = { 1 };*/
   int i = 100;

   a.func<int>(i);


   return 0;
}
