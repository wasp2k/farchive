#include <stdio.h>

#include "farchive.h"

int main(int /*argc*/, char */*argv*/[])
{
   FDBG( "Hello World %d", 123 );

   farchive f;

   f.create("test.arc");
   f.close();


   return 0;
}
